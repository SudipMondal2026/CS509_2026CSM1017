#include "mst.h"
#include <limits>
#include <queue>

MSTResult prim_mst(const CSRGraph &g, int start) {
    MSTResult result;
    const int V = g.V;
    if (V <= 0) return result;
    if (start < 0 || start >= V) start = 0;

    const double INF = std::numeric_limits<double>::infinity();
    std::vector<double> key(V, INF);
    std::vector<int> parent(V, -1);
    std::vector<bool> inMST(V, false);

    using PQItem = std::pair<double, int>; // (edge weight into tree, vertex)
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;

    key[start] = 0.0;
    pq.push({0.0, start});

    int included = 0;
    while (!pq.empty() && included < V) {
        auto top = pq.top();
        pq.pop();
        int u = top.second;
        
        if (inMST[u]) continue; // stale lazy-deleted entry
        inMST[u] = true;
        ++included;

        if (parent[u] != -1) {
            result.edges.push_back({parent[u], u, key[u]});
            result.total_weight += key[u];
        }

        for (int idx = g.row_ptr[u]; idx < g.row_ptr[u + 1]; ++idx) {
            int v = g.col_idx[idx];
            double w = g.values[idx];
            if (!inMST[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
                pq.push({w, v});
            }
        }
    }

    result.spans_all_vertices = (included == V);
    return result;
}
