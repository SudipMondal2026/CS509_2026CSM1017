#include "csr_graph.h"
#include "mst.h"
#include "mst_io.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>

using Clock = std::chrono::high_resolution_clock;

static std::string format_weight(double w) {
    if (std::floor(w) == w) {
        return std::to_string(static_cast<long long>(w));
    }
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.6f", w);
    return std::string(buf);
}

static void print_mst_result(const std::string &label, const MSTResult &r, double ms, int V) {
    std::cout << "Algorithm: " << label << "\n";
    std::cout << "MST edges:\n";
    for (const auto &e : r.edges) {
        std::cout << e.u << " " << e.v << " " << format_weight(e.weight) << "\n";
    }
    if (!r.spans_all_vertices) {
        std::cout << "WARNING: only " << r.edges.size() << " of " << (V - 1)
                   << " required MST edges were selected - the input graph is not connected.\n";
    }
    std::cout << "Total MST weight: " << format_weight(r.total_weight) << "\n";
    std::cout << "Execution time: " << ms << " ms\n";
}

static void run_mst(const std::string &path, const std::string &mode) {
    int V, E;
    AdjList adj;

    read_mst_adjlist(path, V, E, adj);
    CSRGraph g = adjlist_to_csr(adj, /*weighted=*/true);

    bool wantKruskal = (mode == "kruskal" || mode == "both");
    bool wantPrim = (mode == "prim" || mode == "both");

    MSTResult kruskalResult, primResult;
    double kruskalMs = 0.0, primMs = 0.0;

    if (wantKruskal) {
        auto t1 = Clock::now();
        kruskalResult = kruskal_mst(g);
        auto t2 = Clock::now();
        kruskalMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
    }

    if (wantPrim) {
        auto t1 = Clock::now();
        primResult = prim_mst(g, /*start=*/0);
        auto t2 = Clock::now();
        primMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
    }

    if (wantKruskal) {
        print_mst_result("Kruskal's MST", kruskalResult, kruskalMs, V);
    }
    if (wantKruskal && wantPrim) std::cout << "\n";
    if (wantPrim) {
        print_mst_result("Prim's MST", primResult, primMs, V);
    }

    if (wantKruskal && wantPrim) {
        std::cout << "\nEqual total weight (Kruskal vs Prim): "
                   << (kruskalResult.total_weight == primResult.total_weight ? "Yes" : "No")
                   << "\n";
        std::cout << "Kruskal time: " << kruskalMs << " ms | Prim time: " << primMs << " ms\n";
    }
}

static void print_usage(const char *prog) {
    std::cerr << "Usage:\n"
              << "  " << prog << " kruskal <input_file>\n"
              << "  " << prog << " prim    <input_file>\n"
              << "  " << prog << " both    <input_file>   (runs Kruskal then Prim on the same graph)\n";
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    std::string path = argv[2];

    if (mode != "kruskal" && mode != "prim" && mode != "both") {
        print_usage(argv[0]);
        return 1;
    }

    try {
        run_mst(path, mode);
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
