#include <graphics.h>
#include <conio.h>
#include <stdio.h> // 用于调试输出
#include <cmath>
#include <ctime>
#include <memory>
#include "define.h"
#include "Plane.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace std;

int main() {
    // 初始化图形窗口
    initgraph(MAXSIZE, MAXSIZE);
    setbkcolor(WHITE);
    cleardevice();

    // 加载图片（确保图片路径正确）
    IMAGE img, bu;
    loadimage(&img, L"plane.png", 53, 77);
    // 初始化坐标
    double x = 375;
    double y = 700;
    Plane p(x, y);

    BeginBatchDraw();
    bool running = true;
    srand(time(0));
    while (running) {
        p.draw();
        if (rand() % 100 / 100000.0f < MODE) {
            Enemy::create(rand() % 700 + 50, 100);
        }
    }

    EndBatchDraw();
    closegraph();
    return 0;
}