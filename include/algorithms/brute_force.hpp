#ifndef BRUTE_FORCE_HPP
#define BRUTE_FORCE_HPP

#include "nearest_neighbor_algorithm.hpp"

/**
 * @brief Реализация наивного алгоритма поиска ближайшего соседа
 * 
 * Алгоритм перебирает все точки в наборе данных и находит ближайшую
 * по заданной метрике расстояния.
 */
class BruteForce : public NearestNeighborAlgorithm {
public:
    /**
     * @brief Конструктор с метрикой расстояния
     * 
     * @param metric Метрика расстояния
     */
    explicit BruteForce(std::shared_ptr<DistanceMetric> metric);
    
    /**
     * @brief Инициализирует алгоритм набором данных
     * 
     * Для наивного алгоритма просто сохраняет набор данных.
     * 
     * @param dataset Набор точек данных
     * @return AlgorithmPerformance Результаты предобработки
     */
    AlgorithmPerformance build(const std::vector<std::vector<double>>& dataset) override;
    
    /**
     * @brief Находит ближайшего соседа к запрашиваемой точке
     * 
     * @param query Запрашиваемая точка
     * @return std::pair<size_t, double> Индекс ближайшего соседа и расстояние до него
     */
    std::pair<size_t, double> findNearest(const std::vector<double>& query) const override;
    
    /**
     * @brief Находит k ближайших соседей к запрашиваемой точке
     * 
     * @param query Запрашиваемая точка
     * @param k Количество соседей для поиска
     * @return std::vector<std::pair<size_t, double>> Индексы ближайших соседей и расстояния до них
     */
    std::vector<std::pair<size_t, double>> findKNearest(
        const std::vector<double>& query, size_t k) const override;
    
    /**
     * @brief Возвращает название алгоритма
     * 
     * @return std::string "BruteForce"
     */
    std::string name() const override;
};

#endif // BRUTE_FORCE_HPP 