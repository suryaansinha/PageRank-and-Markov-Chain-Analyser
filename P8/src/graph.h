// graph.h -- linked-list (adjacency list) graph and edge-list file input
#ifndef GRAPH_H
#define GRAPH_H
#include <string>

struct Edge { int to; Edge* next; };                       // one outgoing link
struct Node { Edge* head; int outdeg; Node() : head(NULL), outdeg(0) {} };

class Graph {
public:
    int n; Node* nodes; long edges;
    Graph();
    ~Graph();
    void clear();                                          // frees every list node
    void init(int N);                                      // N empty pages
    bool addEdge(int u, int v);                            // false if duplicate or self-loop
    bool load(const std::string& fname, std::string& err);// file lines: "from to", '#' = comment
private:
    Graph(const Graph&);                                   // non-copyable (owns raw pointers)
    Graph& operator=(const Graph&);
};
#endif
