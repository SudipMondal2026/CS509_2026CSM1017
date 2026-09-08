#ifndef CSR_GRAPH_H
#define CSR_GRAPH_H

#include <vector>

using AdjList = std::vector<std::vector<int>>;

struct CSRGraph {
    int V = 0;
    int E = 0;
    std::vector<int> row_ptr;
    std::vector<int> col_idx;
};

CSRGraph adjlist_to_csr(const AdjList &adj);

#endif
