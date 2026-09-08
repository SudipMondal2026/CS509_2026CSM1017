#include "csr_graph.h"
#include<cstddef>

CSRGraph adjlist_to_csr(const AdjList &adj) {
    CSRGraph g;
    g.V = static_cast<int>(adj.size());
    g.row_ptr.assign(g.V + 1, 0);

    for (int u = 0; u < g.V; ++u) {
        g.row_ptr[u + 1] = g.row_ptr[u] + static_cast<int>(adj[u].size());
    }
    g.E = g.row_ptr[g.V];
    g.col_idx.resize(g.E);

    for (int u = 0; u < g.V; ++u) {
        int base = g.row_ptr[u];
        for (int k = 0; k < adj[u].size(); ++k) {
            g.col_idx[base + k] = adj[u][k];
        }
    }
    return g;
}
