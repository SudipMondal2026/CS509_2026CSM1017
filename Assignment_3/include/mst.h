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
    bool spans_all_vertices = false; // true if V-1 edges were selected (graph is connected)
};

// Kruskal's algorithm: sorts the undirected edge set extracted from the CSR
// representation and greedily adds edges using a Disjoint Set Union (DSU)
// structure, skipping any edge that would create a cycle.
MSTResult kruskal_mst(const CSRGraph &g);

// Prim's algorithm: grows a single tree from `start` using a min-priority
// queue over CSR adjacency, always picking the cheapest edge that reaches a
// new vertex outside the current tree.
MSTResult prim_mst(const CSRGraph &g, int start = 0);

#endif
