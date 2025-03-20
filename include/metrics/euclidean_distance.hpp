#ifndef EUCLIDEAN_DISTANCE_HPP
#define EUCLIDEAN_DISTANCE_HPP

#include "distance_metric.hpp"

/**
 * @brief Реализация евклидовой метрики расстояния
 */
class EuclideanDistance : public DistanceMetric {
public:
    /**
     * @brief Вычисляет евклидово расстояние между двумя точками
     * 
     * @param point1 Первая точка
     * @param point2 Вторая точка
     * @return double Евклидово расстояние между точками
     */
    double distance(const std::vector<double>& point1, 
                  const std::vector<double>& point2) const override;
    
    /**
     * @brief Возвращает название метрики
     * 
     * @return std::string "Euclidean"
     */
    std::string name() const override;
};

#endif // EUCLIDEAN_DISTANCE_HPP 