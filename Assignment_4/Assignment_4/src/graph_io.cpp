#include "graph_io.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Reads one non-empty line, skipping blank lines, returning false at EOF.
// Line-based (rather than raw token-stream) parsing is used deliberately:
// it lets us check that a vertex's line contains EXACTLY `degree` neighbour
// tokens and nothing more or less, which a plain `>>` token stream cannot
// reliably detect (an over- or under-count would otherwise silently borrow
// tokens from the next line instead of being flagged as invalid).
static bool next_nonblank_line(std::ifstream &fin, std::string &line) {
    while (std::getline(fin, line)) {
        bool blank = true;
        for (char c : line) {
            if (!std::isspace(static_cast<unsigned char>(c))) { blank = false; break; }
        }
        if (!blank) return true;
    }
    return false;
}

void read_coloring_adjlist(const std::string &path, int &V, int &E, AdjList &adj) {
    std::ifstream fin(path);
    if (!fin.is_open())
        throw std::runtime_error("Could not open input file: " + path);

    std::string line;
    if (!next_nonblank_line(fin, line))
        throw std::runtime_error("Missing header line (expected: V E)");
    {
        std::istringstream iss(line);
        if (!(iss >> V >> E))
            throw std::runtime_error("Malformed header line (expected: V E)");
    }
    if (V <= 0)
        throw std::runtime_error("V must be positive");
    if (E < 0)
        throw std::runtime_error("E must be non-negative");

    adj.assign(V, {});
    std::vector<bool> seen(V, false);

    for (int line_no = 0; line_no < V; ++line_no) {
        if (!next_nonblank_line(fin, line))
            throw std::runtime_error("Missing adjacency line for entry " + std::to_string(line_no));

        std::istringstream iss(line);
        int u, degree;
        if (!(iss >> u >> degree))
            throw std::runtime_error("Malformed adjacency line for entry " + std::to_string(line_no));
        if (u < 0 || u >= V)
            throw std::runtime_error("Vertex id out of range: " + std::to_string(u));
        if (seen[u])
            throw std::runtime_error("Duplicate adjacency line for vertex " + std::to_string(u));
        if (degree < 0)
            throw std::runtime_error("Negative degree for vertex " + std::to_string(u));
        seen[u] = true;

        adj[u].reserve(degree);
        for (int d = 0; d < degree; ++d) {
            int nbr;
            if (!(iss >> nbr))
                throw std::runtime_error(
                    "Mismatched degree/neighbour count for vertex " + std::to_string(u) +
                    " (expected " + std::to_string(degree) + " neighbours, line has fewer)");
            if (nbr < 0 || nbr >= V)
                throw std::runtime_error("Neighbor id out of range: " + std::to_string(nbr));
            if (nbr == u)
                throw std::runtime_error("Self-loop not allowed at vertex " + std::to_string(u));
            adj[u].push_back(nbr);
        }
        int extra;
        if (iss >> extra)
            throw std::runtime_error(
                "Mismatched degree/neighbour count for vertex " + std::to_string(u) +
                " (expected " + std::to_string(degree) + " neighbours, line has more)");
    }
}

void read_pagerank_adjlist(const std::string &path, int &V, int &E, AdjList &adj,
                            double &damping, double &tolerance, int &maxIterations) {
    std::ifstream fin(path);
    if (!fin.is_open())
        throw std::runtime_error("Could not open input file: " + path);

    std::string line;
    if (!next_nonblank_line(fin, line))
        throw std::runtime_error("Missing header line (expected: V E)");
    {
        std::istringstream iss(line);
        if (!(iss >> V >> E))
            throw std::runtime_error("Malformed header line (expected: V E)");
    }
    if (V <= 0)
        throw std::runtime_error("V must be positive");
    if (E < 0)
        throw std::runtime_error("E must be non-negative");

    adj.assign(V, {});
    std::vector<bool> seen(V, false);

    for (int line_no = 0; line_no < V; ++line_no) {
        if (!next_nonblank_line(fin, line))
            throw std::runtime_error("Missing adjacency line for entry " + std::to_string(line_no));

        std::istringstream iss(line);
        int u, outdegree;
        if (!(iss >> u >> outdegree))
            throw std::runtime_error("Malformed adjacency line for entry " + std::to_string(line_no));
        if (u < 0 || u >= V)
            throw std::runtime_error("Vertex id out of range: " + std::to_string(u));
        if (seen[u])
            throw std::runtime_error("Duplicate adjacency line for vertex " + std::to_string(u));
        if (outdegree < 0)
            throw std::runtime_error("Negative outdegree for vertex " + std::to_string(u));
        seen[u] = true;

        adj[u].reserve(outdegree);
        for (int d = 0; d < outdegree; ++d) {
            int nbr;
            if (!(iss >> nbr))
                throw std::runtime_error(
                    "Mismatched outdegree/neighbour count for vertex " + std::to_string(u) +
                    " (expected " + std::to_string(outdegree) + " neighbours, line has fewer)");
            if (nbr < 0 || nbr >= V)
                throw std::runtime_error("Neighbor id out of range: " + std::to_string(nbr));
            adj[u].push_back(nbr);
        }
        int extra;
        if (iss >> extra)
            throw std::runtime_error(
                "Mismatched outdegree/neighbour count for vertex " + std::to_string(u) +
                " (expected " + std::to_string(outdegree) + " neighbours, line has more)");
    }

    auto read_tagged_double = [&](const char *expectedTag, double &val) {
        if (!next_nonblank_line(fin, line))
            throw std::runtime_error(std::string("Missing ") + expectedTag + " line");
        std::istringstream iss(line);
        std::string tag;
        if (!(iss >> tag) || tag != expectedTag)
            throw std::runtime_error(std::string("Expected ") + expectedTag + " line");
        if (!(iss >> val))
            throw std::runtime_error(std::string("Malformed ") + expectedTag + " value");
    };
    auto read_tagged_int = [&](const char *expectedTag, int &val) {
        if (!next_nonblank_line(fin, line))
            throw std::runtime_error(std::string("Missing ") + expectedTag + " line");
        std::istringstream iss(line);
        std::string tag;
        if (!(iss >> tag) || tag != expectedTag)
            throw std::runtime_error(std::string("Expected ") + expectedTag + " line");
        if (!(iss >> val))
            throw std::runtime_error(std::string("Malformed ") + expectedTag + " value");
    };

    read_tagged_double("DAMPING", damping);
    if (!(damping > 0.0) || !(damping < 1.0))
        throw std::runtime_error("DAMPING must satisfy 0 < d < 1");

    read_tagged_double("TOLERANCE", tolerance);
    if (!(tolerance > 0.0))
        throw std::runtime_error("TOLERANCE must be positive");

    read_tagged_int("MAX_ITERATIONS", maxIterations);
    if (maxIterations <= 0)
        throw std::runtime_error("MAX_ITERATIONS must be positive");
}
