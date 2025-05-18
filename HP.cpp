#include "HP.h"
#include "define.h"

// 初始化变量
IMAGE HP::img;
vector<HPPtr> HP::member;
unordered_set<HPPtr> HP::available;
unordered_set<HPPtr> HP::to_remove;

void HP::init()
{
    // 载入图片
    loadimage(&img, L"assets/HP.png", 25, 25);

    // 清空member和available成员即所有的HP成员
    member.clear();
    available.clear();

    // 初始化HP
    create();
}

HP::HP() :
    object(member.size() * 30 + 200, 25)
{
}

HP::HP(const double x, const double y) :
    object(x, y)
{
}


HP::HP(const HP &h) :
    object(h.x_, h.y_)
{
}

void HP::private_draw()
{
    putimage(x_, y_, &img);
}

void HP::draw()
{
    outtextxy(100, 25, L"生命值 :");
    for (auto &p : member)
    {
        p->private_draw();
    }
    for (auto &p : available)
    {
        p->y_ += 4.0f;
        if (p->y_ > MAXSIZE)
        {
            to_remove.insert(p);
        }
        p->private_draw();
    }
    for (auto &p : to_remove)
    {
        available.erase(p);
    }
    to_remove.clear();
    FlushBatchDraw();
}

HPPtr HP::create()
{
    auto ptr = make_shared<HP>();
    member.emplace_back(ptr);
    return ptr;
}

HPPtr HP::create(const double x, const double y)
{
    auto ptr = make_shared<HP>(x, y);
    available.insert(ptr);
    return ptr;
}