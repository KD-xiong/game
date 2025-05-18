#pragma once
#include "object.h"

class Bar : public object
{
protected:
    double maxSize_;
    double width_;
    double height_;
    double &curSize_;

public:
    Bar(const double x, const double y, const double width,
        const double height, const double maxSize, double &curSize);

    Bar(const Bar &b);

    Bar &operator=(const Bar &b);

    // 进度条的渲染
    void draw();
};