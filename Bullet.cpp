#include "Bullet.h"
#include "define.h"

unordered_set<BulletPtr> Bullet::st;
unordered_set<BulletPtr> Bullet::to_remove;

Bullet::Bullet(object *fa, double x, double y, double rad, double speed) :
    object(x, y), speed_(speed), rad_(rad), father(fa)
{
    loadimage(&img_, L"assets/bullet.png", 30, 20);
    rotateimage(&img_, &img_, -rad, WHITE);
}

Bullet::Bullet(const Bullet &b) :
    object(b.x_, b.y_), speed_(b.speed_), rad_(b.rad_), father(b.father)
{
}

void Bullet::private_draw()
{
    // 沿着角度飞行
    y_ += sinf(rad_) * speed_;
    x_ += cosf(rad_) * speed_;
    putimage(x_, y_, &img_);

    // 超出屏幕范围后，插入to_remove，之后进行资源的回收
    if (x_ > MAXSIZE || y_ > MAXSIZE)
    {
        to_remove.insert(shared_from_this());
    }
}

void Bullet::draw()
{
    for (auto &p : st)
    {
        p->private_draw();
    }

    // 回收资源
    for (auto &p : to_remove)
    {
        st.erase(p);
    }

    // 防止资源二次回收，通过智能指针管理能更好的管理类的析构
    to_remove.clear();
}

BulletPtr Bullet::create(object *fa, double x, double y, double rad, double speed)
{
    auto bullet = make_shared<Bullet>(fa, x, y, rad, speed);
    st.insert(bullet);
    return bullet;
}

void Bullet::clear()
{
    st.clear();
}