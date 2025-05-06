#pragma once
#include <memory>
#include <unordered_set>
#include "Plane.h"
#include "define.h"

using namespace std;

#define EnemyPtr shared_ptr<Enemy>

class Enemy : public Plane, public enable_shared_from_this<Enemy> {
private:
    void private_draw();

public:
    bool isHit();
    static unordered_set<EnemyPtr> st;
    static unordered_set<EnemyPtr> to_remove;
    Enemy(double x, double y);
    static EnemyPtr create(double x, double y);
    static void draw();
};
