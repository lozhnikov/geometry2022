#include "test.hpp"
#include "test_core.hpp"

#include <nlohmann/json.hpp>
#include <point.hpp>
#include <random>

using namespace std;
using namespace geometry;

nlohmann::json SerializePointVector(const vector< Point<double> > & v) {
  auto array = nlohmann::json::array(); //массив формата json
  for (auto & p : v) {
    nlohmann::json x = p.X();
    nlohmann::json y = p.Y();
    array.push_back(x);
    array.push_back(y);
  }
  return array;
}
vector<int> DeserializeIndexVector(const nlohmann::json & j) {
  vector<int> result;
    for (auto & coord : j) {
        int int_index = coord.get<int>();
        result.push_back(int_index);
    }
    return result;
}

std::vector<int> Triangulate(httplib::Client & client, const std::vector<Point<double>> & v)
{
    nlohmann::json input = SerializePointVector(v);
    std::string json = input.dump();
    httplib::Result result = client.Post("/triangulate", json, "text/json");
    // result - указатель на "настоящий" результат. 0 если сбой в обращении к серверу
    if (!result) return std::vector<int>();
    nlohmann::json result_json = nlohmann::json::parse(result->body);
    return DeserializeIndexVector(result_json);
}

void VerifyTriangulate(const vector<Point<double>> & input, const vector<int> & indicies, double area)
{
    // удостоверяемся, что, например, число индексов корректно, то есть 3k
    REQUIRE(indicies.size() % 3 == 0);
    
    unsigned long num_edges = 0;
    for (unsigned long i = 0; i < input.size() - 1; i++) {
        for (unsigned long j = i + 1; j < input.size(); j++) {
            // пара (i, j) пробегает здесь все теоретически возможные рёбра
            bool edge_present = false;
            for (unsigned long k = 0; k < indicies.size(); k += 3) {
                if (static_cast<long unsigned>(indicies[k]) == i && static_cast<long unsigned>(indicies[k + 1]) == j) edge_present = true;
                else if (static_cast<long unsigned>(indicies[k]) == j && static_cast<long unsigned>(indicies[k + 1]) == i) edge_present = true;
                else if (static_cast<long unsigned>(indicies[k]) == i && static_cast<long unsigned>(indicies[k + 2]) == j) edge_present = true;
                else if (static_cast<long unsigned>(indicies[k]) == j && static_cast<long unsigned>(indicies[k + 2]) == i) edge_present = true;
                else if (static_cast<long unsigned>(indicies[k + 1]) == i && static_cast<long unsigned>(indicies[k + 2]) == j) edge_present = true;
                else if (static_cast<long unsigned>(indicies[k + 1]) == j && static_cast<long unsigned>(indicies[k + 2]) == i) edge_present = true;
                if (edge_present) break;
            }
            if (edge_present) num_edges++;
        }
    }
    // проверяем формулу Эйлера
    REQUIRE(indicies.size() / 3 + 1 + input.size() - num_edges == 2);
    
    double sum = 0.0;
    for (long unsigned i = 2; i < indicies.size(); i += 3) {
        std::cout<<indicies[i]<<std::endl;
        auto & p1 = input[static_cast<long unsigned>(indicies[i - 2])];
        auto & p2 = input[static_cast<long unsigned>(indicies[i - 1])];
        auto & p3 = input[static_cast<long unsigned>(indicies[i])];
        auto p12 = p1 - p2;
        auto p31 = p3 - p1;
        auto p23 = p2 - p3;
        auto p = (p12.Length() + p23.Length() + p31.Length()) / 2.0;
        auto S = sqrt(p * (p - p12.Length()) * (p - p23.Length()) * (p - p31.Length()));
        sum += S;
    }
    // проверяем площадь
    REQUIRE_CLOSE(sum, area, 1.0E-5);
}

// client нужен для отправвки тестовых запросов серверу
void TestTriangulate(httplib::Client & client)
{
    // suite для вывода красивых отчётов
    TestSuite suite("TestTriangulate");

    //площадь = 1,5
    vector<Point<double>> input;
    input.push_back(Point<double>(0.0, 0.0));
    input.push_back(Point<double>(1.0, 0.0));
    input.push_back(Point<double>(0.0, 1.0));
    input.push_back(Point<double>(1.0, 2.0));
    auto indicies = Triangulate(client, input);
    VerifyTriangulate(input, indicies, 1.5);

    //площадь = 3.0
    input.clear();
    input.push_back(Point<double>(0.0, 0.0));
    input.push_back(Point<double>(1.0, 0.0));
    input.push_back(Point<double>(0.0, 1.0));
    input.push_back(Point<double>(1.0, 2.0));
    input.push_back(Point<double>(2.0, 2.0));
    input.push_back(Point<double>(2.0, 1.0));
    indicies = Triangulate(client, input);
    VerifyTriangulate(input, indicies, 3.0);

    //площадь = 1.0
    input.clear();
    input.push_back(Point<double>(0.0, 0.0));
    input.push_back(Point<double>(1.0, 0.0));
    input.push_back(Point<double>(0.0, 1.0));
    input.push_back(Point<double>(1.0, 1.0));
    input.push_back(Point<double>(0.6, 0.5));
    indicies = Triangulate(client, input);
    VerifyTriangulate(input, indicies, 1.0);

    //площадь = 10.0 для любого количества точек внутри фигуры
    input.clear();
    input.push_back(Point<double>(0.0, 0.0));
    input.push_back(Point<double>(5.0, 0.0));
    input.push_back(Point<double>(0.0, 2.0));
    input.push_back(Point<double>(5.0, 2.0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dist(0.0, 1.0);
    for (int i = 0; i < 50; i++) {
        input.push_back(Point<double>(5.0 * dist(gen), 2.0 * dist(gen)));
    }
    indicies = Triangulate(client, input);
    VerifyTriangulate(input, indicies, 10.0);
}