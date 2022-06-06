/**
 * @file methods/graham_sort_method.cpp
 * @author Eldarushkaa
 *
 * Файл содержит функцию, которая вызывает алгоритм построения 
 * выпуклой оболочки методом обхода Грэхема.
 * Функция принимает и возвращает данные в JSON формате.
 */

#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "graham_scan.hpp"

namespace geometry {

template<typename T>
int GrahamScanMethodHelper(const nlohmann::json& input, nlohmann::json* output, std::string type);

int GrahamScanMethod(const nlohmann::json& input, nlohmann::json* output) {
  std::string type = input.at("type");
  if (type == "int") {
    return GrahamScanMethodHelper<int>(input, output, type);
  }
  else if (type == "float") {
    return GrahamScanMethodHelper<float>(input, output, type);
  }
  else if (type == "double") {
    return GrahamScanMethodHelper<double>(input, output, type);
  }
  else if (type == "long double") {
    return GrahamScanMethodHelper<long double>(input, output, type);
  }

    return -1;
}

template<typename T>
int GrahamScanMethodHelper(const nlohmann::json& input, 
                           nlohmann::json* output, std::string type) {
  (*output)["id"] = input.at("id");

  size_t size = input.at("size");

  std::list<Point<T>> data;
  for (size_t i = 0; i < size; i++) {
    data.push_back(Point<T>(input.at("data").at(0).at(i), input.at("data").at(1).at(i)));
  }

  Polygon<T> P = GrahamScan<T>(data, size);

  size = P.Size();
  data = P.Vertices();
  (*output)["size"] = size;
  (*output)["type"] = type;

  size_t i = 0;
  for (Point<T> n : data) {
    (*output)["data"][0][i] = n.X();
    (*output)["data"][1][i] = n.Y();
    i++;
  }

  return -1;
}

// template<typename T>
// static int InsertionSortMethodHelper(const nlohmann::json& input,
//                                      nlohmann::json* output,
//                                      std::string type) {
//   (*output)["id"] = input.at("id");

//   size_t size = input.at("size");

//   T* data = new T[size];

//   for (size_t i = 0; i < size; i++) {
//     /* Для словарей используется индекс в виде строки,
//     а для массивов просто целое число типа size_t. */
//     data[i] = input.at("data").at(i);
//   }

//   /* Здесь вызывается сам алгоритм сортировки вставками. */
//   InsertionSort(data, size, [](T left, T right) {
//     if (left < right)
//       return -1;
//     else if (left > right)
//       return 1;

//     return 0;
//   });

//   /* Сохраняем в ответе результат работы алгоритма. */
//   (*output)["size"] = size;
//   (*output)["type"] = type;

//   for (size_t i = 0; i < size; i++)
//     (*output)["data"][i] = data[i];

//   delete[] data;

//   return 0;
// }

}  // namespace geometry
