#ifndef DATA_GENERATOR_HPP
#define DATA_GENERATOR_HPP

#include <vector>
#include <string>
#include <random>

/**
 * @brief Интерфейс для генератора данных
 * 
 * Этот класс определяет общий интерфейс для генераторов данных с различными
 * распределениями.
 */
class DataGenerator {
public:
    /**
     * @brief Генерирует набор данных
     * 
     * @param num_points Количество точек
     * @param dimensions Размерность пространства
     * @return std::vector<std::vector<double>> Сгенерированные точки
     */
    virtual std::vector<std::vector<double>> generate(size_t num_points, size_t dimensions) = 0;
    
    /**
     * @brief Возвращает название распределения
     * 
     * @return std::string
     */
    virtual std::string name() const = 0;

    virtual ~DataGenerator() = default;
};

/**
 * @brief Генератор данных с равномерным распределением
 */
class UniformDataGenerator : public DataGenerator {
public:
    /**
     * @brief Конструктор с параметрами распределения
     * 
     * @param min Минимальное значение
     * @param max Максимальное значение
     * @param seed Seed для генератора случайных чисел
     */
    UniformDataGenerator(double min = 0.0, double max = 1.0, unsigned seed = std::random_device{}());
    
    /**
     * @brief Генерирует набор данных с равномерным распределением
     * 
     * @param num_points Количество точек
     * @param dimensions Размерность пространства
     * @return std::vector<std::vector<double>> Сгенерированные точки
     */
    std::vector<std::vector<double>> generate(size_t num_points, size_t dimensions) override;
    
    /**
     * @brief Возвращает название распределения
     * 
     * @return std::string "Uniform"
     */
    std::string name() const override;
    
private:
    double min_;
    double max_;
    std::mt19937 gen_;
};

#endif // DATA_GENERATOR_HPP 