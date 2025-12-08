#pragma once

#include <numeric>
#include <vector>

class DisjointSet {
  public:
    explicit DisjointSet(size_t n) : parent_(n), rank_(n, 0) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    size_t find(size_t x) {
        if (parent_[x] != x) {
            parent_[x] = find(parent_[x]);
        }
        return parent_[x];
    }

    void unite(size_t x, size_t y) {
        size_t rootX = find(x);
        size_t rootY = find(y);
        if (rootX != rootY) {
            if (rank_[rootX] < rank_[rootY]) {
                std::swap(rootX, rootY);
            }
            parent_[rootY] = rootX;
            if (rank_[rootX] == rank_[rootY]) {
                rank_[rootX]++;
            }
        }
    }

  private:
    std::vector<size_t> parent_;
    std::vector<size_t> rank_;
};