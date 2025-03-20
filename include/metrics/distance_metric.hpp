#ifndef DISTANCE_METRIC_HPP
#define DISTANCE_METRIC_HPP

#include <vector>
#include <string>

/**
 * @brief Интерфейс для метрики расстояния
 * 
 * Этот класс определяет общий интерфейс для различных метрик расстояния.
 * Каждая конкретная метрика должна реализовать этот интерфейс.
 */
class DistanceMetric {
public:
    /**
     * @brief Вычисляет расстояние между двумя точками
     * 
     * @param point1 Первая точка
     * @param point2 Вторая точка
     * @return double Расстояние между точками
     */
    virtual double distance(const std::vector<double>& point1, 
                            const std::vector<double>& point2) const = 0;
    
    /**
     * @brief Возвращает название метрики
     * 
     * @return std::string Название метрики
     */
    virtual std::string name() const = 0;
    
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~DistanceMetric() = default;
};

#endif // DISTANCE_METRIC_HPP 