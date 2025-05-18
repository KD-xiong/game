#pragma once
// 利用单独的头文件，使多个文件获得相同的常量信息，方便管理
const int MAXSIZE = 900;
const double PI = 3.1415926;
const int PLANEWIDTH = 60;
const int PLANEHEIGHT = 77;

// MODE作为游戏难度设置，控制Enemy的生成概率来控制游戏难度
const double MODE = 2.3e-6;

const int FPS = 120;
const int NEEDTOSLEEP = 1000 / FPS;