#ifndef NEAREST_NEIGHBOR_ALGORITHM_HPP
#define NEAREST_NEIGHBOR_ALGORITHM_HPP

#include <vector>
#include <memory>
#include <utility>
#include <chrono>
#include "metrics/distance_metric.hpp"

/**
 * @brief Структура для результатов бенчмарка алгоритма
 */
struct AlgorithmPerformance {
    std::chrono::microseconds preprocess_time{0}; // Время предобработки
    size_t memory_usage{0};                       // Использованная память в байтах
    std::chrono::microseconds query_time{0};      // Среднее время запроса
    double recall{0.0};                           // Полнота (доля истинно положительных)
};

/**
 * @brief Интерфейс для алгоритмов поиска ближайшего соседа
 */
class NearestNeighborAlgorithm {
public:
    /**
     * @brief Конструктор с метрикой расстояния
     * 
     * @param metric Метрика расстояния
     */
    explicit NearestNeighborAlgorithm(std::shared_ptr<DistanceMetric> metric) 
        : metric_(std::move(metric)) {}
    
    /**
     * @brief Инициализирует алгоритм набором данных
     * 
     * @param dataset Набор точек данных
     * @return AlgorithmPerformance Результаты предобработки
     */
    virtual AlgorithmPerformance build(const std::vector<std::vector<double>>& dataset) = 0;
    
    /**
     * @brief Находит ближайшего соседа к запрашиваемой точке
     * 
     * @param query Запрашиваемая точка
     * @return std::pair<size_t, double> Индекс ближайшего соседа и расстояние до него
     */
    virtual std::pair<size_t, double> findNearest(const std::vector<double>& query) const = 0;
    
    /**
     * @brief Находит k ближайших соседей к запрашиваемой точке
     * 
     * @param query Запрашиваемая точка
     * @param k Количество соседей для поиска
     * @return std::vector<std::pair<size_t, double>> Индексы ближайших соседей и расстояния до них
     */
    virtual std::vector<std::pair<size_t, double>> findKNearest(
        const std::vector<double>& query,
        size_t k
    ) const = 0;
    
    /**
     * @brief Возвращает название алгоритма
     * 
     * @return std::string Название алгоритма
     */
    virtual std::string name() const = 0;
    
    /**
     * @brief Возвращает метрику расстояния
     * 
     * @return const DistanceMetric& Используемая метрика
     */
    const DistanceMetric& metric() const { return *metric_; }
    
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~NearestNeighborAlgorithm() = default;
    
protected:
    std::shared_ptr<DistanceMetric> metric_;
    std::vector<std::vector<double>> dataset_;
};

#endif // NEAREST_NEIGHBOR_ALGORITHM_HPP 