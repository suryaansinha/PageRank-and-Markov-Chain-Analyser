// main.cpp -- menu, built-in test suite, random graph generator
#include "common.h"
#include "graph.h"
#include "pagerank.h"
#include "markov.h"
#include <cstdlib>
using namespace std;

void testSuite() {
    Mat W = {{0.8,0.15,0.05},{0.4,0.4,0.2},{0.2,0.3,0.5}};
    analyzeMarkov(W, {1,0,0}, false, "TEST 1 weather (sunny/cloudy/rainy), start sunny");
    Mat M = {{0.7,0.2,0.1},{0.1,0.8,0.1},{0.2,0.2,0.6}};
    analyzeMarkov(M, {0.6,0.3,0.1}, false, "TEST 2 market share, 3 brands");
    Mat Two = {{0.9,0.1},{0.5,0.5}};                                // exact answer (5/6, 1/6)
    analyzeMarkov(Two, {0,1}, false, "TEST 3 two-state, exact pi = (5/6, 1/6)");
    Mat Per = {{0,1},{1,0}};
    analyzeMarkov(Per, {1,0}, false, "TEST 4 periodic chain (plain power method)");
    analyzeMarkov(Per, {1,0}, true,  "TEST 4b periodic chain (lazy fix)");
    Graph g; g.init(4);
    g.addEdge(0,1); g.addEdge(0,2); g.addEdge(1,2); g.addEdge(2,0); g.addEdge(3,2);
    runPageRank(g, 0.85, "TEST 5 four-page graph");
    Graph h; h.init(4); h.addEdge(0,1); h.addEdge(0,2); h.addEdge(1,2); h.addEdge(3,2);   // page 2 is dangling
    runPageRank(h, 0.85, "TEST 6 graph with a dangling page");
}

void randomGraphFile(int N, int E, const string& fname) {
    ofstream f(fname.c_str()); srand(12345);
    for (int i = 0; i < E; i++) {
        int u = rand() % N, v = rand() % N;
        if (rand() % 3 == 0) v = rand() % (N / 10 + 1);             // make some pages popular
        f << u << " " << v << "\n";
    }
    out << "  wrote " << E << " random links to " << fname << endl;
}

int main() {
    out.f.open("results.txt");
    out << fixed;
    int ch; string fname;
    do {
        cout << "\n===== P8: PageRank & Markov Chain Analyzer =====\n"
                " 1. PageRank from edge-list file\n 2. Markov chain from matrix file\n"
                " 3. Run built-in test suite\n 4. Damping-factor study (edge-list file)\n"
                " 5. Generate large random graph file\n 0. Exit\nChoice: ";
        if (!(cin >> ch)) { cin.clear(); cin.ignore(1000, '\n'); ch = -1; }
        if (ch == 1 || ch == 4) {
            cout << "Edge-list file: "; cin >> fname; Graph g; string err;
            if (!g.load(fname, err)) { out << "  ERROR: " << err << endl; continue; }
            if (ch == 1) {
                double d; cout << "Damping factor (0<d<1, e.g. 0.85): ";
                if (!(cin >> d) || d <= 0 || d >= 1) { cin.clear(); cin.ignore(1000, '\n'); out << "  ERROR: damping must be in (0,1)" << endl; continue; }
                runPageRank(g, d, fname);
            } else dampingStudy(g);
        } else if (ch == 2) {
            cout << "Matrix file: "; cin >> fname; Mat P; vector<double> pi0;
            if (!loadMarkov(fname, P, pi0)) continue;
            char a; cout << "Use lazy chain (P+I)/2 for periodic chains? (y/n): "; cin >> a;
            analyzeMarkov(P, pi0, a == 'y' || a == 'Y', fname);
        } else if (ch == 3) testSuite();
        else if (ch == 5) {
            int N, E; cout << "Pages and links: ";
            if (cin >> N >> E && N > 10 && E > 0) randomGraphFile(N, E, "samples/random_graph.txt");
            else { cin.clear(); cin.ignore(1000, '\n'); out << "  ERROR: need N>10, E>0" << endl; }
        } else if (ch != 0) cout << "Invalid choice.\n";
    } while (ch != 0);
    cout << "Results saved to results.txt\n";
    return 0;
}
