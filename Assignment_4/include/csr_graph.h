#ifndef CSR_GRAPH_H
#define CSR_GRAPH_H

#include <vector>

// Plain adjacency-list representation built directly from a text input file.
// For an undirected graph (Vertex Coloring) every edge must already appear
// in BOTH endpoints' lists before conversion. For a directed graph
// (PageRank) only outgoing edges are listed per vertex.
using AdjList = std::vector<std::vector<int>>;

// Compressed Sparse Row representation of an unweighted graph.
//   row_ptr[u] .. row_ptr[u+1]-1   -> range of col_idx holding u's neighbours
//   col_idx[i]                    -> neighbour vertex id
// `values` is intentionally omitted: both Vertex Coloring and PageRank use
// unweighted graphs (Assignment 4 spec, Section 4.1), so no edge weights are
// stored.
struct CSRGraph {
    int V = 0;
    int E = 0; // number of directed entries actually stored in col_idx
    std::vector<int> row_ptr;
    std::vector<int> col_idx;
};

// Converts an adjacency list into CSR format. This is pure preprocessing:
// per the spec's timing rule (Section 9), its cost must NEVER be included
// inside a measured algorithm-execution time.
//
// This is the same conversion routine used in Assignments 2 and 3. Per the
// spec ("You are expected to call the CSR conversion function from the
// previous assignment. DO NOT copy the code into this assignment."), a real
// submission should compile this file straight out of its own previous
// assignment folder (see the Makefile's ASGX_DIR comment) rather than
// keeping a second copy here.
CSRGraph adjlist_to_csr(const AdjList &adj);

#endif
