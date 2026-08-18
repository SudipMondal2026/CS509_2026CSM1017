#include "mst.h"
#include <algorithm>
#include <numeric>

namespace {

// Union-Find / Disjoint Set Union with path compression + union by rank.
class DSU {
public:
    explicit DSU(int n) : parent(n), rnk(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // path halving
            x = parent[x];
        }
        return x;
    }

    // Returns true if a and b were in different components (i.e. the union
    // was performed / the edge does not create a cycle).
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (rnk[a] < rnk[b]) std::swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b]) ++rnk[a];
        return true;
    }

private:
    std::vector<int> parent;
    std::vector<int> rnk;
};

struct RawEdge {
    int u, v;
    double w;
};

} // namespace

MSTResult kruskal_mst(const CSRGraph &g) {
    MSTResult result;
    const int V = g.V;
    if (V <= 0) return result;

    // --- Extraction of a sortable undirected edge list from the CSR arrays.
    // Each undirected edge is stored twice in the CSR (once per endpoint's
    // row), so only the (u < v) copy is kept. This extraction + the sort
    // that follows are both part of the timed Kruskal call, per the
    // assignment's timing rules (Section 8).
    std::vector<RawEdge> edges;
    edges.reserve(g.col_idx.size() / 2);
    for (int u = 0; u < V; ++u) {
        for (int idx = g.row_ptr[u]; idx < g.row_ptr[u + 1]; ++idx) {
            int v = g.col_idx[idx];
            if (v > u) {
                edges.push_back({u, v, g.values[idx]});
            }
        }
    }

    std::sort(edges.begin(), edges.end(),
              [](const RawEdge &a, const RawEdge &b) { return a.w < b.w; });

    DSU dsu(V);
    result.edges.reserve(static_cast<size_t>(V > 0 ? V - 1 : 0));

    int selected = 0;
    for (const auto &e : edges) {
        if (dsu.unite(e.u, e.v)) {
            result.edges.push_back({e.u, e.v, e.w});
            result.total_weight += e.w;
            ++selected;
            if (selected == V - 1) break;
        }
    }

    result.spans_all_vertices = (selected == V - 1);
    return result;
}
