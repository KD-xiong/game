#include "Enemy.h"
#include "Bullet.h"
#include "graphics.h"

unordered_set<EnemyPtr> Enemy::st;
unordered_set<EnemyPtr> Enemy::to_remove;

Enemy::Enemy(double x, double y) :
    Plane(x, y) {
    loadimage(&img, L"assets/enemy.png", PLANEWIDTH, PLANEHEIGHT);
}

bool Enemy::isHit() {
    for (auto &p : Bullet::st) {
        if (fabs(p->x_ - x_) <= 50 && fabs(p->y_ - y_) <= 50) {
            Bullet::to_remove.insert(p);
            return true;
        }
    }
}

void Enemy::private_draw() {
    y_ += 0.5;
    putimage(x_, y_, &img);
    if (y_ > MAXSIZE || isHit()) {
        to_remove.insert(shared_from_this());
    }
}

void Enemy::draw() {
    for (auto &p : st) {
        p->private_draw();
    }
    for (auto &p : to_remove) {
        st.erase(p);
    }
    to_remove.clear();
}

EnemyPtr Enemy::create(double x, double y) {
    auto enemy = make_shared<Enemy>(x, y);
    st.insert(enemy);
    return enemy;
}