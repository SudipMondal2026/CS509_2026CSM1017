#include "mst.h"
#include <algorithm>
#include <numeric>

namespace {

class DSU {
public:
    explicit DSU(int n) : parent(n), rnk(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; 
            x = parent[x];
        }
        return x;
    }
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

}

MSTResult kruskal_mst(const CSRGraph &g) {
    MSTResult result;
    const int V = g.V;
    if (V <= 0) return result;

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
