#include "metrics/euclidean_distance.hpp"
#include <cmath>
#include <stdexcept>


double EuclideanDistance::distance(const std::vector<double>& point1, 
                                   const std::vector<double>& point2) const {
    // Проверка на совпадение размерностей векторов
    if (point1.size() != point2.size()) {
        throw std::invalid_argument("Vectors must have the same dimension");
    }
    
    double sum_squared_diff = 0.0;
    for (size_t i = 0; i < point1.size(); ++i) {
        double diff = point1[i] - point2[i];
        sum_squared_diff += diff * diff;
    }
    
    return std::sqrt(sum_squared_diff);
}

std::string EuclideanDistance::name() const {
    return "Euclidean";
} 