#include <graphics.h>
#include <functional>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include "Plane.h"
#include "Record.h"
#include "Bullet.h"
#include "Enemy.h"
#include "define.h"
#include "Menu.hpp"
#include "HP.h"

// 标记游戏是否结束
bool isEnd;
// 标记游戏是否暂停
bool stop;

// 初始化信息
// 用于线程安全
mutex mut_;
int Plane::score = 0;
bool Plane::signal_ = false;
ExMessage msg_key;
ExMessage msg_mouse;
const int *scorePtr = nullptr;
double remainBullet = 50;

bool isR = false;
unordered_set<PlanePtr> Plane::PlaneSt;

void Plane::pressR()
{
    if (remainBullet == 50 || isR)
    {
        return;
    }
    // 将数据锁起来，以防其他线程进行修改
    lock_guard<mutex> lockGuard(mut_);
    isR = true;
    // 换弹时间
    this_thread::sleep_for(chrono::milliseconds(600));
    remainBullet = 50;
    isR = false;
    // 结束后自动释放锁
}

void Plane::sendBulletF()
{
    if (isR)
    {
        return;
    }
    // 将数据锁起来，以防其他线程进行修改
    lock_guard<mutex> lockGuard(mut_);
    for (size_t i = 1; i <= 10 && remainBullet > 0; i++)
    {
        Bullet::create(this->obj(), x_ + 20, y_ - 30, PI / 11 * i + PI);
        --remainBullet;
    }
    // 结束后自动释放锁
}

Plane::Plane(const double x, const double y) :
    object(x, y), bulletBar(100, 140, 150, 40, 50, remainBullet)
{
    loadimage(&img, L"assets/plane.png", PLANEWIDTH, PLANEHEIGHT);
    speed_ = 4e-7;
    stop = false;
    isEnd = false;
}

PlanePtr Plane::create(const double x, const double y)
{
    auto ptr = make_shared<Plane>(x, y);
    PlaneSt.insert(ptr);
    return ptr;
}

// 在此处重置，为重新渲染做准备
void Plane::init()
{
    // 初始化分数和子弹信息
    score = 0;
    remainBullet = 50;
    scorePtr = nullptr;

    // 清空飞机
    PlaneSt.clear();
}

object *Plane::sendObj()
{
    return Plane::object::obj();
}

void Plane::message_event()
{
    ExMessage msg;
    // 启动键盘消息处理线程
    // 防止多次peekmessage，导致消息处理的遗漏
    // 并加快消息处理的效率，使得操作更加连贯
    thread key(bind(&Plane::message_event_key, this));
    while (!isEnd)
    {
        while (peekmessage(&msg, WM_KEYDOWN | WM_KEYUP | WM_LBUTTONDOWN | WM_LBUTTONUP))
        {
            if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
            {
                msg_key = msg;
            }
            else
            {
                msg_mouse = msg;
                message_event_mouse();
            }
        }
    }
    // 等待键盘消息处理线程结束
    key.join();
}

bool Plane::isHit()
{
    for (auto &p : Bullet::st)
    {
        if (p->father == this->obj())
        {
            continue;
        }
        if (fabs(p->x_ - 25 - x_) <= 30 && fabs(p->y_ - y_ - 25) <= 30)
        {
            Bullet::to_remove.insert(p);
            score++;
            return true;
        }
    }
}

bool Plane::accept()
{
    for (auto &p : HP::available)
    {
        if (fabs(p->x_ - 25 - x_) <= 30 && fabs(p->y_ - y_ - 10) <= 30)
        {
            HP::to_remove.insert(p);
            return true;
        }
    }
}

void Plane::message_event_key()
{
    while (!isEnd)
    {
        if (msg_key.message == WM_KEYDOWN)
        {
            // 处理ESC暂停消息
            if (msg_key.vkcode == VK_ESCAPE)
            {
                stop = !stop;
                while (msg_key.message == WM_KEYDOWN && msg_key.vkcode == VK_ESCAPE);
            }
            if (stop)
            {
                continue;
            }
            if (msg_key.vkcode == 'A')
            {
                // 按 A 键移动图片
                x_ -= speed_;
                if (x_ > MAXSIZE || x_ < 0)
                {
                    x_ += speed_;
                }
            }
            else if (msg_key.vkcode == 'D')
            {
                // 按 D 键移动图片
                x_ += speed_;
                if (x_ > MAXSIZE - PLANEWIDTH || x_ < 0)
                {
                    x_ -= speed_;
                }
            }
            else if (msg_key.vkcode == 'W')
            {
                // 按 W 键移动图片
                y_ -= speed_;
                if (y_ > MAXSIZE || y_ < 0)
                {
                    y_ += speed_;
                }
            }
            else if (msg_key.vkcode == 'S')
            {
                // 按 S 键移动图片
                y_ += speed_;
                if (y_ > MAXSIZE - PLANEHEIGHT || y_ < 0)
                {
                    y_ -= speed_;
                }
            }
            else if (msg_key.vkcode == 'R')
            {
                // 换弹处理
                thread t(&Plane::pressR, this);
                t.detach();

                // 修复bug，防止循环开设进程，一开始写的时候疯狂掉帧，内存占用从4m-140m
                while (msg_key.message == WM_KEYDOWN && msg_key.vkcode == 'R');
            }
            else if (msg_key.vkcode == 'F')
            {
                // 发射技能
                sendBulletF();
                while (msg_key.message == WM_KEYDOWN && msg_key.vkcode == 'F');
            }
        }
    }
}

void Plane::message_event_mouse()
{
    // 处理暂停事件
    if (stop)
    {
        return;
    }
    // 利用signal规避多次执行相同操作
    // 鼠标按下松开弹起后才能进行下次操作
    // 利用静态变量，减少性能损耗的同时，能够完成正常的逻辑处理
    static bool signal = true;
    if (msg_mouse.message == WM_LBUTTONDOWN && signal)
    {
        signal = false;
        if (remainBullet <= 0 || isR)
        {
            return;
        }
        double rad = atanf((msg_mouse.y - y_) / (msg_mouse.x - x_));
        if (msg_mouse.x < x_)
        {
            rad += PI;
        }
        lock_guard<mutex> lockGuard(mut_);
        Bullet::create(this->obj(), x_ + 20, y_ - 30, rad);
        --remainBullet;
    }
    else if (msg_mouse.message == WM_LBUTTONUP)
    {
        signal = true;
    }
}

void Plane::draw()
{
    // 绘画出与Plane相关的信息
    putimage(x_, y_, &img);
    bulletBar.draw();
    wstring remain = L"剩余子弹数: " + to_wstring((int)remainBullet) + L"/50";
    outtextxy(100, 200, remain.c_str());

    // 处理换弹事件
    if (isR)
    {
        outtextxy(100, 230, L"换弹中……");
    }

    // 处理接受到available的HP事件
    if (accept())
    {
        // 创造一个新的HP加入member数组中
        auto hpp = HP::create();

        // 创造一个生成新的HP的爆炸效果
        auto ptr = Boom::create(hpp->obj()->x_, hpp->obj()->y_, 175, true);

        HP::draw();
        Boom::draw();
        FlushBatchDraw();
    }

    // 处理被子弹击中的事件
    if (isHit())
    {
        // 减少一颗HP的东海
        auto ptr = Boom::create(HP::member.back()->obj()->x_, HP::member.back()->obj()->y_, 120, true);
        HP::member.pop_back();
        // 因为自己被击中会导致分数增加一，这里恢复真实的分数
        --score;
        Boom::create(x_, y_, 150);
        HP::draw();
        Boom::draw();
        FlushBatchDraw();

        if (HP::member.empty())
        {
            // 更新游戏状态
            isEnd = true;
            // 将成绩传入
            scorePtr = &score;

            // 启动一个新的线程处理弹窗信息，避免影响后续的界面信息提示，从而影响游戏体验
            static HWND hwnd = GetHWnd();
            thread messageBox(MessageBox, hwnd, L"游戏结束!", L"Game Over!", MB_OK);
            messageBox.detach();

            // 打印界面信息
            printMenu();
            // 等待处理信息
            system("pause");
            exit(0);
        }
    }
}