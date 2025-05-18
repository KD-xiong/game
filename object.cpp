#include "object.h"
object::object(double x, double y) :
    x_(x), y_(y)
{
}

void object::draw()
{
}

object *object::obj()
{
    return this;
}