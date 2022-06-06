/**
 * @file tests/graham_scan_test.cpp
 * @author Eldarushkaa
 *
 * Реализация набора тестов для алгоритма сортировки вставками.
 */

#include <ostream>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <graham_scan.hpp>
#include "test_core.hpp"
#include "test.hpp"

template<typename T>
int geometry::polarCmp(Point<T> p, Point<T> q);

static void SimpleTestGrahamScan1(httplib::Client* cli);
static void SimpleTestGrahamScan2(httplib::Client* cli);
static void SimpleTestGrahamScan3(httplib::Client* cli);
static void RandomTestGrahamScan(httplib::Client* cli);

// template<typename T>
// static void RandomIntegerHelperTest(httplib::Client* cli, std::string type);

template<typename T>
static void RandomFloatingPointHelperTest(httplib::Client* cli,
                                          std::string type);

void TestGrahamScan(httplib::Client* cli) {
  TestSuite suite("TestGrahamScan");

  // SimpleTestGrahamScan1(cli);

  RUN_TEST_REMOTE(suite, cli, SimpleTestGrahamScan1);
  RUN_TEST_REMOTE(suite, cli, SimpleTestGrahamScan2);
  RUN_TEST_REMOTE(suite, cli, SimpleTestGrahamScan3);
  RUN_TEST_REMOTE(suite, cli, RandomTestGrahamScan);
  // std::cout<<"Test has been started"<<std::endl;
}

/** 
 * @brief Простейший статический тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void SimpleTestGrahamScan1(httplib::Client* cli) {
  {
    /*
     Библиотека nlohmann json позволяет преобразовать
     строку в объект nlohmann::json не только при помощи
     функции nlohmann::json::parse(), но и при помощи
     специального литерала _json. Если его поставить после строки
     в кавычках, то она конвертируется в json объект.

     R"(
     )" Так записывается строка, содержащая символы перевода строки
     в C++. Всё, что между скобками это символы строки. Перводы строк
     можно ставить просто как перевод строки в текстовом редактора
     (а не через \n).     
    */
    nlohmann::json input;
//       = R"(
//   {
//     "id": 1,
//     "type": "long double",
//     "size": 5,
//     "data": [ [3, 2, 4, 5, 1], [1, 2, 3, 4, 5] ]
//   }
// )"_json;
    input["id"] = 1;
    input["type"] = "long double";
    input["size"] = 5;
    input["data"] = { {-30, 10, 0, 20, 50}, {0, 20, 30, 10, 50} };
    /*
    ***********     ***********
    *********O*     *********3*
    ****O******     ****4******
    *****O*****     ***********
    ******O****     ******2****
    *O*********     *1*********
    ***********     ***********
    */

    /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
    httplib::Result res = cli->Post("/GrahamScan", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(4, output["size"]);
    REQUIRE_EQUAL(1, output["id"]);
    REQUIRE_EQUAL("long double", output["type"]);

    REQUIRE_EQUAL(output["data"][0][0], -30);
    REQUIRE_EQUAL(output["data"][1][0], 0);
    REQUIRE_EQUAL(output["data"][0][1], 20);
    REQUIRE_EQUAL(output["data"][1][1], 10);
    REQUIRE_EQUAL(output["data"][0][2], 50);
    REQUIRE_EQUAL(output["data"][1][2], 50);
    REQUIRE_EQUAL(output["data"][0][3], 0);
    REQUIRE_EQUAL(output["data"][1][3], 30);

    // std::cout<<"SimpleTest done"<<std::endl;
    // std::cout<<input["data"]<<std::endl;
    // std::cout<<output["data"]<<std::endl;
  }
}

static void SimpleTestGrahamScan3(httplib::Client* cli) {
  {
    /*
     Библиотека nlohmann json позволяет преобразовать
     строку в объект nlohmann::json не только при помощи
     функции nlohmann::json::parse(), но и при помощи
     специального литерала _json. Если его поставить после строки
     в кавычках, то она конвертируется в json объект.

     R"(
     )" Так записывается строка, содержащая символы перевода строки
     в C++. Всё, что между скобками это символы строки. Перводы строк
     можно ставить просто как перевод строки в текстовом редактора
     (а не через \n).     
    */
    nlohmann::json input;

    input["id"] = 3;
    input["type"] = "long double";
    input["size"] = 25;
    for (size_t i = 0; i < 5; i++) {
      for (size_t j = 0; j < 5; j++) {
        input["data"][0][i * 5 + j] = i;
        input["data"][1][i * 5 + j] = j;
      }
    }
    /*
    *******     *******
    *OOOOO*     *3***2*
    *OOOOO*     *******
    *OOOOO*     *******
    *OOOOO*     *******
    *OOOOO*     *4***1*
    *******     *******
    */

    /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
    httplib::Result res = cli->Post("/GrahamScan", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(4, output["size"]);
    REQUIRE_EQUAL(3, output["id"]);
    REQUIRE_EQUAL("long double", output["type"]);

    REQUIRE_EQUAL(output["data"][0][0], 4);
    REQUIRE_EQUAL(output["data"][1][0], 0);
    REQUIRE_EQUAL(output["data"][0][1], 4);
    REQUIRE_EQUAL(output["data"][1][1], 4);
    REQUIRE_EQUAL(output["data"][0][2], 0);
    REQUIRE_EQUAL(output["data"][1][2], 4);
    REQUIRE_EQUAL(output["data"][0][3], 0);
    REQUIRE_EQUAL(output["data"][1][3], 0);

    // std::cout<<input["data"]<<std::endl;
    // std::cout<<output["data"]<<std::endl;
  }
}

static void SimpleTestGrahamScan2(httplib::Client* cli) {
  {
    /*
     Библиотека nlohmann json позволяет преобразовать
     строку в объект nlohmann::json не только при помощи
     функции nlohmann::json::parse(), но и при помощи
     специального литерала _json. Если его поставить после строки
     в кавычках, то она конвертируется в json объект.

     R"(
     )" Так записывается строка, содержащая символы перевода строки
     в C++. Всё, что между скобками это символы строки. Перводы строк
     можно ставить просто как перевод строки в текстовом редактора
     (а не через \n).     
    */
    nlohmann::json input;
//       = R"(
//   {
//     "id": 1,
//     "type": "long double",
//     "size": 5,
//     "data": [ [3, 2, 4, 5, 1], [1, 2, 3, 4, 5] ]
//   }
// )"_json;
    input["id"] = 2;
    input["type"] = "long double";
    input["size"] = 9;
    input["data"] = { {-10, 30, 10, -50, -10, -20, -40, 10, -30}, 
                     {-20, -50, 10, -50, -60, -10, -10, -30, -30} };
    /*
    ***********     ***********
    *******O***     *******3***
    ***********     ***********
    **O*O******     **4********
    *****O*****     ***********
    ***O***O***     ***********
    ***********     ***********
    *O*******O*     *5*******2*
    *****O*****     *****1*****
    ***********     ***********
    */

    /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
    httplib::Result res = cli->Post("/GrahamScan", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(5, output["size"]);
    REQUIRE_EQUAL(2, output["id"]);
    REQUIRE_EQUAL("long double", output["type"]);

    REQUIRE_EQUAL(output["data"][0][0], -10);
    REQUIRE_EQUAL(output["data"][1][0], -60);
    REQUIRE_EQUAL(output["data"][0][1], 30);
    REQUIRE_EQUAL(output["data"][1][1], -50);
    REQUIRE_EQUAL(output["data"][0][2], 10);
    REQUIRE_EQUAL(output["data"][1][2], 10);
    REQUIRE_EQUAL(output["data"][0][3], -40);
    REQUIRE_EQUAL(output["data"][1][3], -10);
    REQUIRE_EQUAL(output["data"][0][4], -50);
    REQUIRE_EQUAL(output["data"][1][4], -50);

    // std::cout<<"SimpleTest done"<<std::endl;
    // std::cout<<input["data"]<<std::endl;
    // std::cout<<output["data"]<<std::endl;
  }
}

/** 
 * @brief Простейший случайный тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void RandomTestGrahamScan(httplib::Client* cli) {
//   RandomIntegerHelperTest<int>(cli, "int");

  RandomFloatingPointHelperTest<float>(cli, "float");
  RandomFloatingPointHelperTest<double>(cli, "double");
  RandomFloatingPointHelperTest<long double>(cli, "long double");
}
// /** 
//  * @brief Простейший случайный тест для целых чисел.
//  *
//  * @tparam T Тип данных сортируемых элементов.
//  *
//  * @param cli Указатель на HTTP клиент.
//  * @param type Строковое представление типа данных сортируемых элементов.
//  *
//  * Функция используется для сокращения кода, необходимого для поддержки
//  * различных типов данных.
//  */
// template<typename T>
// static void RandomIntegerHelperTest(httplib::Client* cli, std::string type) {
//   // Число попыток.
//   const int numTries = 100;
//   // Используется для инициализации генератора случайных чисел.
//   std::random_device rd;
//   // Генератор случайных чисел.
//   std::mt19937 gen(rd());
//   // Распределение для количества элементов массива.
//   std::uniform_int_distribution<size_t> arraySize(10, 50);
//   // Распределение для элементов массива.
//   std::uniform_int_distribution<T> elem(-10'000, 10'000);

//   for (int it = 0; it < numTries; it++) {
//     // Получаем случайный размер массива, используя функцию распределения.
//     size_t size = arraySize(gen);

//     nlohmann::json input;

//     input["id"] = it;
//     input["type"] = type;
//     input["size"] = size;

//     std::vector<T> data(size);

//     for (size_t i = 0; i < size; i++) {
//       // Получаем случайный элемент массива, используя функцию распределения.
//       data[i] = elem(gen);

//       // Записываем элемент в JSON.
//       input["data"][i] = data[i];
//     }

//     /* Отправляем данные на сервер POST запросом. */
//     httplib::Result res = cli->Post("/InsertionSort", input.dump(),
//         "application/json");

//     /* Используем метод parse() для преобразования строки ответа сервера
//     (res->body) в объект JSON. */
//     nlohmann::json output = nlohmann::json::parse(res->body);

//     /* Проверка результатов сортировки. */

//     std::sort(data.begin(), data.end());

//     REQUIRE_EQUAL(size, output["size"]);
//     REQUIRE_EQUAL(it, output["id"]);
//     REQUIRE_EQUAL(type, output["type"]);

//     for (size_t i = 0; i < size; i++) {
//       REQUIRE_EQUAL(data[i], output["data"][i]);
//     }
//   }
// }

/** 
 * @brief Простейший случайный тест для чисел с плавающей точкой.
 *
 * @tparam T Тип данных сортируемых элементов.
 *
 * @param cli Указатель на HTTP клиент.
 * @param type Строковое представление типа данных сортируемых элементов.
 *
 * Функция используется для сокращения кода, необходимого для поддержки
 * различных типов данных.
 */

template<typename T>
static void RandomFloatingPointHelperTest(httplib::Client* cli,
                                          std::string type) {
  // Число попыток.
  const int numTries = 100;

  // Используется для инициализации генератора случайных чисел.
  std::random_device rd;
  // Генератор случайных чисел.
  std::mt19937 gen(rd());
  // Распределение для количества элементов массива.
  std::uniform_int_distribution<size_t> arraySize(10, 50);
  // Распределение для элементов массива.
  std::uniform_real_distribution<T> elem(T(-10'000), T(10'000));

  for (int it = 0; it < numTries; it++) {
    // Получаем случайный размер массива, используя функцию распределения.
    size_t size = arraySize(gen);

    nlohmann::json input;

    input["id"] = it;
    input["type"] = type;
    input["size"] = size;

    for (size_t i = 0; i < size; i++) {
      // Получаем случайный элемент массива, используя функцию распределения.
      // Записываем элемент в JSON.
      input["data"][0][i] = elem(gen);
      input["data"][1][i] = elem(gen);
    }

    /* Отправляем данные на сервер POST запросом. */
    httplib::Result res = cli->Post("/GrahamScan", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */

    size = output.at("size");

    std::list<geometry::Point<T>> data;
    for (size_t i = 0; i < size; i++) {
      data.push_back(geometry::Point<T>(output.at("data").at(0).at(i),
                                          output.at("data").at(1).at(i)));
    }
    data.push_back(*data.begin());

    auto it1 = data.begin();
    auto it2 = it1;
    it2++;
    auto it3 = it2;
    it3++;
    for (; it3 != data.end(); it1++, it2++, it3++) {
      if (!geometry::polarCmp<T>(*it2 - *it1, *it3 - *it2)) {
        REQUIRE_EQUAL(1, 0);
      }
    }
  }
}
