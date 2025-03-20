#include "algorithms/brute_force.hpp"
#include <algorithm>
#include <limits>
#include <queue>
#include <functional>
#include <chrono>

BruteForce::BruteForce(std::shared_ptr<DistanceMetric> metric)
    : NearestNeighborAlgorithm(std::move(metric)) {}

AlgorithmPerformance BruteForce::build(const std::vector<std::vector<double>>& dataset) {
    AlgorithmPerformance performance;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Просто сохраняем набор данных, без предобработки
    dataset_ = dataset;
    
    auto end = std::chrono::high_resolution_clock::now();
    
    // Измеряем время построения
    performance.preprocess_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Приблизительно оцениваем использование памяти (размер векторов)
    size_t memory = 0;
    for (const auto& point : dataset) {
        memory += point.size() * sizeof(double); // Размер точки
    }
    memory += sizeof(dataset_); // Размер вектора
    
    performance.memory_usage = memory;
    performance.recall = 1.0; // Наивный алгоритм всегда находит точный ответ
    
    return performance;
}

std::pair<size_t, double> BruteForce::findNearest(const std::vector<double>& query) const {
    if (dataset_.empty()) {
        throw std::runtime_error("Dataset is empty. Call build() first.");
    }
    
    size_t nearest_idx = 0;
    double min_distance = std::numeric_limits<double>::max();
    
    // Перебираем все точки и находим ближайшую
    for (size_t i = 0; i < dataset_.size(); ++i) {
        double dist = metric_->distance(query, dataset_[i]);
        if (dist < min_distance) {
            min_distance = dist;
            nearest_idx = i;
        }
    }
    
    return {nearest_idx, min_distance};
}

std::vector<std::pair<size_t, double>> BruteForce::findKNearest(
    const std::vector<double>& query, size_t k) const {
    
    if (dataset_.empty()) {
        throw std::runtime_error("Dataset is empty. Call build() first.");
    }
    
    if (k > dataset_.size()) {
        k = dataset_.size(); // Ограничиваем k размером набора данных
    }
    
    // Используем очередь с приоритетом, чтобы хранить k ближайших соседей
    // Приоритет по расстоянию (меньше - лучше)
    std::priority_queue<
        std::pair<double, size_t>,
        std::vector<std::pair<double, size_t>>,
        std::greater<>> nearest;
    
    // Перебираем все точки 
    for (size_t i = 0; i < dataset_.size(); ++i) {
        double dist = metric_->distance(query, dataset_[i]);
        nearest.push({dist, i});
    }
    
    // Извлекаем k ближайших
    std::vector<std::pair<size_t, double>> result;
    result.reserve(k);
    
    for (size_t i = 0; i < k && !nearest.empty(); ++i) {
        auto [dist, idx] = nearest.top();
        nearest.pop();
        result.push_back({idx, dist});
    }
    
    return result;
}

std::string BruteForce::name() const {
    return "BruteForce";
}
