#include "../include/algorithms/lsh.hpp"
#include <chrono>
#include <queue>
#include <functional>
#include <limits>
#include "../include//algorithms/brute_force.hpp"

LSH::LSH(std::shared_ptr<DistanceMetric> metric, int num_tables, int hash_size, double w, std::mt19937& rng)
        : NearestNeighborAlgorithm(metric), num_tables_(num_tables), hash_size_(hash_size), w_(w), rng_(rng) {
    hash_tables_.resize(num_tables_);
    hash_offsets_.resize(num_tables_);
    tables_.resize(num_tables_);
    for (int i = 0; i < num_tables_; ++i) {
        hash_tables_[i].resize(hash_size_);
        hash_offsets_[i].resize(hash_size_);
        for (int j = 0; j < hash_size_; ++j) {
            hash_tables_[i][j] = std::vector<double>();
            std::uniform_real_distribution<double> offset_dist(0.0, w_);
            hash_offsets_[i][j] = offset_dist(rng_);
        }
    }
}

AlgorithmPerformance LSH::build(const std::vector<std::vector<double>>& dataset) {
    auto start = std::chrono::high_resolution_clock::now();

    dataset_ = dataset;
    tables_.clear();
    tables_.resize(num_tables_);
    for (int i = 0; i < num_tables_; ++i) {
        tables_[i] = std::unordered_map<int, std::vector<size_t>>();
        for (int j = 0; j < hash_size_; ++j) {
            hash_tables_[i][j].resize(dataset[0].size());
            std::uniform_real_distribution<double> dist(-1.0, 1.0);
            for (size_t k = 0; k < dataset[0].size(); ++k) {
                hash_tables_[i][j][k] = dist(rng_);
            }
        }
    }

    for (size_t i = 0; i < dataset_.size(); ++i) {
        add_point(dataset_[i], i);
    }

    auto end = std::chrono::high_resolution_clock::now();
    AlgorithmPerformance performance;
    performance.preprocess_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    size_t memory = dataset_.size() * sizeof(std::vector<double>);
    for (const auto& point : dataset_) {
        memory += point.size() * sizeof(double);
    }
    for (const auto& table : tables_) {
        memory += sizeof(table) + table.size() * (sizeof(int) + sizeof(std::vector<size_t>));
        for (const auto& bucket : table) {
            memory += bucket.second.size() * sizeof(size_t);
        }
    }
    performance.memory_usage = memory;
    performance.recall = 0.0; // Вычисляется в бенчмарке

    return performance;
}

std::pair<size_t, double> LSH::findNearest(const std::vector<double>& query) const {
    if (dataset_.empty()) {
        throw std::runtime_error("Dataset is empty. Call build() first.");
    }

    size_t nearest_idx = 0;
    double min_dist = std::numeric_limits<double>::max();

    for (int i = 0; i < num_tables_; ++i) {
        int hash_code = hash_function(query, hash_tables_[i], hash_offsets_[i]);
        for (int offset = -5; offset <= 5; ++offset) {
            int check_hash = hash_code + offset;
            auto it = tables_[i].find(check_hash);
            if (it != tables_[i].end()) {
                for (size_t idx : it->second) {
                    double dist = metric_->distance(query, dataset_[idx]);
                    if (dist < min_dist) {
                        min_dist = dist;
                        nearest_idx = idx;
                    }
                }
            }
        }
    }

    return {nearest_idx, min_dist};
}

std::vector<std::pair<size_t, double>> LSH::findKNearest(const std::vector<double>& query, size_t k) const {
    if (dataset_.empty()) {
        throw std::runtime_error("Dataset is empty. Call build() first.");
    }
    if (k > dataset_.size()) {
        k = dataset_.size();
    }

    std::priority_queue<std::pair<double, size_t>, std::vector<std::pair<double, size_t>>, std::less<>> candidates;
    std::unordered_map<size_t, bool> seen;

    for (int i = 0; i < num_tables_; ++i) {
        int hash_code = hash_function(query, hash_tables_[i], hash_offsets_[i]);
        for (int offset = -5; offset <= 5; ++offset) {
            int check_hash = hash_code + offset;
            auto it = tables_[i].find(check_hash);
            if (it != tables_[i].end()) {
                for (size_t idx : it->second) {
                    if (!seen[idx]) {
                        seen[idx] = true;
                        double dist = metric_->distance(query, dataset_[idx]);
                        candidates.push({dist, idx});
                    }
                }
            }
        }
    }

    std::vector<std::pair<size_t, double>> result;
    result.reserve(k);
    for (size_t i = 0; i < k && !candidates.empty(); ++i) {
        auto [dist, idx] = candidates.top();
        candidates.pop();
        result.push_back({idx, dist});
    }

    return result;
}

std::string LSH::name() const {
    return "LSH";
}

int LSH::hash_function(const std::vector<double>& point, const std::vector<std::vector<double>>& table, const std::vector<double>& offsets) const {
    int hash_code = 0;
    for (int j = 0; j < hash_size_; ++j) {
        double dot = 0.0;
        for (size_t k = 0; k < point.size(); ++k) {
            dot += point[k] * table[j][k];
        }
        hash_code = hash_code * 31 + static_cast<int>((dot + offsets[j]) / w_);
    }
    return hash_code;
}

void LSH::add_point(const std::vector<double>& point, size_t idx) {
    for (int i = 0; i < num_tables_; ++i) {
        int hash_code = hash_function(point, hash_tables_[i], hash_offsets_[i]);
        tables_[i][hash_code].push_back(idx);
    }
}

std::shared_ptr<LSH> auto_tune_lsh(const std::vector<std::vector<double>>& points,
                                   const std::vector<std::vector<double>>& queries,
                                   std::shared_ptr<DistanceMetric> metric,
                                   std::mt19937& rng) {
    double avg_dist = 0.0;
    int count = 0;
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            avg_dist += metric->distance(points[i], points[j]);
            count++;
        }
    }
    avg_dist = count > 0 ? avg_dist / count : 1.0;

    std::vector<double> w_values = {avg_dist * 0.25, avg_dist * 0.5, avg_dist, avg_dist * 2.0, avg_dist * 4.0};
    std::vector<int> num_tables_values = {1, 3, 5, 7};
    std::vector<int> hash_size_values = {2, 4, 6, 8};

    double best_score = -std::numeric_limits<double>::max();
    double best_w = avg_dist;
    int best_num_tables = 1;
    int best_hash_size = 2;

    for (double w : w_values) {
        for (int nt : num_tables_values) {
            for (int hs : hash_size_values) {
                LSH lsh(metric, nt, hs, w, rng);
                auto perf = lsh.build(points);

                double total_recall = 0.0;
                double total_time = 0.0;
                for (const auto& query : queries) {
                    auto start = std::chrono::high_resolution_clock::now();
                    auto [lsh_idx, lsh_dist] = lsh.findNearest(query);
                    auto end = std::chrono::high_resolution_clock::now();
                    double time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

                    BruteForce brute(metric);
                    brute.build(points);
                    auto [true_idx, true_dist] = brute.findNearest(query);

                    total_recall += (lsh_idx == true_idx) ? 1.0 : 0.0;
                    total_time += time;
                }

                double avg_recall = total_recall / queries.size();
                double avg_time = total_time / queries.size();
                double score = avg_recall * 100.0 - avg_time * 0.1;

                if (score > best_score) {
                    best_score = score;
                    best_w = w;
                    best_num_tables = nt;
                    best_hash_size = hs;
                }
            }
        }
    }

    return std::make_shared<LSH>(metric, best_num_tables, best_hash_size, best_w, rng);
}