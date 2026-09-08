#ifndef COLORING_H
#define COLORING_H

#include "csr_graph.h"
#include <vector>

struct ColoringResult {
    std::vector<int> colors;
    int numColors = 0;     
};
ColoringResult welsh_powell_coloring(const CSRGraph &g);

bool validate_coloring(const CSRGraph &g, const std::vector<int> &colors);

#endif
