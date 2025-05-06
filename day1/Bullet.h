#pragma once
#include <memory>
#include <graphics.h>
#include <unordered_set>
#include "Enemy.h"

using namespace std;

#define BulletPtr shared_ptr<Bullet>

class Bullet : public enable_shared_from_this<Bullet> {
private:
    double x_, y_;
    double speed_;
    double rad_;
    IMAGE img_;
    void private_draw();

public:
    static unordered_set<BulletPtr> st;
    static unordered_set<BulletPtr> to_remove;
    Bullet(double x, double y, double rad, double speed);
    static BulletPtr create(double x = 20.0f, double y = 20.0f, double rad = 0, double speed = 5.0f);
    static void draw();
    friend bool Enemy::isHit();
};