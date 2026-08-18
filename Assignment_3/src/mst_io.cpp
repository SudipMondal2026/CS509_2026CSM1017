#include "mst_io.h"
#include <fstream>
#include <stdexcept>

void read_mst_adjlist(const std::string &path, int &V, int &E, AdjList &adj) {
    std::ifstream fin(path);
    if (!fin.is_open())
        throw std::runtime_error("Could not open input file: " + path);

    if (!(fin >> V >> E))
        throw std::runtime_error("Malformed header line (expected: V E)");
    if (V <= 0)
        throw std::runtime_error("V must be positive");
    if (V == 1)
        throw std::runtime_error("MST requires at least 2 vertices");

    adj.assign(V, {});

    std::vector<bool> seen(V, false);
    for (int line = 0; line < V; ++line) {
        int u, degree;
        if (!(fin >> u >> degree))
            throw std::runtime_error("Malformed adjacency line for vertex " + std::to_string(line));
        if (u < 0 || u >= V)
            throw std::runtime_error("Vertex id out of range: " + std::to_string(u));
        if (seen[u])
            throw std::runtime_error("Duplicate adjacency line for vertex " + std::to_string(u));
        seen[u] = true;

        if (V > 1 && degree == 0)
            throw std::runtime_error("Isolated vertex " + std::to_string(u) +
                                      " is not allowed in a connected MST graph with V > 1");

        adj[u].reserve(degree);
        for (int d = 0; d < degree; ++d) {
            int nbr;
            double w;
            if (!(fin >> nbr >> w))
                throw std::runtime_error("Malformed neighbor/weight pair for vertex " + std::to_string(u));
            if (nbr < 0 || nbr >= V)
                throw std::runtime_error("Neighbor id out of range: " + std::to_string(nbr));
            if (nbr == u)
                throw std::runtime_error("Self-loop not allowed at vertex " + std::to_string(u));
            adj[u].push_back({nbr, w});
        }
    }
}
