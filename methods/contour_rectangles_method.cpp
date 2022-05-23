/**
 * @file methods/contour_rectangles_method.cpp
 * @author Boris Shevchenko
 *
 * Файл содержит функцию, которая вызывает алгоритм нахождения контура
 * объединения прямоугольников. Функция принимает и возвращает данные в JSON
 * формате.
 */

#include <string>
#include <nlohmann/json.hpp>
#include "contour_rectangles.hpp"

namespace geometry {

template <typename T>
static int ContourRectanglesMethodHelper(const nlohmann::json &input,
                                         nlohmann::json *output,
                                         std::string type);

int ContourRectanglesMethod(const nlohmann::json &input,
                            nlohmann::json *output) {
  std::string type = input.at("type");

  if (type == "int") {
    return ContourRectanglesMethodHelper<int>(input, output, type);
  } else if (type == "float") {
    return ContourRectanglesMethodHelper<float>(input, output, type);
  } else if (type == "double") {
    return ContourRectanglesMethodHelper<double>(input, output, type);
  } else if (type == "long double") {
    return ContourRectanglesMethodHelper<long double>(input, output, type);
  }

  return -1;
}

template <typename T>
static int ContourRectanglesMethodHelper(const nlohmann::json &input,
                                         nlohmann::json *output,
                                         std::string type) {
  (*output)["id"] = input.at("id");

  size_t size = input.at("size");
  Rectangle<T> *data = new Rectangle<T>[size];

  for (size_t i = 0; i < size; i++) {
    data[i] = Rectangle<T>(Point<T>(input.at("data").at(i).at(0).at(0),
                                    input.at("data").at(i).at(0).at(1)),
                           Point<T>(input.at("data").at(i).at(1).at(0),
                                    input.at("data").at(i).at(1).at(1)),
                           i + 1);
  }

  std::list<Edge<T> *> *res = ContourRectangles<T>(data, size);
  // std::cout << input.dump() << '\n';

  (*output)["size"] = res->size();
  (*output)["type"] = type;
  size_t i = 0;
  for (auto iter = res->begin(); iter != res->end(); iter++, i++) {
    (*output)["data"][i][0][0] = (*iter)->Origin().X();
    (*output)["data"][i][0][1] = (*iter)->Origin().Y();
    (*output)["data"][i][1][0] = (*iter)->Destination().X();
    (*output)["data"][i][1][1] = (*iter)->Destination().Y();
  }
  // std::cout << output->dump() << '\n'; //Вывести результат в виде json

  delete[] data;

  return 0;
}

}  // namespace geometry
