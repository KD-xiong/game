#pragma once
#include "Menu.hpp"
#include <string>
#include <iostream>
#include <graphics.h>
#include "Record.h"
#include "define.h"
#include "render.hpp"

using namespace std;

extern const int *scorePtr;

void stopOp()
{
    static IMAGE stopImg;
    loadimage(&stopImg, L"assets/stop.png", 95 * 4, 53 * 4);
    putimage(MAXSIZE / 4, MAXSIZE / 2, &stopImg);
}

void showAllRecord()
{
    Record::read();
    Record::showInfo();
}

void recordRecord()
{
    // 先读取所有记录，然后写入所有记录
    // 通过这种方式实现修改
    Record::read();
    wchar_t name[30];
    InputBox(name, 30, L"记录的名称：");
    Record::records.push_back(Record(*scorePtr, name));

    // 获取最终分数的排名位置
    int rank = Record::write();

    // 显示当前所有的分数排名信息
    cout << "记录列表为:\n";
    Record::showInfo();

    // 显示最终分数的排名位置信息
    cout << "\n你的排名记录是:\n";
    cout << "第" << rank + 1 << "名:" << Record::records[rank] << "\n";
}

void searchRecords()
{
    system("cls");
    // 现将文件读取进来
    Record::read();

    // 读入需要查询的目标名称
    cout << "请输入需要的查询的目标:";
    string target;
    getline(cin, target);

    // 遍历所有的记录，找出与目标相同的记录
    Record::search(target);
}

void printMenu()
{
    // 清空控制台输出的东西，起到刷新的作用
    system("cls");
    cout << "1 : 记录成绩\n"
         << "2 : 查询成绩\n"
         << "3 : 查看所有成绩\n"
         << "4 : 清空成绩\n"
         << "h : 查看游戏指南帮助\n"
         << "r : (重新)开始游戏\n"
         << "q : 退出\n"
         << "Tips :\n"
         << "\n*第一次进行游戏前建议选择h*\n"
         << "\n*记录存储在records.dat中*\n"
         << "*为了能够正常运行程序*\n"
         << "*请先检查是否在运行程序的目录下是否存在该文件*\n";

    // 获取选项信息
    char choice;
    cin >> choice;
    cin.ignore();

    // 判断选择的内容
    if (choice == '1')
    {
        if (scorePtr)
        {
            recordRecord();
        }
        else
        {
            cout << "*请先运行游戏再来记录吧!\n";
        }
    }
    else if (choice == '2')
    {
        searchRecords();
    }
    else if (choice == '3')
    {
        showAllRecord();
    }
    else if (choice == '4')
    {
        Record::records.clear();
        Record::write();
    }
    else if (choice == 'h')
    {
        showHelp();
    }
    else if (choice == 'r')
    {
        system("cls");
        start_render();
    }
    else if (choice == 'q')
    {
        exit(0);
    }
    cout << "\n按任意键返回上一级\n";
    system("pause");

    // 递归进入死循环，只有按q退出
    printMenu();
}

void showHelp()
{
    // 清空控制台输出的东西，起到刷新的作用
    system("cls");
    cout << "操作指南 : \n"
         << "*  W   :   向上移动\n"
         << "*  S   :   向下移动\n"
         << "*  A   :   向左移动\n"
         << "*  D   :   向右移动\n"
         << "*  R   :   填充子弹(换弹时间 0.6s)，换单时间无法射击\n"
         << "*  F   :   发射散射子弹(10发)，如果不足只能发射剩余的子弹\n"
         << "*  ESC :   暂停\n"
         << "*  鼠标 :   发射子弹\n";

    cout << "\n游戏规则 : \n"
         << "(1)射出子弹击中敌方飞机可得一分\n"
         << "   被敌方飞机击中扣除一颗HP\n"
         << "   失去所有的HP游戏结束\n "
         << "\n(2)有1/200的概率随机生成HP\n"
         << "   拾取后可以再增加一个生命值\n "
         << "\n(3)记录存储在records.dat中\n为了能够正常运行程序\n请先检查是否在运行程序的目录下是否存在该文件\n";

    // 防止刷新过快，减少io消耗，但会带来一定的延迟影响
    Sleep(450);
}