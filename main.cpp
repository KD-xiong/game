#include <time.h>
#include <cstdlib>
#include "Menu.hpp"

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
int main()
{
    // 设置随机种子，确保每次游戏都不一样
    srand(time(0));
    // 启动渲染
    printMenu();
    return 0;
}