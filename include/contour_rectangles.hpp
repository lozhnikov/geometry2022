/**
 * @file include/contour_rectangles.hpp
 * @author Boris Shevchenko
 *
 * Реализация алгоритма нахождения контура объединения прямоугольников.
 */

#ifndef INCLUDE_CONTOUR_RECTANGLES_HPP_
#define INCLUDE_CONTOUR_RECTANGLES_HPP_

#include <iostream>
#include <list>
#include <utility>
#include <vector>
#include <edge.hpp>
#include "axis_parallel_edge.h"
#include "dictionary.h"
#include "rectangle.h"


namespace geometry {
/**
 * @brief Сортирует массив
 * @tparam T тип данных элеменов массива
 *  
 * @param arr Сортируемый массив
 * @param size Размер массив
 * @param cmp Компоратор
 */
template <typename T> void Sort(T *arr, size_t size, int (*cmp)(T, T));
/**
 * @brief Специалный компоратор для класса AxisParallelEdge
 * @tparam T тип данных координат точек
 *
 * @param a Указатель на первый сравниваемый объект
 * @param b Указатель на второй сравниваемый объект
 *
 * @return 1 если a > b; -1 если a < b; 0 если a = b
 */
template <typename T>
int AxisParallelEdgeCmp(AxisParallelEdge<T> *a, AxisParallelEdge<T> *b);
/**
 * @brief Строит массив указателей ключевых точек для алгоритма (упорядоченный по х,
 * массив из вертикальных ребер)
 * @tparam T тип данных координат точек
 *
 * @param 1 Массив прямоугольников
 * @param 2 Их количество
 *
 * @return Массив указателей на ребра вертильные ребра
 */
template <typename T> AxisParallelEdge<T> **BuildSchedule(Rectangle<T>*,
                                                          size_t);

/**
 * @brief Строит контур объединения прямоугольников
 * @tparam T тип данных координат точек
 *
 * @param r Массив прямоугольников
 * @param n Их количество
 *
 * @return Указатель на список указателей на ребра контура
 */
template <typename T>
std::list<Edge<T> *> *ContourRectangles(Rectangle<T> r[], int n) {
  AxisParallelEdge<T> **schedule = BuildSchedule(r, static_cast<size_t>(n));
  std::list<Edge<T> *> *segments = new std::list<Edge<T> *>;
  std::vector<AxisParallelEdge<T> *> _arr;
  Dictionary<AxisParallelEdge<T> *> sweepline(AxisParallelEdgeCmp<T>, _arr);
  Rectangle<T> *sentinel = new Rectangle<T>(Point<T>(AxisParallelEdge<T>::T_MIN,
                                                    AxisParallelEdge<T>::T_MIN),
                                            Point<T>(AxisParallelEdge<T>::T_MAX,
                                               AxisParallelEdge<T>::T_MAX), -1);
  sweepline.Insert(new AxisParallelEdge<T>(sentinel, Side::BOTTOM_SIDE));

  for (size_t i = 0; i < static_cast<size_t>(2 * n); i++)
    switch (schedule[i]->type) {
    case Side::LEFT_SIDE:
      schedule[i]->HandleLeftEdge(&sweepline, segments);
      break;
    case Side::RIGHT_SIDE:
      schedule[i]->HandleRightEdge(&sweepline, segments);
      break;
    case Side::BOTTOM_SIDE:
    case Side::TOP_SIDE:
    default:
      break;
    }

  delete sentinel;
  for (size_t i = 0; i < static_cast<size_t>(2 * n); i++) {
    delete schedule[i];
  }
  delete[] schedule;
  for (size_t i = 0; i < static_cast<size_t>(sweepline.size); i++) {
    delete sweepline.arr[i];
  }
  /*for (auto iter = segments->begin(); iter != segments->end(); iter++)
  {
    std::cout << (*iter)->Origin().X() << " " << (*iter)->Origin().Y() << " ";
    std::cout << (*iter)->Destination().X() << " " <<
  (*iter)->Destination().Y(); std::cout << '\n'; }std::cout << '\n';*/
  // Вывести результат в виде набора ребер

  return segments;
}

template <typename T>
AxisParallelEdge<T> **BuildSchedule(Rectangle<T> r[], size_t n) {
  AxisParallelEdge<T> **schedule = new AxisParallelEdge<T> *[2 * n];
  for (size_t i = 0; i < n; i++) {
    schedule[2 * i] = new AxisParallelEdge<T>(&r[i], Side::LEFT_SIDE);
    schedule[2 * i + 1] = new AxisParallelEdge<T>(&r[i], Side::RIGHT_SIDE);
  }
  Sort(schedule, 2 * n, AxisParallelEdgeCmp<T>);
  return schedule;
}

template <typename T>
int AxisParallelEdgeCmp(AxisParallelEdge<T> *a, AxisParallelEdge<T> *b) {
  if (a->Pos() < b->Pos())
    return -1;
  else if (a->Pos() > b->Pos())
    return 1;
  else if (a->type < b->type)
    return -1;
  else if (a->type > b->type)
    return 1;
  else if (a->r->id < b->r->id)
    return -1;
  else if (a->r->id > b->r->id)
    return 1;
  return 0;
}

template <typename T> void Sort(T *arr, size_t size, int (*cmp)(T, T)) {
  for (size_t i = 0; i < size - 1; i++)
    for (size_t j = 0; j < size - 1; j++)
      if (cmp(arr[j], arr[j + 1]) > 0)
        std::swap(arr[j], arr[j + 1]);
}

}  // namespace geometry

#endif  // INCLUDE_CONTOUR_RECTANGLES_HPP_

