#include <cassert>
#include <iostream>
#include <memory>
#include "algorithms/brute_force.hpp"
#include "metrics/euclidean_distance.hpp"

bool test_brute_force() {
    // Создаем метрику
    auto euclidean = std::make_shared<EuclideanDistance>();
    
    // Создаем алгоритм
    BruteForce brute_force(euclidean);
    
    // Создаем простой набор данных
    std::vector<std::vector<double>> dataset = {
        {0.0, 0.0, 0.0}, // точка 0
        {1.0, 0.0, 0.0}, // точка 1
        {0.0, 1.0, 0.0}, // точка 2
        {0.0, 0.0, 1.0}, // точка 3
        {1.0, 1.0, 1.0}  // точка 4
    };
    
    // Строим индекс
    auto perf = brute_force.build(dataset);
    
    // Проверка поиска ближайшего соседа
    std::vector<double> query = {0.5, 0.5, 0.5};
    auto nearest = brute_force.findNearest(query);
    
    // Ближайшая точка должна быть точка 4 (1,1,1)
    assert(nearest.first == 4);
    
    // Проверка поиска K ближайших соседей
    auto k_nearest = brute_force.findKNearest(query, 3);
    assert(k_nearest.size() == 3);
    
    // Первая ближайшая должна быть той же, что и в findNearest
    assert(k_nearest[0].first == nearest.first);
    
    // Проверка имени алгоритма
    assert(brute_force.name() == "BruteForce");
    
    // Проверка на пустой набор данных
    std::vector<std::vector<double>> empty_dataset;
    brute_force.build(empty_dataset);
    
    try {
        brute_force.findNearest(query);
        // Должно выбросить исключение
        std::cerr << "Test FAILED: no exception thrown for empty dataset" << std::endl;
        return false;
    } catch (const std::runtime_error&) {
        // Ожидаемое поведение
    }
    
    return true;
} 