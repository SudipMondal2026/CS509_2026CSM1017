#ifndef COLORING_H
#define COLORING_H

#include "csr_graph.h"
#include <vector>

struct ColoringResult {
    std::vector<int> colors; // colors[v] = color index assigned to vertex v
    int numColors = 0;       // total distinct colors used (max color index + 1)
};

// Greedy Vertex Coloring using Welsh-Powell ordering (Assignment 4 spec,
// Section 2.1.1):
//   1. Compute the degree of every vertex from the CSR representation.
//   2. Order vertices by non-increasing degree.
//   3. Process vertices in that order; assign each the smallest color index
//      not currently used by any already-colored neighbour.
// Works for any unweighted, undirected CSR graph (connected or not, may
// contain isolated vertices); does not hard-code colors for specific graphs.
ColoringResult welsh_powell_coloring(const CSRGraph &g);

// Verifies that no two adjacent vertices in `g` share a color.
bool validate_coloring(const CSRGraph &g, const std::vector<int> &colors);

#endif
