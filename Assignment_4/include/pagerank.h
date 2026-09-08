#ifndef PAGERANK_H
#define PAGERANK_H

#include "csr_graph.h"
#include <vector>

struct PageRankResult {
    std::vector<double> ranks;
    int iterations = 0;
    bool converged = false;
};

PageRankResult pagerank(const CSRGraph &g, double damping, double tolerance, int maxIterations);

#endif
