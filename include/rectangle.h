/**
 * @file include/rectangle.h
 * @author Boris Shevchenko
 *
 * Определение и реализация методов класса прямоугольник.
 */

#ifndef INCLUDE_RECTANGLE_H_
#define INCLUDE_RECTANGLE_H_

#include <iostream>
#include <point.hpp>

namespace geometry {

template<typename T>
class Rectangle{
 public:
  Point<T> sw;  // юго-западный (нижний левый) угол
  Point<T> ne;  // северо-восточный (верхний правый) угол
  int id;

  Rectangle(const Point<T>& _sw, const Point<T>& _ne, int _id = -1):
  sw(_sw), ne(_ne), id(_id)
  {}
  Rectangle(const Rectangle &obj) {
    sw = obj.sw; ne = obj.ne; id = obj.id;
  }
  Rectangle():
  sw(Point<T>(0, 0)), ne(Point<T>(0, 0)), id(-1)
  {}
  ~Rectangle() {}
  Rectangle& operator=(const Rectangle& obj) {
    sw = obj.sw; ne = obj.ne; id = obj.id;
    return *this;
  }
};

}  // namespace geometry

#endif  // INCLUDE_RECTANGLE_H_
