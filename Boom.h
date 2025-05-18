#pragma once
#include "object.h"
#include <chrono>
#include <memory>
#include <graphics.h>
#include <unordered_map>

using namespace std;

// 利用智能指针，省去手动管理内存回收
#define BoomPtr shared_ptr<Boom>

class Boom : public object
{
protected:
    // 爆炸的持续时间
    int duration_;

    // 储存效果的图片
    IMAGE img;

    // 静态hashmap成员变量储存爆炸开始时间，以控制爆炸过程
    static unordered_map<BoomPtr, chrono::steady_clock::time_point> mp;

public:
    // Boom的构造函数，传入坐标
    Boom(const double x, const double y, int Duration = 175, bool isStar = false);

    Boom(const Boom& b);

    // 利用静态成员函数create，将Boom成员加入mp中
    static BoomPtr create(const double x, const double y, int Duration = 175, bool isStar = false);

    // 利用静态成员draw，绘制所有爆炸动画
    static void draw();

    // 清除所有Boom成员
    static void clear();
};