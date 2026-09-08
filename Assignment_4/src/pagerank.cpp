#include "pagerank.h"
#include <cmath>

PageRankResult pagerank(const CSRGraph &g, double damping, double tolerance, int maxIterations) {
    const int V = g.V;
    PageRankResult result;
    result.ranks.assign(V, V > 0 ? 1.0 / V : 0.0);

    if (V == 0) {
        result.converged = true;
        return result;
    }

    std::vector<int> outdeg(V);
    for (int v = 0; v < V; ++v) outdeg[v] = g.row_ptr[v + 1] - g.row_ptr[v];

    std::vector<double> prev = result.ranks;
    std::vector<double> next(V);

    const double baseTerm = (1.0 - damping) / V;

    int iter = 0;
    bool converged = false;

    for (; iter < maxIterations; ++iter) {
        // Every vertex simultaneously starts from the (1-d)/N teleport term.
        std::fill(next.begin(), next.end(), baseTerm);

        // Dangling vertices (outdegree 0): distribute their rank evenly
        // across ALL vertices instead of dividing by zero.
        double danglingSum = 0.0;
        for (int u = 0; u < V; ++u) {
            if (outdeg[u] == 0) danglingSum += prev[u];
        }
        if (danglingSum > 0.0) {
            double danglingShare = damping * danglingSum / V;
            for (int v = 0; v < V; ++v) next[v] += danglingShare;
        }

        // Push-based update, equivalent to summing over incoming edges but
        // expressed directly against the CSR (outgoing-edge) representation:
        // each u distributes PR(u)/outdegree(u) to every out-neighbour v.
        for (int u = 0; u < V; ++u) {
            if (outdeg[u] == 0) continue;
            double share = damping * prev[u] / outdeg[u];
            for (int idx = g.row_ptr[u]; idx < g.row_ptr[u + 1]; ++idx) {
                int v = g.col_idx[idx];
                next[v] += share;
            }
        }

        // Total change across all vertices (simultaneous update, using the
        // previous iteration's values only, per Section 2.2).
        double totalChange = 0.0;
        for (int v = 0; v < V; ++v) totalChange += std::fabs(next[v] - prev[v]);

        std::swap(prev, next);

        if (totalChange <= tolerance) {
            converged = true;
            ++iter; // this iteration counts toward the reported total
            break;
        }
    }

    if (!converged) iter = maxIterations;

    result.ranks = prev;
    result.iterations = iter;
    result.converged = converged;
    return result;
}
