#include "Bullet.h"
#include "define.h"

unordered_set<BulletPtr> Bullet::st;
unordered_set<BulletPtr> Bullet::to_remove;

Bullet::Bullet(double x, double y, double rad, double speed) :
    x_(x), y_(y), speed_(speed), rad_(rad) {
    loadimage(&img_, L"assets/bullet.png", 30, 20);
    rotateimage(&img_, &img_, -rad, WHITE);
}

void Bullet::private_draw() {
    y_ += sinf(rad_) * speed_;
    x_ += cosf(rad_) * speed_;
    putimage(x_, y_, &img_);
    if (x_ > MAXSIZE || y_ > MAXSIZE) {
        to_remove.insert(shared_from_this());
    }
}

void Bullet::draw() {
    for (auto &p : st) {
        p->private_draw();
    }
    for (auto &p : to_remove) {
        st.erase(p);
    }
    to_remove.clear();
}

BulletPtr Bullet::create(double x, double y, double rad, double speed) {
    auto bullet = make_shared<Bullet>(x, y, rad, speed);
    st.insert(bullet);
    return bullet;
}