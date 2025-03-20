#include "benchmark/benchmark.hpp"
#include <algorithm>
#include <numeric>
#include <limits>
#include <iostream>

BenchmarkResult Benchmark::runBenchmark(
    std::shared_ptr<NearestNeighborAlgorithm> algorithm,
    const std::vector<std::vector<double>>& dataset,
    const std::vector<std::vector<double>>& query_set,
    const std::vector<std::pair<size_t, double>>& true_neighbors,
    const std::string& data_distribution) {
    
    // Инициализируем результаты
    BenchmarkResult result;
    result.algorithm_name = algorithm->name();
    result.metric_name = algorithm->metric().name();
    result.data_distribution = data_distribution;
    result.dataset_size = dataset.size();
    result.dimensions = dataset.empty() ? 0 : dataset[0].size();
    
    // Строим индекс и замеряем время и память
    AlgorithmPerformance build_perf = algorithm->build(dataset);
    result.preprocess_time = build_perf.preprocess_time;
    result.memory_usage = build_perf.memory_usage;
    
    // Замеряем время поиска
    std::vector<std::chrono::microseconds> query_times;
    std::vector<std::pair<size_t, double>> found_neighbors;
    found_neighbors.reserve(query_set.size());
    
    for (const auto& query : query_set) {
        auto start = std::chrono::high_resolution_clock::now();
        auto nearest = algorithm->findNearest(query);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto query_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        query_times.push_back(query_time);
        found_neighbors.push_back(nearest);
    }
    
    // Вычисляем статистику по времени запросов
    if (!query_times.empty()) {
        result.avg_query_time = std::chrono::microseconds(
            std::accumulate(query_times.begin(), query_times.end(), 
                            std::chrono::microseconds(0)).count() / query_times.size());
        
        auto [min_it, max_it] = std::minmax_element(query_times.begin(), query_times.end());
        result.min_query_time = *min_it;
        result.max_query_time = *max_it;
    }
    
    // Вычисляем полноту (recall)
    if (true_neighbors.size() == found_neighbors.size() && !true_neighbors.empty()) {
        size_t correct_count = 0;
        for (size_t i = 0; i < true_neighbors.size(); ++i) {
            if (true_neighbors[i].first == found_neighbors[i].first) {
                correct_count++;
            }
        }
        result.recall = static_cast<double>(correct_count) / true_neighbors.size();
    } else {
        result.recall = 0.0;
    }
    
    return result;
}

void Benchmark::saveResults(const std::vector<BenchmarkResult>& results, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    
    // Записываем заголовки столбцов
    file << "Algorithm,Metric,Distribution,DatasetSize,Dimensions,"
         << "PreprocessTime(µs),MemoryUsage(bytes),"
         << "AvgQueryTime(µs),MinQueryTime(µs),MaxQueryTime(µs),Recall\n";
    
    // Записываем результаты
    for (const auto& result : results) {
        file << result.algorithm_name << ","
             << result.metric_name << ","
             << result.data_distribution << ","
             << result.dataset_size << ","
             << result.dimensions << ","
             << result.preprocess_time.count() << ","
             << result.memory_usage << ","
             << result.avg_query_time.count() << ","
             << result.min_query_time.count() << ","
             << result.max_query_time.count() << ","
             << result.recall << "\n";
    }
    
    file.close();
}

std::vector<std::pair<size_t, double>> Benchmark::findTrueNeighbors(
    const std::vector<std::vector<double>>& dataset,
    const std::vector<std::vector<double>>& query_set,
    const std::shared_ptr<DistanceMetric>& metric) {
    
    std::vector<std::pair<size_t, double>> true_neighbors;
    true_neighbors.reserve(query_set.size());
    
    for (const auto& query : query_set) {
        size_t nearest_idx = 0;
        double min_distance = std::numeric_limits<double>::max();
        
        for (size_t i = 0; i < dataset.size(); ++i) {
            double dist = metric->distance(query, dataset[i]);
            if (dist < min_distance) {
                min_distance = dist;
                nearest_idx = i;
            }
        }
        
        true_neighbors.push_back({nearest_idx, min_distance});
    }
    
    return true_neighbors;
} 