#ifndef MST_IO_H
#define MST_IO_H

#include "csr_graph.h"
#include <string>

// Reads the MST weighted-undirected adjacency-list file format described in
// the Assignment 3 spec (Section 5.1):
//
//   V E
//   u0 degree neighbor1 weight1 neighbor2 weight2 ...
//   ...
//
// Unlike the Assignment 2 formats, there is no trailing SOURCE line, so this
// is a small, format-specific reader. The actual adjacency-list -> CSR
// conversion is still done by the *reused, unmodified* adjlist_to_csr()
// helper from Assignment 2 (csr_graph.h / csr_graph.cpp) - it is not
// reimplemented here.
void read_mst_adjlist(const std::string &path, int &V, int &E, AdjList &adj);

#endif
