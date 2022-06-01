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

/**
 * @brief Класс словаря
 *
 * @tparam T тип хранящихся данных
 */
template <typename T> class Dictionary {
 public:
  //! Размер
  int size;
  //! Номер текущего элемента
  int current;
  //! Компоратор
  int (*cmp)(T, T);
  //! Вектор хранящихся объектов
  std::vector<T> arr;

  /**
   * @brief Специальный конструктор для словаря.
   *
   * @param _cmp Компоратор
   * @param _arr Вектор объектов для хранения
   *
   * Создать словарь из элементов из _arr.
   */
  Dictionary(int (*_cmp)(T, T), std::vector<T> _arr) :
  size(0), current(0), cmp(_cmp), arr(_arr)
  { arr.clear(); arr.reserve(30); }
  /**
   * @brief Вставляет элемент в словарь
   *
   * @param 1 Вставляемый элемент
   */
  void Insert(T);
  /**
   * @brief Возвращает текущий элемент
   * @return Текущий элемент
   */
  T Val();
  /**
   * @brief Возвращает предыдущий элемент
   * @return Предыдущий элемент
   */
  T Prev();
  /**
   * @brief Возвращает следующий элемент
   * @return Следующий элемент
   */
  T Next();
  /**
   * @brief Ищет элемент в словаре
   * @param 1 Искомый элемент
   *
   * @return Искомый объект если нашел, иначе нулевой указатель
   */
  T Find(T);
  /**
   * @brief Удаляет элемент из словаря
   * @param 1 Удаляемый элемент
   */
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

