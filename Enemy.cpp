#include <chrono>
#include "Enemy.h"
#include "Bullet.h"
#include "graphics.h"
#include "Boom.h"

unordered_set<EnemyPtr> Enemy::st;
unordered_set<EnemyPtr> Enemy::to_remove;

Enemy::Enemy(object *attacker, const double x, const double y, const double rad) :
    Plane(x, y), attacker_(attacker), rad_(rad)
{
    loadimage(&img, L"assets/enemy.png", PLANEWIDTH, PLANEHEIGHT);
    rotateimage(&img, &img, rad, WHITE);
    speed_ = rand() % 1000 / 500.0f;
    sendBulletTime = rand() % 200 + 800;
    pre_ = chrono::steady_clock::now();
}

Enemy::Enemy(const Enemy &e) :
    Plane(e.x_, e.y_), attacker_(e.attacker_), rad_(e.rad_)
{
    img = e.img;
    speed_ = rand() % 1000 / 500.0f;
    sendBulletTime = rand() % 200 + 800;
    pre_ = chrono::steady_clock::now();
}

void Enemy::clear()
{
    st.clear();
}

void Enemy::sendBullet()
{
    double rad = atanf((attacker_->y_ - y_) / (attacker_->x_ - x_));
    if (attacker_->x_ < x_)
    {
        rad += PI;
    }
    Bullet::create(this->obj(), x_ + 10, y_ + 70, rad);
}

void Enemy::private_draw()
{
    x_ += speed_ * sinf(rad_);
    y_ += speed_ * cosf(rad_);
    putimage(x_, y_, &img);
    auto now = chrono::steady_clock::now();
    auto diff = chrono::duration_cast<std::chrono::milliseconds>(now - pre_);
    if (diff.count() > sendBulletTime)
    {
        sendBullet();
        pre_ = now;
    }
    if (x_ < 0 || x_ > MAXSIZE || y_ > MAXSIZE || isHit())
    {
        to_remove.insert(shared_from_this());
        Boom::create(x_, y_);
    }
}

void Enemy::draw()
{
    IMAGE boom;
    loadimage(&boom, L"assets/boom1.png", PLANEWIDTH, PLANEHEIGHT);
    for (auto &p : st)
    {
        p->private_draw();
    }
    for (auto &p : to_remove)
    {
        st.erase(p);
    }
    to_remove.clear();
}

EnemyPtr Enemy::create(object *attacker, const double x, const double y, const double rad)
{
    auto enemy = make_shared<Enemy>(attacker, x, y, rad);
    st.insert(enemy);
    return enemy;
}