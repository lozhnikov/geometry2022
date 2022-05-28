/**
 * @file include/dictionary.h
 * @author Boris Shevchenko
 *
 * Определение и реализация методов класса словоря.
 */

#ifndef INCLUDE_DICTIONARY_H_
#define INCLUDE_DICTIONARY_H_

#include <iostream>
#include <vector>
#include <point.hpp>

namespace geometry {

template <typename T> class Dictionary {
 public:
  int size;
  int current;
  int (*cmp)(T, T);
  std::vector<T> arr;

  Dictionary(int (*_cmp)(T, T), std::vector<T> _arr) :
  size(0), current(0), cmp(_cmp), arr(_arr)
  { arr.clear(); arr.reserve(30); }
  void Insert(T);
  T Val();
  T Prev();
  T Next();
  T Find(T);
  void Remove(T);
};

template <typename T> void Dictionary<T>::Insert(T obj) {
  for (size_t i = 0; i < static_cast<size_t>(size); i++)
    if (cmp(obj, arr[i]) < 0) {
      arr.emplace(arr.begin() + static_cast<int16_t>(i), obj);
      size++;
      current = static_cast<int>(i);
      return;
    }
  arr.push_back(obj);
  current = size;
  size++;
}

template <typename T> T Dictionary<T>::Val()
  { return arr[static_cast<size_t>(current)]; }

template <typename T> T Dictionary<T>::Prev() {
  current--;
  return arr[static_cast<size_t>(current)]; }

template <typename T> T Dictionary<T>::Next() {
  current++;
  return arr[static_cast<size_t>(current)]; }

template <typename T> T Dictionary<T>::Find(T obj) {
  for (size_t i = 0; i < static_cast<size_t>(size); i++)
    if (cmp(arr[i], obj) == 0) {
      current = static_cast<int>(i);
      return arr[i];
    }
  return nullptr;
}

template <typename T> void Dictionary<T>::Remove(T obj) {
  for (size_t i = 0; i < static_cast<size_t>(size); i++)
    if (cmp(arr[i], obj) == 0) {
      delete arr[i];
      arr.erase(arr.begin() + static_cast<int16_t>(i));
      size--;
      return;
    }
}

}  // namespace geometry

#endif  // INCLUDE_DICTIONARY_H_

