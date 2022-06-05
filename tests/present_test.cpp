/**
 * @file tests/present_test.cpp
 * @author Ivan Yanushevich
 *
 * Реализация набора тестов для алгоритма построения выпуклой оболочки.
 */

#include <present.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "test_core.hpp"
#include "test.hpp"


static void SimpleTest1(httplib::Client* cli);
static void SimpleTest2(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

template<typename T>
static void RandomIntegerHelperTest(httplib::Client* cli, std::string type);

template<typename T>
static void RandomFloatingPointHelperTest(httplib::Client* cli,
                                          std::string type);


void TestPresent(httplib::Client* cli) {
  TestSuite suite("TestPresent");
  RUN_TEST_REMOTE(suite, cli, SimpleTest1);
  RUN_TEST_REMOTE(suite, cli, SimpleTest2);
  RUN_TEST_REMOTE(suite, cli, RandomTest);
}

/** 
 * @brief Простейший статический тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void SimpleTest1(httplib::Client* cli) {
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
    nlohmann::json input = R"(
  {
    "id": 1,
    "type": "int",
    "size": 5,
    "data": [ 3, 3, 0, 1, 7, 11, 2, 4, 5, 1 ]
  }
)"_json;

    /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
    httplib::Result res = cli->Post("/Present", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(4, output["size"]);
    REQUIRE_EQUAL(1, output["id"]);
    REQUIRE_EQUAL("int", output["type"]);

    // поменять
    REQUIRE_EQUAL(output["data"][0], 0);
    REQUIRE_EQUAL(output["data"][1], 1);
    REQUIRE_EQUAL(output["data"][2], 2);
    REQUIRE_EQUAL(output["data"][3], 4);
    REQUIRE_EQUAL(output["data"][4], 7);
    REQUIRE_EQUAL(output["data"][5], 11);
    REQUIRE_EQUAL(output["data"][6], 5);
    REQUIRE_EQUAL(output["data"][7], 1);
  }
}
static void SimpleTest2(httplib::Client* cli) {
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
    nlohmann::json input = R"(
  {
    "id": 1,
    "type": "double",
    "size": 13,
    "data": [-0.93, -0.83, 1.7, -2.3, -4.4, 3, -4.3, -3.1, 6.5, 3.9, 1, 1, -2.9, -4.1, -3.1, -10, 0.65, 4.48, 6, 2.3, 2.7, -7.3,  0.1, -10.5, -2.5, 6.7]
  }
)"_json;

    /* Делаем POST запрос по адресу нашего метода на сервере.
    Метод dump() используется для преобразования JSON обратно в строку.
    (Можно было сразу строку передать). При передаче JSON данных
    необходимо поставить тип MIME "application/json".
    */
    httplib::Result res = cli->Post("/Present", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */
    REQUIRE_EQUAL(8, output["size"]);
    REQUIRE_EQUAL(1, output["id"]);
    REQUIRE_EQUAL("double", output["type"]);
    // поменять
    REQUIRE_EQUAL(output["data"][0], -4.4);
    REQUIRE_EQUAL(output["data"][1], 3.0);
    REQUIRE_EQUAL(output["data"][2], -2.5);
    REQUIRE_EQUAL(output["data"][3], 6.7);
    REQUIRE_EQUAL(output["data"][4], 6.5);
    REQUIRE_EQUAL(output["data"][5], 3.9);
    REQUIRE_EQUAL(output["data"][6], 6.0);
    REQUIRE_EQUAL(output["data"][7], 2.3);
    REQUIRE_EQUAL(output["data"][8], 2.7);
    REQUIRE_EQUAL(output["data"][9], -7.3);
    REQUIRE_EQUAL(output["data"][10], 0.1);
    REQUIRE_EQUAL(output["data"][11], -10.5);
    REQUIRE_EQUAL(output["data"][12], -3.1);
    REQUIRE_EQUAL(output["data"][13], -10.0);
    REQUIRE_EQUAL(output["data"][14], -4.3);
    REQUIRE_EQUAL(output["data"][15], -3.1);
  }
}

/** 
 * @brief Простейший случайный тест.
 *
 * @param cli Указатель на HTTP клиент.
 */
static void RandomTest(httplib::Client* cli) {
  RandomIntegerHelperTest<int>(cli, "int");
  RandomFloatingPointHelperTest<float>(cli, "float");
  RandomFloatingPointHelperTest<double>(cli, "double");
  RandomFloatingPointHelperTest<long double>(cli, "long double");
}

/** 
 * @brief Простейший случайный тест для целых чисел.
 *
 * @param cli Указатель на HTTP клиент.
 * @param type Строковое представление типа данных сортируемых элементов.
 *
 * Функция используется для сокращения кода, необходимого для поддержки
 * различных типов данных.
 */

template<typename T>
static void RandomIntegerHelperTest(httplib::Client* cli, std::string type) {
  // Число попыток.
  const int numTries = 100;
  // Используется для инициализации генератора случайных чисел.
  std::random_device rd;
  // Генератор случайных чисел.
  std::mt19937 gen(rd());
  // Распределение для количества элементов множества.
  std::uniform_int_distribution<size_t> arraySize(10, 50);
  // Распределение для координат точек.
  std::uniform_int_distribution<int> elem(-10'000, 10'000);

  for (int it = 0; it < numTries; it++) {
    // Получаем случайное количество точек, используя функцию распределения.
    size_t size = arraySize(gen);

    nlohmann::json input;

    input["id"] = it;
    input["type"] = type;
    input["size"] = size;

    std::vector<int> data(2*size);
    for (size_t i = 0; i < size; i++) {
      // Получаем случайную координату точки, используя функцию распределения.
      data[2*i] = elem(gen);
      input["data"][2*i] = data[2*i];
      data[2*i+1] = elem(gen);
      input["data"][2*i+1] = data[2*i+1];
    }

    /* Отправляем данные на сервер POST запросом. */
    httplib::Result res = cli->Post("/Present", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */


    REQUIRE_ABOVE_EQUAL(size, output["size"]);
    REQUIRE_EQUAL(it, output["id"]);
    REQUIRE_EQUAL(type, output["type"]);
      
    size_t new_size = output["size"];
    size_t ru = 0;
    //ru - RIGHT AND UP
    for (size_t i = 1; i < new_size; i++)
      if (output["data"][2*i] > output["data"][2*ru] || 
          (output["data"][2*i] == output["data"][2*ru] && output["data"][2*i+1] > output["data"][2*ru+1]))
        ru = i;
    for (size_t i = 0; i < new_size; i++)
        REQUIRE_ABOVE_EQUAL_LEXIGRAFICAL(output["data"][2*ru], output["data"][2*ru+1], data[2*i], data[2*i+1]);
    
  }
}


template<typename T>
static void RandomFloatingPointHelperTest(httplib::Client* cli,
                                          std::string type) {
  // Число попыток.
  const int numTries = 100;
  // Относительная точность сравнения.
  const T eps = std::numeric_limits<T>::epsilon() * T(1e4);
  // Используется для инициализации генератора случайных чисел.
  std::random_device rd;
  // Генератор случайных чисел.
  std::mt19937 gen(rd());
  // Распределение для количества элементов множества.
  std::uniform_int_distribution<size_t> arraySize(10, 50);
  // Распределение для координат точек.
  std::uniform_real_distribution<T> elem(T(-10'000), T(10'000));

  for (int it = 0; it < numTries; it++) {
    // Получаем случайное количество точек, используя функцию распределения.
    size_t size = arraySize(gen);
    nlohmann::json input;

    input["id"] = it;
    input["type"] = type;
    input["size"] = size;

    std::vector<T> data(2*size);

    for (size_t i = 0; i < 2*size; i++) {
      // Получаем случайную точку, используя функцию распределения.
      data[i] = elem(gen);

      // Записываем элемент в JSON.
      input["data"][i] = data[i];
    }

    /* Отправляем данные на сервер POST запросом. */
    httplib::Result res = cli->Post("/Present", input.dump(),
        "application/json");

    /* Используем метод parse() для преобразования строки ответа сервера
    (res->body) в объект JSON. */
    nlohmann::json output = nlohmann::json::parse(res->body);

    /* Проверка результатов сортировки. */


    REQUIRE_ABOVE_EQUAL(size, output["size"]);
    REQUIRE_EQUAL(it, output["id"]);
    REQUIRE_EQUAL(type, output["type"]);
    size_t new_size = output["size"];
    std::vector<T> new_data(2*new_size);
    for (size_t i = 0; i < 2*new_size; i++)
        new_data[i] = output["data"][i];
    size_t new_ru = 0;
    //ru - RIGHT AND UP
    for (size_t i = 1; i < new_size; i++)
      if ((new_data[2*i] - new_data[2*new_ru] > eps) || 
          ((std::fabs(new_data[2*i] - new_data[2*new_ru]) <= eps)  && (new_data[2*i+1] - new_data[2*new_ru+1]) > eps))
        new_ru = i;
    size_t ru = 0;
    for (size_t i = 1; i < size; i++)
      if ((data[2*i] - data[2*ru] > eps) || 
          ((std::fabs(data[2*i] - data[2*ru]) <= eps)  && (data[2*i+1] - data[2*ru+1]) > eps))
        ru = i;
    REQUIRE_CLOSE(new_data[2*new_ru], data[2*ru], eps);
    REQUIRE_CLOSE(new_data[2*new_ru+1], data[2*ru+1], eps);
  }
}