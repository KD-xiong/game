#include "Bar.h"
#include <graphics.h>

Bar::Bar(const double x, const double y, const double width,
         const double height, const double maxSize, double &curSize) :
    object(x, y), curSize_(curSize), maxSize_(maxSize), width_(width), height_(height)
{
}

Bar::Bar(const Bar &b) :
    object(b.x_, b.y_), curSize_(b.curSize_), maxSize_(b.maxSize_), width_(b.width_), height_(b.height_)
{
}

Bar &Bar::operator=(const Bar &b)
{
    x_ = b.x_;
    y_ = b.y_;
    curSize_ = b.curSize_;
    maxSize_ = b.maxSize_;
    width_ = b.width_;
    height_ = b.height_;
    return *this;
}

void Bar::draw()
{
    setlinecolor(BLACK);
    setfillcolor(YELLOW);
    rectangle(x_, y_, x_ + width_, y_ + height_);
    fillrectangle(x_, y_, x_ + width_ * (curSize_ / maxSize_), y_ + height_);
}