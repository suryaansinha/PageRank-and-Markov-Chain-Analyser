// pagerank.h -- PageRank by the power method on adjacency lists
#ifndef PAGERANK_H
#define PAGERANK_H
#include "graph.h"
#include "common.h"

struct PRResult { std::vector<double> r; int iters; bool converged; double ms; std::vector<double> hist; };

PRResult pagerank(const Graph& g, double d, double tol, int maxIt);   // O(N+E) per iteration
Mat googleMatrix(const Graph& g, double d);                           // dense; small N only (exact check)
void runPageRank(const Graph& g, double d, const std::string& tag);   // prints table, writes CSVs
void dampingStudy(const Graph& g);                                    // iterations/time vs d
#endif
