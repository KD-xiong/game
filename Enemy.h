#pragma once
#include <map>
#include <set>
#include <memory>
#include <graphics.h>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include "Plane.h"
#include "define.h"
#include "Boom.h"

using namespace std;

// 利用智能指针，省去手动管理内存回收
#define EnemyPtr shared_ptr<Enemy>

// public enable_shared_from_this<Enemy> : 使得可以从类中使用shared_from_this从而返回智能指针
class Enemy : public Plane, public enable_shared_from_this<Enemy>
{
private:
    // 使用st来储存Enemy指针
    // 使用unordered_set，减少查找的时间
    static unordered_set<EnemyPtr> st;
    static unordered_set<EnemyPtr> to_remove;

    // 设置发射子弹的时间间隔，随机该间隔带来随机性与趣味性
    int sendBulletTime;

    // 上个发射子弹的时间，与sendBulletTime搭配
    chrono::steady_clock::time_point pre_;

    // 私有绘制过程
    void private_draw();

    // 攻击的目标坐标等信息
    object *attacker_;

    // 飞行角度，但是与预期不符，最终选择默认方案
    double rad_;

public:
    // 发射朝向Plane玩家的子弹
    void sendBullet();

    // Enemy构造函数，传入攻击目标和自身坐标
    Enemy(object *attacker, const double x, const double y, const double rad = 0.0f);

    Enemy(const Enemy &e);

    // 静态成员函数create，将智能指针插入绘画列表当中
    static EnemyPtr create(object *attacker, const double x, const double y, const double rad = 0.0f);

    // 静态成员函数draw，绘制所有Enemy
    static void draw();

    // 清除所有Enemy成员
    static void clear();
};