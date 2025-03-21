#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <iomanip>
#include <iostream>

#include "metrics/euclidean_distance.hpp"
#include "algorithms/brute_force.hpp"
#include "algorithms/lsh.hpp"
#include "data_generation/data_generator.hpp"
#include "benchmark/benchmark.hpp"

int main(int argc, char** argv) {
    Benchmark benchmark;
    size_t dataset_size = 1000;
    size_t query_size = 100;
    size_t dimensions = 10;
    int algorithm_choice = 0; // 0 - BruteForce, 1 - LSH

    if (argc > 1) dataset_size = std::stoi(argv[1]);
    if (argc > 2) query_size = std::stoi(argv[2]);
    if (argc > 3) dimensions = std::stoi(argv[3]);
    if (argc > 4) algorithm_choice = std::stoi(argv[4]);

    std::cout << "Запуск бенчмарка поиска ближайшего соседа" << std::endl;
    std::cout << "Размер набора данных: " << dataset_size << std::endl;
    std::cout << "Количество запросов: " << query_size << std::endl;
    std::cout << "Размерность: " << dimensions << std::endl;
    std::cout << "Алгоритм: " << (algorithm_choice == 0 ? "BruteForce" : "LSH") << std::endl;

    auto metric = std::make_shared<EuclideanDistance>();
    UniformDataGenerator generator(-1.0, 1.0);
    std::mt19937 rng(42);

    std::cout << "Генерация набора данных..." << std::endl;
    auto dataset = generator.generate(dataset_size, dimensions);
    auto query_set = generator.generate(query_size, dimensions);

    std::cout << "Инициализация алгоритма..." << std::endl;
    std::shared_ptr<NearestNeighborAlgorithm> algo;
    if (algorithm_choice == 0) {
        algo = std::make_shared<BruteForce>(metric);
    } else if (algorithm_choice == 1) {
        algo = auto_tune_lsh(dataset, query_set, metric, rng);
    } else {
        std::cerr << "Неверный выбор алгоритма! Используйте 0 для BruteForce или 1 для LSH." << std::endl;
        return 1;
    }

    std::cout << "Нахождение истинных ближайших соседей..." << std::endl;
    auto true_neighbors = Benchmark::findTrueNeighbors(dataset, query_set, metric);

    std::cout << "Запуск бенчмарка..." << std::endl;
    auto result = benchmark.runBenchmark(algo, dataset, query_set, true_neighbors, generator.name());

    std::cout << "\nРезультаты бенчмарка:" << std::endl;
    std::cout << "Алгоритм: " << result.algorithm_name << std::endl;
    std::cout << "Метрика: " << result.metric_name << std::endl;
    std::cout << "Распределение данных: " << result.data_distribution << std::endl;
    std::cout << "Время предобработки: " << result.preprocess_time.count() << " мкс" << std::endl;
    std::cout << "Использовано памяти: " << result.memory_usage << " байт" << std::endl;
    std::cout << "Среднее время запроса: " << result.avg_query_time.count() << " мкс" << std::endl;
    std::cout << "Минимальное время запроса: " << result.min_query_time.count() << " мкс" << std::endl;
    std::cout << "Максимальное время запроса: " << result.max_query_time.count() << " мкс" << std::endl;
    std::cout << "Полнота (recall): " << std::fixed << std::setprecision(4) << result.recall << std::endl;

    // Сохраняем результаты в CSV-файл
    //std::vector<BenchmarkResult> results = {result};
    //benchmark.saveResults(results, "benchmark_results.csv");
    //std::cout << "Результаты сохранены в benchmark_results.csv" << std::endl;

    return 0;
}
