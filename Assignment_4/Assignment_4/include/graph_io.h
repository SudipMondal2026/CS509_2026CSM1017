#ifndef GRAPH_IO_H
#define GRAPH_IO_H

#include "csr_graph.h"
#include <string>

// Reads the Vertex Coloring input format (Assignment 4 spec, Section 5.1):
//
//   V E
//   u0 degree neighbor1 neighbor2 ...
//   ...
//   u(V-1) degree neighbor1 neighbor2 ...
//
// The graph is undirected and unweighted. Validation performed here (per
// Section 12):
//   - malformed header / adjacency lines
//   - out-of-range vertex ids
//   - self-loops (rejected)
//   - mismatched degree/neighbour count
// Isolated vertices (degree 0) are allowed and valid for Vertex Coloring.
void read_coloring_adjlist(const std::string &path, int &V, int &E, AdjList &adj);

// Reads the PageRank input format (Assignment 4 spec, Section 6.1):
//
//   V E
//   u0 outdegree neighbor1 neighbor2 ...
//   ...
//   u(V-1) outdegree neighbor1 neighbor2 ...
//   DAMPING d
//   TOLERANCE epsilon
//   MAX_ITERATIONS n
//
// The graph is directed and unweighted; only outgoing edges are listed.
// Validation performed here (per Section 12):
//   - malformed header / adjacency lines / trailer lines
//   - out-of-range vertex ids
//   - mismatched outdegree/neighbour count
//   - damping <= 0 or damping >= 1
//   - tolerance <= 0
//   - MAX_ITERATIONS <= 0
void read_pagerank_adjlist(const std::string &path, int &V, int &E, AdjList &adj,
                            double &damping, double &tolerance, int &maxIterations);

#endif
