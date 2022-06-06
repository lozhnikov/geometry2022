#include "methods.hpp"
#include "triangulate.hpp"

namespace geometry {
    std::vector <Point<double>>
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
