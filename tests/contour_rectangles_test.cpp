/**
 * @file tests/contour_rectangles_test.cpp
 * @author Boris Shevchenko
 *
 * Реализация набора тестов для алгоритма построения контура объединения
 * прямоугольников.
 */

#include <httplib.h>
#include <cmath>
#include <limits>
#include <contour_rectangles.hpp>
#include <nlohmann/json.hpp>
#include "test.hpp"
#include "test_core.hpp"

#define NUM_TRIES_FOR_RANDOM_TEST 100

static void IntSimpleTest(httplib::Client *cli);
static void FloatSimpleTest(httplib::Client *cli);
static void RandomTest(httplib::Client *cli);

template <typename T>
static void RandomIntegerHelperTest(httplib::Client *cli, std::string type);
template <typename T>
static void RandomFloatingPointHelperTest(httplib::Client *cli,
                                          std::string type);


void TestContourRectangles(httplib::Client *cli) {
  TestSuite suite("TestContourRectangles");

  RUN_TEST_REMOTE(suite, cli, IntSimpleTest);
  RUN_TEST_REMOTE(suite, cli, FloatSimpleTest);
  RUN_TEST_REMOTE(suite, cli, RandomTest);
}

/** 
 * @brief Простейший статический тест с целыми числами.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void IntSimpleTest(httplib::Client *cli) {
  nlohmann::json input = R"(
  {
  "id"  : 1,
  "type": "int",
  "size": 3,
  "data": [ [[0,0],[5,5]], [[-1,1],[3,4]], [[2,2],[6,3]] ]
}
)"_json;

  httplib::Result res =
      cli->Post("/ContourRectangles", input.dump(), "application/json");

  nlohmann::json output = nlohmann::json::parse(res->body);

  REQUIRE_EQUAL(1, output["id"]);
  REQUIRE_EQUAL(12, output["size"]);
  REQUIRE_EQUAL("int", output["type"]);

  REQUIRE_EQUAL(to_string(output["data"][0]), "[[-1,1],[-1,4]]");
  REQUIRE_EQUAL(to_string(output["data"][1]), "[[0,0],[0,1]]");
  REQUIRE_EQUAL(to_string(output["data"][2]), "[[-1,1],[0,1]]");
  REQUIRE_EQUAL(to_string(output["data"][3]), "[[-1,4],[0,4]]");
  REQUIRE_EQUAL(to_string(output["data"][4]), "[[0,4],[0,5]]");
  REQUIRE_EQUAL(to_string(output["data"][5]), "[[0,0],[5,0]]");
  REQUIRE_EQUAL(to_string(output["data"][6]), "[[0,5],[5,5]]");
  REQUIRE_EQUAL(to_string(output["data"][7]), "[[5,0],[5,2]]");
  REQUIRE_EQUAL(to_string(output["data"][8]), "[[5,3],[5,5]]");
  REQUIRE_EQUAL(to_string(output["data"][9]), "[[5,2],[6,2]]");
  REQUIRE_EQUAL(to_string(output["data"][10]), "[[5,3],[6,3]]");
  REQUIRE_EQUAL(to_string(output["data"][11]), "[[6,2],[6,3]]");
}

/** 
 * @brief Простейший статический тест с числами с плавающей точкой.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void FloatSimpleTest(httplib::Client *cli) {
  nlohmann::json input = R"(
  {
  "id"  : 2,
  "type": "float",
  "size": 2,
  "data": [ [[2.0,1.4],[4.0,3.7]], [[1.11,1.11],[3.27,2.46]] ]
}
)"_json;

  httplib::Result res =
      cli->Post("/ContourRectangles", input.dump(), "application/json");

  nlohmann::json output = nlohmann::json::parse(res->body);

  REQUIRE_EQUAL(2, output["id"]);
  REQUIRE_EQUAL(8, output["size"]);
  REQUIRE_EQUAL("float", output["type"]);

  const float eps = std::numeric_limits<float>::epsilon() * 1e4;
  float x1, x2, y1, y2;
  for (size_t i = 0; i < 8; i++) {
    x1 = output["data"][i][0][0]; x2 = output["data"][i][1][0];
    y1 = output["data"][i][0][1]; y2 = output["data"][i][1][1];
    if (std::fabs(x1 - x2) < eps) {
      continue;
    } else if (std::fabs(y1 - y2) < eps) {
      continue;
    } else {
      REQUIRE_EQUAL(to_string(output["data"][i]), "Не параллельно осям");
    }
  }
}

/** 
 * @brief Простейший случайный тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void RandomTest(httplib::Client *cli) {
  RandomIntegerHelperTest<int>(cli, "int");

  RandomFloatingPointHelperTest<float>(cli, "float");
  RandomFloatingPointHelperTest<double>(cli, "double");
  RandomFloatingPointHelperTest<long double>(cli, "long double");
}

/** 
 * @brief Простейший случайный тест для целых чисел.
 *
 * @tparam T Тип данных координат точек.
 *
 * @param cli Указатель на HTTP клиент.
 * @param type Строковое представление типа данных координат точек.
 *
 * Функция используется для сокращения кода, необходимого для поддержки
 * различных типов данных.
 */
template <typename T>
static void RandomIntegerHelperTest(httplib::Client *cli, std::string type) {
  // Число попыток.
  const int numTries = NUM_TRIES_FOR_RANDOM_TEST;
  // Используется для инициализации генератора случайных чисел.
  std::random_device rd;
  // Генератор случайных чисел.
  std::mt19937 gen(rd());
  // Распределение для количества элементов массива.
  std::uniform_int_distribution<size_t> numRect(10, 50);
  // Распределение для элементов массива.
  std::uniform_int_distribution<T> elem(-1000, 1000);

  for (int it = 0; it < numTries; it++) {
    // Получаем случайный размер массива, используя функцию распределения.
    size_t size = numRect(gen);

    nlohmann::json input;

    input["id"] = it;
    input["type"] = type;
    input["size"] = size;
    T x1, y1, x2, y2;
    for (size_t i = 0; i < size; i++) {
      for (; 1;) {
        x1 = elem(gen);
        x2 = elem(gen);
        if (x1 != x2)
          break;
      }
      for (; 1;) {
        y1 = elem(gen);
        y2 = elem(gen);
        if (y1 != y2)
          break;
      }
      if (x1 < x2) {
        input["data"][i][0][0] = x1;
        input["data"][i][1][0] = x2;
      } else {
        input["data"][i][0][0] = x2;
        input["data"][i][1][0] = x1;
      }
      if (y1 < y2) {
        input["data"][i][0][1] = y1;
        input["data"][i][1][1] = y2;
      } else {
        input["data"][i][0][1] = y2;
        input["data"][i][1][1] = y1;
      }
    }
    // std::cout << input.dump() << '\n';
    // Отправляем данные на сервер POST запросом.
    httplib::Result res =
        cli->Post("/ContourRectangles", input.dump(), "application/json");

    nlohmann::json output = nlohmann::json::parse(res->body);

    REQUIRE_EQUAL(it, output["id"]);
    REQUIRE_EQUAL(type, output["type"]);

    for (size_t i = 0; i < output["size"]; i++) {
      if (output["data"][i][0][0] == output["data"][i][1][0]) {
        continue;
      } else if (output["data"][i][0][1] == output["data"][i][1][1]) {
        continue;
      } else {
        REQUIRE_EQUAL(to_string(output["data"][i]), "Не параллельно осям");
      }
    }
  }
}

/** 
 * @brief Простейший случайный тест для чисел с плавающей точкой.
 *
 * @tparam T Тип данных координат точек.
 *
 * @param cli Указатель на HTTP клиент.
 * @param type Строковое представление типа данных координат точек.
 *
 * Функция используется для сокращения кода, необходимого для поддержки
 * различных типов данных.
 */
template <typename T>
static void RandomFloatingPointHelperTest(httplib::Client *cli,
                                          std::string type) {
  // Число попыток.
  const int numTries = NUM_TRIES_FOR_RANDOM_TEST;
  // Относительная точность сравнения.
  const T eps = std::numeric_limits<T>::epsilon() * T(1e4);
  // Используется для инициализации генератора случайных чисел.
  std::random_device rd;
  // Генератор случайных чисел.
  std::mt19937 gen(rd());
  // Распределение для количества элементов массива.
  std::uniform_int_distribution<size_t> arraySize(10, 50);
  // Распределение для элементов массива.
  std::uniform_real_distribution<T> elem(T(-100), T(100));

  for (int it = 0; it < numTries; it++) {
    // Получаем случайный размер массива, используя функцию распределения.
    size_t size = arraySize(gen);

    nlohmann::json input;

    input["id"] = it;
    input["type"] = type;
    input["size"] = size;

    T x1, y1, x2, y2;
    for (size_t i = 0; i < size; i++) {
      for (; 1;) {
        x1 = elem(gen);
        x2 = elem(gen);
        if (std::fabs(x1 - x2) > eps)
          break;
      }
      for (; 1;) {
        y1 = elem(gen);
        y2 = elem(gen);
        if (std::fabs(y1 - y2) > eps)
          break;
      }
      if (x1 < x2) {
        input["data"][i][0][0] = x1;
        input["data"][i][1][0] = x2;
      } else {
        input["data"][i][0][0] = x2;
        input["data"][i][1][0] = x1;
      }
      if (y1 < y2) {
        input["data"][i][0][1] = y1;
        input["data"][i][1][1] = y2;
      } else {
        input["data"][i][0][1] = y2;
        input["data"][i][1][1] = y1;
      }
    }

    httplib::Result res =
        cli->Post("/ContourRectangles", input.dump(), "application/json");

    nlohmann::json output = nlohmann::json::parse(res->body);

    REQUIRE_EQUAL(it, output["id"]);
    REQUIRE_EQUAL(type, output["type"]);

    for (size_t i = 0; i < output["size"]; i++) {
      x1 = output["data"][i][0][0]; x2 = output["data"][i][1][0];
      y1 = output["data"][i][0][1]; y2 = output["data"][i][1][1];
      if (std::fabs(x1 - x2) < eps) {
        continue;
      } else if (std::fabs(y1 - y2) < eps) {
        continue;
      } else {
        REQUIRE_EQUAL(to_string(output["data"][i]), "Не параллельно осям");
      }
    }
  }
}
