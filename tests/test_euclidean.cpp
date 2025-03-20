#include <cassert>
#include <iostream>
#include <cmath>
#include "metrics/euclidean_distance.hpp"

bool test_euclidean_distance() {
    EuclideanDistance euclidean;
    
    // Проверка на совпадающих точках
    std::vector<double> p1 = {1.0, 2.0, 3.0};
    std::vector<double> p2 = {1.0, 2.0, 3.0};
    double distance = euclidean.distance(p1, p2);
    assert(distance == 0.0);
    
    // Проверка на простых точках
    p2 = {4.0, 6.0, 8.0};
    distance = euclidean.distance(p1, p2);
    double expected = std::sqrt(34.0); // sqrt((4-1)^2 + (6-2)^2 + (8-3)^2)
    assert(std::abs(distance - expected) < 1e-9);
    
    // Проверка на точках разной размерности
    try {
        p2 = {4.0, 6.0};
        euclidean.distance(p1, p2);
        // Должно выбросить исключение
        std::cerr << "Test FAILED: no exception thrown for vectors of different dimensions" << std::endl;
        return false;
    } catch (const std::invalid_argument&) {
        // Ожидаемое поведение
    }
    
    // Проверка названия метрики
    assert(euclidean.name() == "Euclidean");
    
    return true;
} 