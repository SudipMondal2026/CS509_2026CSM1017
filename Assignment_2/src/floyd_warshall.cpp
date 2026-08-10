#include "floyd_warshall.h"
#include <fstream>
#include <stdexcept>

void read_adjmatrix(const std::string &path, int &V, Matrix &matrix) {
    std::ifstream fin(path);
    if (!fin.is_open())
        throw std::runtime_error("Could not open input file: " + path);

    if (!(fin >> V))
        throw std::runtime_error("Malformed header line (expected: V)");
    if (V <= 0)
        throw std::runtime_error("V must be positive");

    matrix.assign(V, std::vector<double>(V, 0.0));

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            std::string tok;
            if (!(fin >> tok))
                throw std::runtime_error("Malformed matrix row " + std::to_string(i));
            if (tok == "INF") {
                matrix[i][j] = FW_INF;
            } else {
                try {
                    matrix[i][j] = std::stod(tok);
                } catch (...) {
                    throw std::runtime_error("Invalid matrix entry at (" + std::to_string(i) +
                                              "," + std::to_string(j) + "): " + tok);
                }
            }
        }
    }

    for (int i = 0; i < V; ++i) {
        if (matrix[i][i] != 0.0)
            throw std::runtime_error("Diagonal entry (" + std::to_string(i) + "," +
                                      std::to_string(i) + ") must be 0");
    }
}

FloydWarshallResult floyd_warshall(const Matrix &matrix) {
    int V = static_cast<int>(matrix.size());
    FloydWarshallResult result;
    result.dist = matrix; // start from the direct-edge matrix, refine in place

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            double dik = result.dist[i][k];
            if (dik == FW_INF) continue; // no path i -> k, nothing to relax through k
            for (int j = 0; j < V; ++j) {
                double dkj = result.dist[k][j];
                if (dkj == FW_INF) continue; // no path k -> j
                double via_k = dik + dkj;
                if (via_k < result.dist[i][j]) {
                    result.dist[i][j] = via_k;
                }
            }
        }
    }

    // Negative-cycle check (Section 6.3): after the DP, dist[i][i] is the
    // shortest cycle length through i. A negative diagonal entry means a
    // negative-weight cycle exists, so all distances are undefined.
    for (int i = 0; i < V; ++i) {
        if (result.dist[i][i] < 0.0) {
            result.negative_cycle = true;
            break;
        }
    }

    if (result.negative_cycle) {
        result.dist.clear();
    }

    return result;
}
