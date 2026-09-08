#ifndef MST_IO_H
#define MST_IO_H

#include "csr_graph.h"
#include <string>

void read_mst_adjlist(const std::string &path, int &V, int &E, AdjList &adj);

#endif
