/**
 * @file include/axis_parallel_edge.h
 * @author Boris Shevchenko
 *
 * Определение и частичная реализация методов класса ребер параллельных осям.
 */

#ifndef INCLUDE_AXIS_PARALLEL_EDGE_H_
#define INCLUDE_AXIS_PARALLEL_EDGE_H_

#include <iostream>
#include <list>
#include <limits>
#include "dictionary.h"
#include "rectangle.h"

namespace geometry {
/**
* Вид ребра в прямоугольнике
*/
enum Side { LEFT_SIDE, RIGHT_SIDE, BOTTOM_SIDE, TOP_SIDE };

/**
 * @brief Класс параллельного оси ребра
 *
 * @tparam T тип данных координат точек
 */
template <typename T> class AxisParallelEdge {
 public:
  //! Максимальное значение типа T
  static const T T_MAX;
  //! Минимальное значение типа T
  static const T T_MIN;
  //! Прямоугольник родитель
  Rectangle<T> *r;
  //! Количество прямоугольников между текущим ребром и следующим
  int count;
  //! Минимальная координата
  T m;
  //! Тип ребра
  Side type;

  /**
   * @brief Специальный конструктор для ребра.
   *
   * @param _r Указатель на прямоугольник родитель.
   * @param _type тип ребра.
   *
   * Создать ребро данного типа от этого прямоугольника.
   */
  AxisParallelEdge(Rectangle<T> *_r, Side _type)
      : r(_r), count(0), m(AxisParallelEdge<T>::T_MIN), type(_type) {}
  /**
   * @brief Получить координату \f$ x \f$ нижней точки, если это вертикальное ребро,
     а если горизонтальное то \f$ y \f$ у левой точки
   * @return координату \f$ х \f$ или \f$ y \f$, в зависимости от типа
   */
  T Pos();
  /**
   * @brief Получить координату \f$ y \f$ нижней точки, если это вертикальное ребро,
     а если горизонтальное то \f$ x \f$ у левой точки
   * @return координату \f$ х \f$ или \f$ y \f$, в зависимости от типа
   */
  T Min();
  /**
   * @brief Получить координату \f$ y \f$ верхней точки, если это вертикальное ребро,
     а если горизонтальное то \f$ x \f$ у правой точки
   * @return координату \f$ х \f$ или \f$ y \f$, в зависимости от типа
   */
  T Max();
  /**
   * @brief изменить m
   */
  void SetMin(T);
  /**
   * @brief Оброботчик левого ребра(для алгоритма) 
   *
   * @param 1 Указатель на словарь ребер, которых пересекает сканирующая линия.
   * @param 2 Указатель на список отсканированных элементов контура.
   *
   * На входе в левое ребро прямоугольника, добавляет, в зависимости
   * от count ребер, в сканируещей линии их сегменты в контур.
   * Добавляет в сканирующую линию верхнее и нижнее ребро от \f$ r \f$.
   */
  void HandleLeftEdge(Dictionary<AxisParallelEdge<T> *> *,
                      std::list<Edge<T> *> *);
  /**
   * @brief Оброботчик правого ребра(для алгоритма) 
   *
   * @param 1 Указатель на словарь ребер, которых пересекает сканирующая линия.
   * @param 2 Указатель на список отсканированных элементов контура.
   *
   * На выходе из правого ребра прямоугольника, добавляет, в зависимости
   * от count ребер, в сканируещей линии их сегменты в контур.
   * Удаляет из сканирующей линии верхнее и нижнее ребро от \f$ r \f$.
   */
  void HandleRightEdge(Dictionary<AxisParallelEdge<T> *> *,
                       std::list<Edge<T> *> *);
};
template <typename T>
const T AxisParallelEdge<T>::T_MAX = std::numeric_limits<T>::max();
template <typename T>
const T AxisParallelEdge<T>::T_MIN = std::numeric_limits<T>::lowest();


template <typename T> T AxisParallelEdge<T>::Pos() {
  switch (type) {
  case Side::LEFT_SIDE:
    return r->sw.X();
    break;
  case Side::RIGHT_SIDE:
    return r->ne.X();
    break;
  case Side::TOP_SIDE:
    return r->ne.Y();
    break;
  case Side::BOTTOM_SIDE:
  default:
    return r->sw.Y();
    break;
  }
}

template <typename T> T AxisParallelEdge<T>::Min() {
  if (m > AxisParallelEdge<T>::T_MIN)
    return m;
  switch (type) {
  case Side::LEFT_SIDE:
  case Side::RIGHT_SIDE:
    return r->sw.Y();
    break;
  case Side::TOP_SIDE:
  case Side::BOTTOM_SIDE:
  default:
    return r->sw.X();
    break;
  }
}

template <typename T> T AxisParallelEdge<T>::Max() {
  switch (type) {
  case Side::LEFT_SIDE:
  case Side::RIGHT_SIDE:
    return r->ne.Y();
    break;
  case Side::TOP_SIDE:
  case Side::BOTTOM_SIDE:
  default:
    return r->ne.X();
    break;
  }
}

template <typename T> void AxisParallelEdge<T>::SetMin(T f) { m = f; }

template <typename T>
void AxisParallelEdge<T>::HandleLeftEdge(
  Dictionary<AxisParallelEdge<T> *> *sweepline, std::list<Edge<T> *> *segs) {
  sweepline->Insert(new AxisParallelEdge<T>(r, Side::TOP_SIDE));
  AxisParallelEdge<T> *u = sweepline->Val();
  sweepline->Insert(new AxisParallelEdge<T>(r, Side::BOTTOM_SIDE));
  AxisParallelEdge<T> *l = sweepline->Val();
  AxisParallelEdge<T> *p = sweepline->Prev();
  T curx = Pos();
  l->count = p->count + 1;
  p = sweepline->Next();
  l = sweepline->Next();
  for (; l != u; p = l, l = sweepline->Next()) {
    if ((l->type == Side::BOTTOM_SIDE) && (l->count++ == 1)) {
      segs->push_back(
          new Edge<T>(Point<T>(curx, p->Pos()), Point<T>(curx, l->Pos())));
      segs->push_back(
          new Edge<T>(Point<T>(l->Min(), l->Pos()), Point<T>(curx, l->Pos())));
    } else {if ((l->type == Side::TOP_SIDE) && (l->count++ == 0))
    {segs->push_back(
          new Edge<T>(Point<T>(l->Min(), l->Pos()), Point<T>(curx, l->Pos())));}
           }
  }
  if ((l->count = p->count - 1) == 0)
    segs->push_back(
        new Edge<T>(Point<T>(curx, p->Pos()), Point<T>(curx, l->Pos())));
}

template <typename T>
void AxisParallelEdge<T>::HandleRightEdge(
  Dictionary<AxisParallelEdge<T> *> *sweepline, std::list<Edge<T> *> *segs) {
  AxisParallelEdge<T> uedge(r, Side::TOP_SIDE);
  AxisParallelEdge<T> ledge(r, Side::BOTTOM_SIDE);
  AxisParallelEdge<T> *u = sweepline->Find(&uedge);
  AxisParallelEdge<T> *l = sweepline->Find(&ledge);
  T curx = Pos();
  if (l->count == 1)
    segs->push_back(
        new Edge<T>(Point<T>(l->Min(), l->Pos()), Point<T>(curx, l->Pos())));
  if (u->count == 0)
    segs->push_back(
        new Edge<T>(Point<T>(u->Min(), u->Pos()), Point<T>(curx, u->Pos())));
  AxisParallelEdge<T> *initl = l;
  AxisParallelEdge<T> *p = l;
  l = sweepline->Next();
  for (; l != u; p = l, l = sweepline->Next()) {
    if ((l->type == Side::BOTTOM_SIDE) && (--l->count == 1)) {
      segs->push_back(
          new Edge<T>(Point<T>(curx, p->Pos()), Point<T>(curx, l->Pos())));
      l->SetMin(curx);
    } else {if ((l->type == Side::TOP_SIDE) && (--l->count == 0))
      l->SetMin(curx);}
  }
  if (l->count == 0)
    segs->push_back(
        new Edge<T>(Point<T>(curx, p->Pos()), Point<T>(curx, l->Pos())));
  sweepline->Remove(u);
  sweepline->Remove(initl);
}

}  // namespace geometry

#endif  // INCLUDE_AXIS_PARALLEL_EDGE_H_

