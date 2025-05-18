#pragma once
#include <memory>
#include <graphics.h>
#include <unordered_set>
#include "object.h"
#include "Bar.h"

using namespace std;

// 利用智能指针，省去手动管理内存回收
#define PlanePtr shared_ptr<Plane>

class Plane : public object
{
private:
    // 子弹数量的进度条
    Bar bulletBar;

    // 处理键盘的消息
    void message_event_key();

    // 处理鼠标信息
    void message_event_mouse();

    // 技能F，处理键盘F事件
    void sendBulletF();

    // 处理键盘R事件
    void pressR();

    // 标记场上的Plane数量，方便之后clear操作管理
    static unordered_set<PlanePtr> PlaneSt;


protected:
    double speed_;
    IMAGE img;

public:
    Plane(const double x = 110.0f, const double y = 0.0f);

    static bool signal_;
    static int score;
    // 渲染Plane
    void draw();

    // 消息循环
    void message_event();

    // 判断是否击中
    bool isHit();

    // 判断是否接受到HP
    bool accept();

    // 发送当前的坐标信息
    object *sendObj();

    // 生成一个Plane成员
    static PlanePtr create(const double x = 110.0f, const double y = 0.0f);

    // 清除所有Plane成员
    static void init();
};
