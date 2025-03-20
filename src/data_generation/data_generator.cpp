#include "data_generation/data_generator.hpp"

UniformDataGenerator::UniformDataGenerator(double min, double max, unsigned seed)
    : min_(min), max_(max), gen_(seed) {}

std::vector<std::vector<double>> UniformDataGenerator::generate(size_t num_points, size_t dimensions) {
    std::vector<std::vector<double>> data;
    data.reserve(num_points);
    
    std::uniform_real_distribution<double> dist(min_, max_);
    
    for (size_t i = 0; i < num_points; ++i) {
        std::vector<double> point;
        point.reserve(dimensions);
        
        for (size_t j = 0; j < dimensions; ++j) {
            point.push_back(dist(gen_));
        }
        
        data.push_back(std::move(point));
    }
    
    return data;
}

std::string UniformDataGenerator::name() const {
    return "Uniform";
} 