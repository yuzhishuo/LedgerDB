/*
 * @Author: Leo
 * @Date: 2022-07-22 09:08:07
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-26 02:18:44
 */

#include <exception>
#include <gtest/gtest.h>
#include <utility/Mixin.h>
using namespace yuzhi::tool;

class Point
{
public:
  Point(int x, int y) : x_(x), y_(y) {}
  int x() const { return x_; }
  int y() const { return y_; }

  void setX(int x) { x_ = x; }
  void setY(int y) { y_ = y; }

private:
  int x_;
  int y_;
};

class Drawable
{

public:
  Drawable(Point *p) : p_(p) {}
  void draw() { std::cout << "Drawable::draw()" << std::endl; }

private:
  Point *p_;
};

class Moveable
{
public:
  Moveable(Drawable *p) : p_(p) {}
  void move() { std::cout << "Moveable::move()" << std::endl; }

private:
  Drawable *p_;
};

using DrawableMixin = Mixin<Point, Drawable, Moveable>;
TEST(MixinTEST, Mixin_Point_Test)
{
  DrawableMixin d(1, 2);
  d.draw();
  d.x();
  d.move();
}
