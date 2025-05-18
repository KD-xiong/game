#include "Boom.h"
#include "define.h"
#include <chrono>
#include <graphics.h>
#include <unordered_set>

using namespace std;

// 使用st来储存超出爆炸时间待去除的指针
// 使用unordered_set和unordered_map，减少查找的时间
unordered_set<BoomPtr> st;
unordered_map<BoomPtr, chrono::steady_clock::time_point> Boom::mp;

Boom::Boom(const double x, const double y, int duration, bool isStar) :
    object(x, y), duration_(duration)
{
    if (isStar)
    {
        loadimage(&img, L"assets/star.png", 25, 25);
    }
    else
    {
        loadimage(&img, L"assets/boom1.png", PLANEWIDTH, PLANEHEIGHT);
    }
}


Boom::Boom(const Boom &b) :
    object(b.x_, b.y_), duration_(b.duration_)
{
    img = b.img;
}

void Boom::clear()
{
    mp.clear();
}

BoomPtr Boom::create(const double x, const double y, int duration, bool isStar)
{
    auto ptr = make_shared<Boom>(x, y, duration, isStar);
    mp[ptr] = chrono::steady_clock::now();
    return ptr;
}

void Boom::draw()
{
    // 利用两个容器来删除指针，避免使用一个单独的容器删除键带来的迭代器错误
    for (auto &p : mp)
    {
        auto diff = chrono::duration_cast<std::chrono::milliseconds>(chrono::steady_clock::now() - mp[p.first]);
        // 爆炸持续时间为duration_
        if (diff.count() < p.first->duration_)
        {
            // 利用时间差绘制随机一点的爆炸效果，有1/5的概率获得随机爆炸效果
            if (diff.count() % 5 == 0)
                rotateimage(&(p.first->img), &(p.first->img), diff.count(), WHITE);
        }
        else
        {
            // 时间差超出duration_，加入st集合中准备删除
            st.insert(p.first);
        }
        // 绘制爆炸图片
        putimage(p.first->x_, p.first->y_, &(p.first->img));
    }

    // 利用st集合，删除已经超出爆炸时间的Boom成员
    for (auto p : st)
    {
        mp.erase(p);
    }
    // 清除st成员，重复利用
    st.clear();
}