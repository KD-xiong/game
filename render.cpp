#pragma once
#include "render.hpp"
#include <graphics.h>
#include <string>
#include <thread>
#include "Plane.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Menu.hpp"
#include "HP.h"

using namespace std;

// 从Plane.cpp中获取游戏暂停信号
extern bool stop;

// 从Plane.cpp中获取游戏结束信号
extern bool isEnd;

void clearForRender()
{
    // 清除所有的实体
    Plane::init();
    Boom::clear();
    Bullet::clear();
    Enemy::clear();
    HP::init();

    // 确保全部清除
    this_thread::sleep_for(chrono::milliseconds(100));
}

void start_render()
{
    clearForRender();
    // 初始化图形窗口和渲染缓冲区
    initgraph(MAXSIZE, MAXSIZE);
    setbkcolor(WHITE);
    BeginBatchDraw();
    settextcolor(BLACK);
    settextstyle(20, 0, _T("宋体"));

    // 初始化Plane
    PlanePtr p = Plane::create(375, 700);

    // 初始化字符串
    wstring tmp, FPScontext, timeStr;

    // 启动消息循环，获取当前的消息时间
    thread event(&Plane::message_event, p);

    // 若游戏未结束，不断渲染画面
    while (!isEnd)
    {
        // 若游戏暂停，提示信息并且跳过其他物体的渲染
        if (stop)
        {
            stopOp();
            outtextxy(100, 100, tmp.c_str());
            FlushBatchDraw();
            showHelp();
            if (!stop)
            {
                system("cls");
            }
            continue;
        }

        // 随机生成Enemy的位置
        if (rand() % 100 / 100000.0f < MODE)
        {
            Enemy::create(p.get(), rand() % 700 + 50, 100);
        }

        // 随机生成HP的位置
        if (!(rand() % 200))
        {
            HP::create(rand() % 700 + 50, rand() % 700 + 50);
        }

        auto start_t = std::chrono::steady_clock::now();
        // 清屏并重新绘制
        cleardevice();

        // 渲染字体
        tmp = L"得分:" + to_wstring(Plane::score);
        outtextxy(100, 100, tmp.c_str());
        outtextxy(100, 60, FPScontext.c_str());
        outtextxy(100, 250, timeStr.c_str());

        // 渲染实体
        Bullet::draw();
        Enemy::draw();
        p->draw();
        HP::draw();
        Boom::draw();
        // 如果游戏结束，渲染将阻塞在p的渲染中

        FlushBatchDraw();
        // 控制帧率（避免过快)，通过计算渲染时间差，来控制帧率
        auto end_t = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t);
        if (duration.count() < NEEDTOSLEEP)
        {
            Sleep(NEEDTOSLEEP - duration.count());
            FPScontext = L"FPS:" + to_wstring(FPS);
        }
        else
        {
            FPScontext = L"FPS:" + to_wstring((int)(1000.0f / (chrono::duration_cast<std::chrono::milliseconds>(duration)).count() - NEEDTOSLEEP));
        }
    }

    event.join();

    EndBatchDraw();
    closegraph();
}