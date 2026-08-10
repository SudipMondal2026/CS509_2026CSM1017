#ifndef CSR_GRAPH_H
#define CSR_GRAPH_H

#include <vector>
#include <string>
#include <utility>

using AdjEntry = std::pair<int, double>;
using AdjList  = std::vector<std::vector<AdjEntry>>;

struct CSRGraph {
    int V = 0;
    bool weighted = false;
    std::vector<int> row_ptr;   
    std::vector<int> col_idx;  
    std::vector<double> values; 
};

int read_unweighted_adjlist(const std::string &path, int &V, int &E, AdjList &adj);

int read_weighted_adjlist(const std::string &path, int &V, int &E, AdjList &adj);

CSRGraph adjlist_to_csr(const AdjList &adj, bool weighted);

void print_csr(const CSRGraph &g);

#endif
