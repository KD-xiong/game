#pragma once
#include <memory>
#include <graphics.h>
#include <unordered_set>
#include "Enemy.h"
#include "object.h"

using namespace std;

// 利用智能指针，省去手动管理内存回收
#define BulletPtr shared_ptr<Bullet>

class Bullet : public enable_shared_from_this<Bullet>, public object
{
private:
    // 子弹速度作为成员变量，可以设置随机子弹射速，带来随机性和趣味性
    double speed_;
    // 子弹的飞行
    double rad_;
    // 子弹的图片
    IMAGE img_;
    // 该子弹的私有绘制过程
    void private_draw();
    // 子弹是哪个物体发射的
    object *father;

    // 使用st来储存子弹指针
    // 使用unordered_set，减少查找的时间
    static unordered_set<BulletPtr> st;
    static unordered_set<BulletPtr> to_remove;

public:
    // Bullet的构造函数，传入坐标角度和速度信息
    Bullet(object *fa, double x, double y, double rad, double speed);

    Bullet(const Bullet& b);

    // 静态成员函数create，将智能指针插入绘画列表当中
    static BulletPtr create(object *fa, double x = 20.0f, double y = 20.0f, double rad = 0, double speed = 5.0f);

    // 清除所有Boom成员
    static void clear();

    // 静态成员函数draw，绘画所有子弹
    static void draw();

    // 友元函数方便Plane获得子弹信息
    friend bool Plane::isHit();
};