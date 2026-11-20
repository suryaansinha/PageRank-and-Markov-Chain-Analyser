// graph.cpp
#include "graph.h"
#include "common.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

Graph::Graph() : n(0), nodes(NULL), edges(0) {}
Graph::~Graph() { clear(); }

void Graph::clear() {
    for (int i = 0; i < n; i++) { Edge* e = nodes[i].head; while (e) { Edge* t = e->next; delete e; e = t; } }
    delete[] nodes; nodes = NULL; n = 0; edges = 0;
}
void Graph::init(int N) { clear(); n = N; nodes = new Node[N]; }

bool Graph::addEdge(int u, int v) {
    if (u == v) return false;
    for (Edge* e = nodes[u].head; e; e = e->next) if (e->to == v) return false;
    Edge* e = new Edge; e->to = v; e->next = nodes[u].head; nodes[u].head = e;   // insert at head
    nodes[u].outdeg++; edges++; return true;
}

bool Graph::load(const string& fname, string& err) {
    ifstream in(fname.c_str());
    if (!in) { err = "cannot open file '" + fname + "'"; return false; }
    vector<pair<int,int> > list; string line; int lineNo = 0, bad = 0, maxId = -1;
    while (getline(in, line)) {
        lineNo++;
        size_t c = line.find('#'); if (c != string::npos) line = line.substr(0, c);
        istringstream ss(line); long u, v; string extra;
        if (!(ss >> u)) {
            if (line.find_first_not_of(" \t\r\n") != string::npos) {   // non-numeric text
                out << "  warning: skipped malformed line " << lineNo << endl; bad++; }
            continue;                                               // (blank lines are ignored)
        }
        if (!(ss >> v) || (ss >> extra) || u < 0 || v < 0 || u > 10000000 || v > 10000000) {
            out << "  warning: skipped malformed line " << lineNo << endl; bad++; continue; }
        list.push_back(make_pair((int)u, (int)v));
        maxId = max(maxId, (int)max(u, v));
    }
    if (list.empty()) { err = "no valid edges found"; return false; }
    init(maxId + 1);
    int dup = 0;
    for (size_t i = 0; i < list.size(); i++) if (!addEdge(list[i].first, list[i].second)) dup++;
    out << "  loaded " << n << " pages, " << edges << " links (" << dup << " duplicate/self links ignored, "
        << bad << " bad lines)" << endl;
    return true;
}
