#include "Record.h"
#include <fstream>
#include <algorithm>
#include <graphics.h>
#include <locale>
#include <codecvt>

using namespace std;

vector<Record> Record::records;

string wchar_to_gbk(const wchar_t *wstr)
{
    // 第一次调用获取目标缓冲区大小
    int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (len <= 0) return ""; // 错误处理

    // 分配内存并转换
    char *gbk_str = new char[len];
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, gbk_str, len, NULL, NULL);

    std::string result(gbk_str);
    // 释放内存
    delete[] gbk_str;
    return result;
}

Record::Record() :
    score_(0)
{
    // 初始化数据区，将名称全部清零
    memset(name_, 0, sizeof(wchar_t) * 30);
}

Record::Record(const int score, const wchar_t name[30]) :
    score_(score)
{
    // 深拷贝
    memcpy(name_, name, sizeof(wchar_t) * 30);
}

ostream &operator<<(ostream &out, Record &record)
{
    out << wchar_to_gbk(record.name_) << "的分数:" << record.score_;
    return out;
}

void Record::showInfo()
{
    size_t len = records.size();
    for (size_t i = 0; i < len; i++)
    {
        cout << "第" << i + 1 << "名\n"
             << records[i] << "\n";
    }
    if (len == 0)
    {
        cout << "当前无记录!\n";
    }
}

void Record::read(const wstring &str)
{
    Record::records.clear();
    fstream file(str, ios::in | ios::binary);
    static HWND hwnd = GetHWnd();
    // 处理文件无法打开情况
    if (!file)
    {
        MessageBox(hwnd, L"错误", L"无法打开文件!", MB_OK);
        return;
    }

    // 初始化buffer
    size_t len;
    int score;
    wchar_t name[30];

    // 通过while读入数据，能够避免eof的错误情况
    while (file.read(reinterpret_cast<char *>(&score), sizeof(score)))
    {
        // 读取score成功后，继续读取len和name
        file.read(reinterpret_cast<char *>(name), sizeof(wchar_t) * 30);
        if (file.fail())
        {
            MessageBox(hwnd, L"错误", L"无法读取文件!", MB_OK);
        }
        records.push_back(Record(score, name));
    }
    file.close();
}

Record &Record::operator=(const Record &x)
{
    score_ = x.score_;
    // 深拷贝
    memcpy(name_, x.name_, sizeof(wchar_t) * 30);
    return *this;
}

bool Record::operator<(const Record &y) const
{
    if (score_ == y.score_)
    {
        for (int i = 0; i < 30; i++)
        {
            if (name_[i] != y.name_[i])
            {
                return name_[i] < y.name_[i];
            }
        }
    }
    else
    {
        return score_ > y.score_;
    }
}

bool Record::operator==(const Record &other) const
{
    if (score_ != other.score_)
    {
        return false;
    }
    for (int i = 0; i < 30; i++)
    {
        if (name_[i] != other.name_[i])
        {
            return false;
        }
    }
    return true;
}

int Record::write(const wstring &str)
{
    Record end_;
    // 非空才能取出
    if (!records.empty())
        end_ = records.back();

    // 用快速排序法进行排序
    sort(records.begin(), records.end());

    fstream file(str, ios::out | ios::binary);
    static HWND hwnd = GetHWnd();

    // 处理文件无法打开情况
    if (!file)
    {
        MessageBox(hwnd, L"错误", L"无法打开文件!", MB_OK);
        return -1;
    }
    size_t ans = -1;
    for (size_t i = 0; i < records.size(); i++)
    {
        if (records[i] == end_)
        {
            ans = i;
        }
        file.write(reinterpret_cast<const char *>(&records[i].score_), sizeof(records[i].score_));
        file.write(reinterpret_cast<const char *>(records[i].name_), sizeof(wchar_t) * 30);
    }
    file.close();
    return ans;
}

void Record::search(const string &name)
{
    size_t len = records.size();
    int res = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (name == wchar_to_gbk(records[i].name_))
        {
            cout << "第" << i + 1 << "名\n"
                 << records[i] << "\n";
            ++res;
        }
    }
    if (res == 0)
    {
        cout << "当前无记录!\n";
    }
}