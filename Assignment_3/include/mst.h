#ifndef MST_H
#define MST_H

#include "csr_graph.h"
#include <vector>

struct MSTEdge {
    int u;
    int v;
    double weight;
};

struct MSTResult {
    std::vector<MSTEdge> edges;
    double total_weight = 0.0;
    bool spans_all_vertices = false;
};
MSTResult kruskal_mst(const CSRGraph &g);
MSTResult prim_mst(const CSRGraph &g, int start = 0);

#endif
