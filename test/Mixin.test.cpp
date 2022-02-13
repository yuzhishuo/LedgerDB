/*
 * @Author: Leo
 * @Date: 2022-02-01 23:35:03
 * @LastEditTime: 2022-02-02 00:13:58
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/test/Mixin.test.cpp
 */

#include <exception>
#include <gtest/gtest.h>
#include <utility/Mixin.h>
using namespace yuzhi::tool;

class Point {
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

class Drawable {

public:
  Drawable(Point *p) : p_(p) {}
  void draw() { std::cout << "Drawable::draw()" << std::endl; }

private:
  Point *p_;
};

class Moveable {
public:
  Moveable(Drawable *p) : p_(p) {}
  void move() { std::cout << "Moveable::move()" << std::endl; }

private:
  Drawable *p_;
};

using DrawableMixin = Mixin<Point, Drawable, Moveable>;
TEST(MixinTEST, Mixin_Point_Test) {
  DrawableMixin d(1, 2);
  d.draw();
  d.x();
  d.move();
}
