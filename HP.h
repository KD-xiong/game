#pragma once
#include "object.h"
#include "Plane.h"
#include <vector>
#include <memory>
#include <graphics.h>
#include <unordered_set>

using namespace std;

// 利用智能指针，省去手动管理内存回收
#define HPPtr shared_ptr<HP>

class HP : public object
{
private:
    static IMAGE img;
    void private_draw();

public:
    HP();
    HP(const double x, const double y);
    HP(const HP& h);

    // 初始化状态(默认状态是清空所有成员，并初始化一颗HP)
    static void init();

    // 静态成员函数，用于渲染
    static void draw();

    // 生成一颗HP并加入生命值中
    static HPPtr create();

    // 生成一颗available的HP，可被Plane吸收
    static HPPtr create(const double x, const double y);

    // 生命值中的HP
    static vector<HPPtr> member;
    
    // 随机生成的可被吸收的HP
    static unordered_set<HPPtr> available;

    // 动态清除HP
    static unordered_set<HPPtr> to_remove;

    // 通过友元函数，方便处理Plane接受到available的HP的信息处理
    friend bool Plane::accept();
};