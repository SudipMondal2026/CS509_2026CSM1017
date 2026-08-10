#include "csr_graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

static int read_adjlist_common(const std::string &path, int &V, int &E,
                                AdjList &adj, bool weighted) {
    std::ifstream fin(path);
    if (!fin.is_open())
        throw std::runtime_error("Could not open input file: " + path);

    if (!(fin >> V >> E))
        throw std::runtime_error("Malformed header line (expected: V E)");
    if (V <= 0)
        throw std::runtime_error("V must be positive");

    adj.assign(V, {});

    for (int line = 0; line < V; ++line) {
        int u, degree;
        if (!(fin >> u >> degree))
            throw std::runtime_error("Malformed adjacency line for vertex " + std::to_string(line));
        if (u < 0 || u >= V)
            throw std::runtime_error("Vertex id out of range: " + std::to_string(u));

        adj[u].reserve(degree);
        for (int d = 0; d < degree; ++d) {
            int nbr;
            double w = 1.0;
            if (!(fin >> nbr))
                throw std::runtime_error("Malformed neighbor list for vertex " + std::to_string(u));
            if (weighted) {
                if (!(fin >> w))
                    throw std::runtime_error("Missing edge weight for vertex " + std::to_string(u));
            }
            adj[u].push_back({nbr, w});
        }
    }

    std::string tag;
    int source = 0;
    if (fin >> tag >> source) {
        if (tag != "SOURCE")
            throw std::runtime_error("Expected SOURCE line, got: " + tag);
    } else {
        throw std::runtime_error("Missing SOURCE line");
    }

    return source;
}

int read_unweighted_adjlist(const std::string &path, int &V, int &E, AdjList &adj) {
    return read_adjlist_common(path, V, E, adj, /*weighted=*/false);
}

int read_weighted_adjlist(const std::string &path, int &V, int &E, AdjList &adj) {
    return read_adjlist_common(path, V, E, adj, /*weighted=*/true);
}

CSRGraph adjlist_to_csr(const AdjList &adj, bool weighted) {
    CSRGraph g;
    g.V = static_cast<int>(adj.size());
    g.weighted = weighted;
    g.row_ptr.assign(g.V + 1, 0);

    for (int u = 0; u < g.V; ++u) {
        g.row_ptr[u + 1] = g.row_ptr[u] + static_cast<int>(adj[u].size());
    }

    int total_edges = g.row_ptr[g.V];
    g.col_idx.resize(total_edges);
    g.values.resize(total_edges);

    for (int u = 0; u < g.V; ++u) {
        int idx = g.row_ptr[u];
        for (const auto &edge : adj[u]) {
            g.col_idx[idx] = edge.first;
            g.values[idx]  = weighted ? edge.second : 1.0;
            ++idx;
        }
    }

    return g;
}

void print_csr(const CSRGraph &g) {
    std::cout << "row_ptr: ";
    for (int v : g.row_ptr) std::cout << v << " ";
    std::cout << "\ncol_idx: ";
    for (int v : g.col_idx) std::cout << v << " ";
    if (g.weighted) {
        std::cout << "\nvalues:  ";
        for (double v : g.values) std::cout << v << " ";
    }
    std::cout << "\n";
}
