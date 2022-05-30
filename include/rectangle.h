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

/**
 * @brief Класс прямоугольника
 *
 * @tparam T тип данных точки
 */
template<typename T>
class Rectangle{
 public:
  //! Юго-западный (нижний левый) угол
  Point<T> sw;
  //! Северо-восточный (верхний правый) угол
  Point<T> ne;
  //! Идентификатор
  int id;

  /**
   * @brief Специальный конструктор для прямоугольника.
   *
   * @param _sw Юго-западный (нижний левый) угол
   * @param _ne Северо-восточный (верхний правый) угол
   * @param _id Идентификатор
   *
   * Создать прямоугльник параллеьный осям по двум точкам
   */
  Rectangle(const Point<T>& _sw, const Point<T>& _ne, int _id = -1):
  sw(_sw), ne(_ne), id(_id)
  {}
  /**
   * @brief Конструктор копирования.
   *
   * @param _obj копируемый объект
   *
   * Естественно копирует поля
   */
  Rectangle(const Rectangle &obj) {
    sw = obj.sw; ne = obj.ne; id = obj.id;
  }
  /**
   * @brief Конструктор поумолчанию
   *
   * Создает вырожденный одноточечный прямоугольник
   */
  Rectangle():
  sw(Point<T>(0, 0)), ne(Point<T>(0, 0)), id(-1)
  {}
  /**
   * @brief Деструктор
   */
  ~Rectangle() {}
  /**
   * @brief Прегруженный оператор присваивания
   *
   * @return Ссылку на объект метода
   * Естественно копирует поля
   */
  Rectangle& operator=(const Rectangle& obj) {
    sw = obj.sw; ne = obj.ne; id = obj.id;
    return *this;
  }
};

}  // namespace geometry

#endif  // INCLUDE_RECTANGLE_H_
