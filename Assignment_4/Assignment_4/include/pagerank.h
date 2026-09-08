#ifndef PAGERANK_H
#define PAGERANK_H

#include "csr_graph.h"
#include <vector>

struct PageRankResult {
    std::vector<double> ranks;
    int iterations = 0;
    bool converged = false;
};

// PageRank (Assignment 4 spec, Section 2.2):
//   PR(v) = (1-d)/N + d * sum over u->v of ( PR(u) / outdegree(u) )
// - All vertices initialized to 1/N.
// - Dangling vertices (outdegree 0) distribute their rank evenly across ALL
//   vertices instead of causing a divide-by-zero.
// - Every vertex is updated simultaneously using the PREVIOUS iteration's
//   values (Jacobi-style sweep, not Gauss-Seidel).
// - Stops when the total absolute change across all vertices is <=
//   tolerance, or when maxIterations is reached (whichever first).
PageRankResult pagerank(const CSRGraph &g, double damping, double tolerance, int maxIterations);

#endif
