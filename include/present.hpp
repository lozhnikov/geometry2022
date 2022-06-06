/**
 * @file include/present.hpp
 * @author Ivan Yanushevich
 *
 * Реализация алгоритма построения выпуклой оболочки.
 */

#ifndef INCLUDE_PRESENT_HPP_
#define INCLUDE_PRESENT_HPP_

#include <cstddef>
#include <utility>
#include "polygon.hpp"
#include "point.hpp"
#include "common.hpp"

namespace geometry {

/**
 * @brief Алгоритм построения выпуклой оболочки.
 *
 * @param s Массив точек.
 * @param n Количество элементов в массиве.
 *
 * Функция формирует выпуклую оболочку конечного набора точек
 * методом "заворачивания подарка".
 */

template<typename T>
Polygon<T>* Present(Point<T> s[], int n) {
  int a, i;
  for (a = 0, i = 1; i < n; i++)
    if (s[i] < s[a])
      a = i;
  s[n] = s[a];
  Polygon<T>* p = new Polygon<T>;
  for (int m = 0; m < n; m++) {
    std::swap(s[a], s[m]);
    p->Insert(s[m]);
    a = m + 1;
    for (i = m+2; i <= n; i++) {
      Position c = s[i].Classify(s[m], s[a], static_cast<T>(0.001));
      if ((c == Position::Left) || (c == Position::Beyond))
        a = i;
    }
    if (a == n) {
      Polygon<T>* result(p);
      return result;
    }
  }
  return NULL;
}
}  // namespace geometry

#endif  // INCLUDE_PRESENT_HPP_
