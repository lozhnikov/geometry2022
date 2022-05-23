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
  std::vector<T> arr;
  int size;
  int current;
  int (*cmp)(T, T);

  explicit Dictionary(int (*_cmp)(T, T)) : size(0), cmp(_cmp)
  { arr.reserve(30); }
  void insert(T);
  T val();
  T prev();
  T next();
  T find(T);
  void remove(T);
};

template <typename T> void Dictionary<T>::insert(T obj) {
  for (int i = 0; i < size; i++)
    if (cmp(obj, arr[i]) < 0) {
      arr.emplace(arr.begin() + i, obj);
      size++;
      current = i;
      return;
    }
  arr.push_back(obj);
  current = size;
  size++;
}

template <typename T> T Dictionary<T>::val() { return arr[current]; }

template <typename T> T Dictionary<T>::prev() { return arr[--current]; }

template <typename T> T Dictionary<T>::next() { return arr[++current]; }

template <typename T> T Dictionary<T>::find(T obj) {
  for (int i = 0; i < size; i++)
    if (cmp(arr[i], obj) == 0) {
      current = i;
      return arr[i];
    }
  return nullptr;
}

template <typename T> void Dictionary<T>::remove(T obj) {
  for (int i = 0; i < size; i++)
    if (cmp(arr[i], obj) == 0) {
      arr.erase(arr.begin() + i);
      size--;
      return;
    }
}

}  // namespace geometry

#endif  // INCLUDE_DICTIONARY_H_
