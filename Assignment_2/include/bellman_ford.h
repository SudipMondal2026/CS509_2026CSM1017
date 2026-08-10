#ifndef BELLMANFORD_H
#define BELLMANFORD_H

#include "csr_graph.h"
#include <vector>
#include <limits>

struct BellmanFordResult {
    std::vector<double> distance; 
    std::vector<int> predecessor;
    bool negative_cycle = false;
};

constexpr double BF_INF = std::numeric_limits<double>::infinity();

BellmanFordResult bellmanford(const CSRGraph &g, int source);

#endif
