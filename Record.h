#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Record
{
private:
    // 记录的分数
    int score_;
    // 记录的持有者名称
    wchar_t name_[30];

public:
    Record();
    Record(const int score, const wchar_t name[30]);

    // 静态成员变量记录当前所有的记录信息
    static vector<Record> records;

    // 二进制文件读写
    static void read(const wstring &str = L"records.dat");
    static int write(const wstring &str = L"records.dat");

    // 友元函数重载<<运算符，方便格式化输出
    friend ostream &operator<<(ostream &out, Record &record);

    // 静态成员函数，显示当前records所有信息
    static void showInfo();

    // 重载=运算符，拷贝复制
    Record &operator=(const Record &x);

    // 重载<运算符，快排的比价函数
    bool operator<(const Record &y) const;

    // 重载==运算符，用于查找信息
    bool operator==(const Record &other) const;

    // 在records里面查询相关的记录信息
    static void search(const string &name);
};
