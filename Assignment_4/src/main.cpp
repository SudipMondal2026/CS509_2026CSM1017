#include "coloring.h"
#include "csr_graph.h"
#include "graph_io.h"
#include "pagerank.h"
#include <chrono>
#include <cstdio>
#include <iostream>
#include <stdexcept>

using Clock = std::chrono::high_resolution_clock;

static void run_coloring(const std::string &path) {
    int V, E;
    AdjList adj;

    read_coloring_adjlist(path, V, E, adj);
    CSRGraph g = adjlist_to_csr(adj);

    auto t1 = Clock::now();
    ColoringResult result = welsh_powell_coloring(g);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    bool valid = validate_coloring(g, result.colors);

    // --- Untimed: output printing. ---
    std::cout << "Algorithm: Greedy Vertex Coloring\n";
    std::cout << "Vertex colors:\n";
    for (int v = 0; v < g.V; ++v) {
        std::cout << v << " " << result.colors[v] << "\n";
    }
    std::cout << "Colors used: " << result.numColors << "\n";
    std::cout << "Valid coloring: " << (valid ? "true" : "false") << "\n";
    std::cout << "Execution time: " << ms << " ms\n";
}

static void run_pagerank(const std::string &path) {
    int V, E;
    AdjList adj;
    double damping, tolerance;
    int maxIterations;

    read_pagerank_adjlist(path, V, E, adj, damping, tolerance, maxIterations);
    CSRGraph g = adjlist_to_csr(adj);
    auto t1 = Clock::now();
    PageRankResult result = pagerank(g, damping, tolerance, maxIterations);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    double sumRanks = 0.0;
    for (double r : result.ranks) sumRanks += r;

    std::cout << "Algorithm: PageRank\n";
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.6f", damping);
    std::cout << "Damping: " << buf << "\n";
    std::cout << "Vertex ranks:\n";
    for (int v = 0; v < g.V; ++v) {
        std::snprintf(buf, sizeof(buf), "%.6f", result.ranks[v]);
        std::cout << v << " " << buf << "\n";
    }
    std::snprintf(buf, sizeof(buf), "%.6f", sumRanks);
    std::cout << "Sum of ranks: " << buf << "\n";
    std::cout << "Iterations: " << result.iterations << "\n";
    std::cout << "Converged: " << (result.converged ? "true" : "false") << "\n";
    std::cout << "Execution time: " << ms << " ms\n";
}

static void print_usage(const char *prog) {
    std::cerr << "Usage:\n"
              << "  " << prog << " color    <input_file>\n"
              << "  " << prog << " pagerank <input_file>\n";
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    std::string path = argv[2];

    try {
        if (mode == "color") {
            run_coloring(path);
        } else if (mode == "pagerank") {
            run_pagerank(path);
        } else {
            print_usage(argv[0]);
            return 1;
        }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
