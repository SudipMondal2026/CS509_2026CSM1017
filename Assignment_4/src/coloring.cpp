#include "coloring.h"
#include <algorithm>
#include <numeric>

ColoringResult welsh_powell_coloring(const CSRGraph &g) {
    const int V = g.V;
    ColoringResult result;
    result.colors.assign(V, -1);

    // Step 1: degree of every vertex, directly from CSR row_ptr.
    std::vector<int> degree(V);
    for (int v = 0; v < V; ++v) {
        degree[v] = g.row_ptr[v + 1] - g.row_ptr[v];
    }

    // Step 2: order vertices by non-increasing degree (Welsh-Powell).
    std::vector<int> order(V);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        if (degree[a] != degree[b]) return degree[a] > degree[b];
        return a < b; // stable tie-break for reproducibility
    });

    // Step 3: process vertices in that order, assigning the smallest color
    // index not used by any already-colored neighbour.
    std::vector<bool> usedByNeighbour;
    int maxColorSeen = -1;

    for (int v : order) {
        // Reset the "used" scratch buffer only as far as we might need.
        usedByNeighbour.assign(maxColorSeen + 2, false);

        for (int idx = g.row_ptr[v]; idx < g.row_ptr[v + 1]; ++idx) {
            int nbr = g.col_idx[idx];
            int c = result.colors[nbr];
            if (c >= 0 && c < static_cast<int>(usedByNeighbour.size())) {
                usedByNeighbour[c] = true;
            }
        }

        int chosen = 0;
        while (chosen < static_cast<int>(usedByNeighbour.size()) && usedByNeighbour[chosen]) {
            ++chosen;
        }

        result.colors[v] = chosen;
        maxColorSeen = std::max(maxColorSeen, chosen);
    }

    result.numColors = maxColorSeen + 1;
    return result;
}

bool validate_coloring(const CSRGraph &g, const std::vector<int> &colors) {
    for (int v = 0; v < g.V; ++v) {
        for (int idx = g.row_ptr[v]; idx < g.row_ptr[v + 1]; ++idx) {
            int nbr = g.col_idx[idx];
            if (colors[v] == colors[nbr]) return false;
        }
    }
    return true;
}
