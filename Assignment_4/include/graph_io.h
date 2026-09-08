#ifndef GRAPH_IO_H
#define GRAPH_IO_H

#include "csr_graph.h"
#include <string>

void read_coloring_adjlist(const std::string &path, int &V, int &E, AdjList &adj);

void read_pagerank_adjlist(const std::string &path, int &V, int &E, AdjList &adj,double &damping, double &tolerance, int &maxIterations);

#endif
