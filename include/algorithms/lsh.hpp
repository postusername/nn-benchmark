#ifndef LSH_HPP
#define LSH_HPP

#include "nearest_neighbor_algorithm.hpp"
#include <vector>
#include <unordered_map>
#include <random>

class LSH : public NearestNeighborAlgorithm {
public:
    LSH(std::shared_ptr<DistanceMetric> metric, int num_tables, int hash_size, double w, std::mt19937& rng);
    AlgorithmPerformance build(const std::vector<std::vector<double>>& dataset) override;
    std::pair<size_t, double> findNearest(const std::vector<double>& query) const override;
    std::vector<std::pair<size_t, double>> findKNearest(const std::vector<double>& query, size_t k) const override;
    std::string name() const override;

private:
    int hash_function(const std::vector<double>& point, const std::vector<std::vector<double>>& table, const std::vector<double>& offsets) const;
    void add_point(const std::vector<double>& point, size_t idx);

    int num_tables_;
    int hash_size_;
    double w_;
    std::mt19937& rng_;
    std::vector<std::vector<std::vector<double>>> hash_tables_;
    std::vector<std::vector<double>> hash_offsets_;
    std::vector<std::unordered_map<int, std::vector<size_t>>> tables_;
};

std::shared_ptr<LSH> auto_tune_lsh(const std::vector<std::vector<double>>& points,
                                   const std::vector<std::vector<double>>& queries,
                                   std::shared_ptr<DistanceMetric> metric,
                                   std::mt19937& rng);

#endif