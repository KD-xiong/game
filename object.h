#pragma once
class object
{
public:
    double x_;
    double y_;
    object(double x, double y);

    // 添加虚析构函数
    virtual ~object() = default;
    virtual void draw();

    // 返回当前的位置信息
    object *obj();
};