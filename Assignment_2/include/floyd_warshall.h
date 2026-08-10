#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <vector>
#include <string>
#include <limits>

// Dense V x V distance/adjacency matrix. matrix[i][j] = direct edge weight
// from i to j, or FW_INF if no direct edge. matrix[i][i] == 0.
using Matrix = std::vector<std::vector<double>>;

constexpr double FW_INF = std::numeric_limits<double>::infinity();

struct FloydWarshallResult {
    Matrix dist;                 // all-pairs shortest distances; empty if negative_cycle
    bool negative_cycle = false; // true if a negative-weight cycle was detected
};

// Reads a dense adjacency-matrix file (Assignment 2, Section 6.1):
//   V
//   row 0 values (V space-separated entries: an integer, or the literal "INF")
//   ...
//   row (V-1) values
// Validates that V > 0 and that every diagonal entry is 0.
// Floyd-Warshall is exempt from the CSR pipeline: the matrix is read directly.
void read_adjmatrix(const std::string &path, int &V, Matrix &matrix);

// Floyd-Warshall all-pairs shortest path via dynamic programming:
// for every intermediate vertex k, dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]).
// O(V^3) time, O(V^2) memory.
// After the DP completes, checks the diagonal: a negative dist[i][i] means a
// negative-weight cycle exists, in which case negative_cycle is set to true
// and dist is left empty (distances are undefined).
FloydWarshallResult floyd_warshall(const Matrix &matrix);

#endif
