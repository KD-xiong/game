#include <graphics.h>
#include <chrono>
#include "Plane.h"
#include "Bullet.h"
#include "Enemy.h"
#include "define.h"

Plane::Plane(double x, double y) :
    x_(x), y_(y) {
    loadimage(&img, L"assets/plane.png", PLANEWIDTH, PLANEHEIGHT);
}

void Plane::draw() {
    ExMessage msg;
    auto t_start = std::chrono::high_resolution_clock::now();
    while (peekmessage(&msg, EX_KEY | EX_MOUSE)) {
        if (msg.message == WM_KEYDOWN) {
            if (msg.vkcode == 'A') { // 按 A 键移动图片
                x_ -= 10;
                if (x_ > MAXSIZE || x_ < 0) {
                    x_ += 10;
                }
            } else if (msg.vkcode == 'D') { // 按 D 键移动图片
                x_ += 10;
                if (x_ > MAXSIZE || x_ < 0) {
                    x_ -= 10;
                }
            } else if (msg.vkcode == 'W') { // 按 W 键移动图片
                y_ -= 10;
                if (y_ > MAXSIZE || y_ < 0) {
                    y_ += 10;
                }
            } else if (msg.vkcode == 'S') { // 按 S 键移动图片
                y_ += 10;
                if (y_ > MAXSIZE || y_ < 0) {
                    y_ -= 10;
                }
            }
        } else if (msg.message == WM_LBUTTONDOWN) {
            double rad = atanf((msg.y - y_) / (msg.x - x_));
            if (msg.x < x_) {
                rad += PI;
            }
            Bullet::create(x_ + 5, y_ + 10, rad);
        }
    }
    // 清屏并重新绘制
    cleardevice();
    putimage(x_, y_, &img);
    Bullet::draw();
    Enemy::draw();
    FlushBatchDraw();
    // 控制帧率（避免过快）
    auto t_end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start);
    if (duration.count() < NEEDTOSLEEP) {
        Sleep((int)(NEEDTOSLEEP - duration.count()));
    }
}