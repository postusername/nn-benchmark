#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <fstream>
#include "algorithms/nearest_neighbor_algorithm.hpp"
#include "data_generation/data_generator.hpp"

/**
 * @brief Результаты бенчмарка для алгоритма
 */
struct BenchmarkResult {
    std::string algorithm_name;                  // Название алгоритма
    std::string metric_name;                     // Название метрики
    std::string data_distribution;               // Название распределения данных
    size_t dataset_size;                         // Размер набора данных
    size_t dimensions;                           // Размерность пространства
    std::chrono::microseconds preprocess_time;   // Время предобработки
    size_t memory_usage;                         // Использованная память
    std::chrono::microseconds avg_query_time;    // Среднее время запроса
    std::chrono::microseconds min_query_time;    // Минимальное время запроса
    std::chrono::microseconds max_query_time;    // Максимальное время запроса
    double recall;                               // Полнота поиска
};

/**
 * @brief Класс для проведения бенчмарков алгоритмов
 */
class Benchmark {
public:
    /**
     * @brief Проводит бенчмаркинг алгоритма на наборе данных
     * 
     * @param algorithm Алгоритм для тестирования
     * @param dataset Набор данных
     * @param query_set Набор запросов
     * @param true_neighbors Истинные ближайшие соседи для запросов
     * @param data_distribution Название распределения данных
     * @return BenchmarkResult Результаты бенчмарка
     */
    BenchmarkResult runBenchmark(
        std::shared_ptr<NearestNeighborAlgorithm> algorithm,
        const std::vector<std::vector<double>>& dataset,
        const std::vector<std::vector<double>>& query_set,
        const std::vector<std::pair<size_t, double>>& true_neighbors,
        const std::string& data_distribution);
    
    /**
     * @brief Сохраняет результаты бенчмарка в CSV-файл
     * 
     * @param results Результаты бенчмарка
     * @param filename Имя файла для сохранения
     */
    void saveResults(const std::vector<BenchmarkResult>& results, 
                     const std::string& filename = "benchmark_results.csv");
    
    /**
     * @brief Находит истинные ближайшие соседи для набора запросов
     * 
     * @param dataset Набор данных
     * @param query_set Набор запросов
     * @param metric Метрика расстояния
     * @return std::vector<std::pair<size_t, double>> Индексы и расстояния до истинных ближайших соседей
     */
    static std::vector<std::pair<size_t, double>> findTrueNeighbors(
        const std::vector<std::vector<double>>& dataset,
        const std::vector<std::vector<double>>& query_set,
        const std::shared_ptr<DistanceMetric>& metric);
};

#endif // BENCHMARK_HPP 