#pragma once
#include <graphics.h>

class Plane {
protected:
    double x_, y_;
    double speed_;
    IMAGE img;

public:
    void draw();
    Plane(double x = 110.0f, double y = 0.0f);
};
