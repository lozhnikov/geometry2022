/**
 * @file methods/present_method.cpp
 * @author Ivan Yanushevich
 *
 * Файл содержит функцию, которая вызывает алгоритм построения выпуклой оболочки.
 * Функция принимает и возвращает данные в JSON формате.
 */
#include <list>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "present.hpp"
#include "point.hpp"
#include "polygon.hpp"

namespace geometry {

template<typename T>
static int PresentMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     std::string type);

int PresentMethod(const nlohmann::json& input, nlohmann::json* output) {
  /*
  С классом nlohmann::json можно работать как со словарём.
  Метод at() в отличие оператора [] не меняет объект, поэтому
  этот метод можно использовать с константными объектами.
  */
  std::string type = input.at("type");

  /* Пока реализована только поддержка целых чисел и чисел типа double.*/
  if (type == "int") {
    return PresentMethodHelper<int>(input, output, type);
  } else if (type == "float") {
    return PresentMethodHelper<float>(input, output, type);
  } else if (type == "double") {
    return PresentMethodHelper<double>(input, output, type);
  } else if (type == "long double") {
    return PresentMethodHelper<long double>(input, output, type);
  }

  return -1;
}

/**
 * @brief Метод построения выпуклой оболочки.
 *
 * @param input Входные данные в формате JSON.
 * @param output Выходные данные в формате JSON.
 * @param type Строковое представление типа данных мортируемых элементов.
 * @return Функция возвращает 0 в случае успеха и отрицательное число
 * если входные данные заданы некорректно.
 *
 * Функция запускает алгоритм сортировки вставками, используя входные данные
 * в JSON формате. Результат также выдаётся в JSON формате. Функция
 * используется для сокращения кода, необходимого для поддержки различных типов данных.
 */
template<typename T>
static int PresentMethodHelper(const nlohmann::json& input,
                                     nlohmann::json* output,
                                     std::string type) {
  (*output)["id"] = input.at("id");

  size_t size = input.at("size");

  Point<T>* data = new Point<T>[2*size];

  for (size_t i = 0; i < size; i++) {
    Point<T> current = Point<T>
        (input.at("data").at(2*i), input.at("data").at(2*i+1));
    data[i] = current;
  }
  /* Здесь вызывается сам алгоритм построения выпуклой оболочки. */
  Polygon<T>* res = Present(data, static_cast <int>(size));
  std::list<Point<T>>& result = res->Vertices();

  /* Сохраняем в ответе результат работы алгоритма. */
  size = res->Size();
  (*output)["size"] = size;
  (*output)["type"] = type;
  size_t i = 0;
  for (auto current = result.begin(); current != result.end(); current++) {
    Point<T> current_point = *current;
    (*output)["data"][2*i] = current_point.X();
    (*output)["data"][2*i+1] = current_point.Y();
    i++;
  }

  delete[] data;

  return 0;
}
}  // namespace geometry
