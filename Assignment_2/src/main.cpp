#include "csr_graph.h"
#include "bellman_ford.h"
#include "floyd_warshall.h"
#include <iostream>
#include <chrono>
#include <stdexcept>

using Clock = std::chrono::high_resolution_clock;

static void run_bellman_ford(const std::string &path) {
    int V, E;
    AdjList adj;

    int source = read_weighted_adjlist(path, V, E, adj);
    CSRGraph g = adjlist_to_csr(adj, /*weighted=*/true);

    auto t1 = Clock::now();
    BellmanFordResult r = bellmanford(g, source);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Algorithm: Bellman Ford\n";
    std::cout << "Source: " << source << "\n";
    std::cout << "Vertex Distance\n";
    for (int v = 0; v < V; ++v) {
        std::cout << v << " ";
        if (r.distance[v] == BF_INF) std::cout << "INF\n";
        else std::cout << r.distance[v] << "\n";
    }
    std::cout << "Execution time: " << ms << " ms\n";
}

static void run_floyd_warshall(const std::string &path) {
    int V;
    Matrix matrix;

    read_adjmatrix(path, V, matrix);

    auto t1 = Clock::now();
    FloydWarshallResult r = floyd_warshall(matrix);
    auto t2 = Clock::now();
    double ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    std::cout << "Algorithm: Floyd-Warshall\n";
    if (r.negative_cycle) {
        std::cout << "Negative cycle: true\n";
    } else {
        std::cout << "Distance matrix:\n";
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (r.dist[i][j] == FW_INF) std::cout << "INF";
                else std::cout << r.dist[i][j];
                std::cout << (j + 1 < V ? " " : "\n");
            }
        }
        std::cout << "Negative cycle: none\n";
    }
    std::cout << "Execution time: " << ms << " ms\n";
}

static void print_usage(const char *prog) {
    std::cerr << "Usage:\n"
              << "  " << prog << " bellmanford  <input_file>\n"
              << "  " << prog << " floydwarshall  <input_file>\n";
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    std::string path = argv[2];

    try {
        if (mode == "bellmanford")       run_bellman_ford(path);
        else if (mode == "floydwarshall")  run_floyd_warshall(path);
        else { print_usage(argv[0]); return 1; }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
