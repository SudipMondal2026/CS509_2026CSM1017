#include "bellman_ford.h"
#include <vector>

BellmanFordResult bellmanford(const CSRGraph &g, int source) {
    BellmanFordResult result;
    result.distance.assign(g.V, BF_INF);
    result.predecessor.assign(g.V, -1);
    result.distance[source] = 0.0;


    for (int pass=0; pass<g.V-1; pass++){
        bool changed = false;
        for (int u= 0; u <g.V; u++){
            if (result.distance[u] == BF_INF) continue;
            for (int e = g.row_ptr[u]; e < g.row_ptr[u + 1]; e++) {
                int v = g.col_idx[e];
                double w = g.values[e];
                double new_dist = result.distance[u] + w;
                if (new_dist < result.distance[v]) {
                    result.distance[v] = new_dist;
                    result.predecessor[v] = u;
                    changed = true;
                }
            }
        }
        if (!changed) break;
    }

    for (int u= 0; u <g.V; u++){
            if (result.distance[u] == BF_INF) continue;
            for (int e = g.row_ptr[u]; e < g.row_ptr[u + 1]; e++) {
                int v = g.col_idx[e];
                double w = g.values[e];
                if (result.distance[u] + w < result.distance[v]) {
                    result.negative_cycle = true;
                    return result;
                }
            }
        }
    return result;
}