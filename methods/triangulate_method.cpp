/**
 * @file methods/contour_rectangles_method.cpp
 * @author Ivan Semochkin
 *
 * Файл содержит функцию, которая вызывает алгоритм Делоне.
 * Функция принимает и возвращает данные в JSON
 * формате.
 */
#include "methods.hpp"
#include "triangulate.hpp"

namespace geometry {
    std::vector <Point<double>>
    /**
    * @brief Десериализатор точек
    * @param j входная строка формата JSON
    * @return строка
    */
    DeserializePointVector(const nlohmann::json & j) {
        std::vector <Point<double>> result;
        double last_coord = 0;
        bool was_last_coord = false;
        for (auto & coord : j) {
            double double_coord = coord.get<double>();
            if (was_last_coord) {
                result.push_back(Point<double>(last_coord, double_coord));
                was_last_coord = false;
            } else {
                last_coord = double_coord;
                was_last_coord = true;
            }
        }
        return result;
    }
    /**
    * @brief Сериализатор индексов
    * @param v входная строка
    * Выходная строка формата JSON
    */
    void SerializeIndexVector(const std::vector<int> & v,
    nlohmann::json * output) {
        *output = nlohmann::json::array();
        for (auto & p : v) {
            nlohmann::json element = p;
            output->push_back(element);
        }
    }

    int Triangulate(const nlohmann::json& input, nlohmann::json* output) {
        auto points = DeserializePointVector(input);
        auto indicies = Triangulate(points);
        SerializeIndexVector(indicies, output);
        return 0;
    }
}  // namespace geometry
