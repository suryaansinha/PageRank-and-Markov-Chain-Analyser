// pagerank.cpp
#include "pagerank.h"
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iomanip>
using namespace std;

PRResult pagerank(const Graph& g, double d, double tol, int maxIt) {
    PRResult R; int n = g.n; clock_t t0 = clock();
    vector<double> r(n, 1.0 / n), nr(n); R.converged = false; R.iters = 0;
    for (int it = 1; it <= maxIt; it++) {
        double dangling = 0; for (int i = 0; i < n; i++) if (g.nodes[i].outdeg == 0) dangling += r[i];
        fill(nr.begin(), nr.end(), (1.0 - d) / n + d * dangling / n);     // teleport + dangling share
        for (int i = 0; i < n; i++) {
            if (!g.nodes[i].outdeg) continue;
            double share = d * r[i] / g.nodes[i].outdeg;
            for (Edge* e = g.nodes[i].head; e; e = e->next) nr[e->to] += share;   // walk linked list
        }
        double diff = l1(nr, r); r = nr; R.hist.push_back(diff); R.iters = it;
        if (diff < tol) { R.converged = true; break; }
    }
    R.r = r; R.ms = 1000.0 * (clock() - t0) / CLOCKS_PER_SEC; return R;
}

Mat googleMatrix(const Graph& g, double d) {
    int n = g.n; Mat G(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) G[i][j] = (1 - d) / n + (g.nodes[i].outdeg ? 0 : d / n);
        for (Edge* e = g.nodes[i].head; e; e = e->next) G[i][e->to] += d / g.nodes[i].outdeg;
    }
    return G;
}

void runPageRank(const Graph& g, double d, const string& tag) {
    out << "\n=== PageRank [" << tag << "]  N=" << g.n << "  E=" << g.edges << "  d=" << setprecision(2) << d << " ===" << endl;
    PRResult R = pagerank(g, d, 1e-10, 1000);
    if (!R.converged) out << "  WARNING: did not converge in 1000 iterations" << endl;
    out << "  iterations = " << R.iters << ", time = " << fixed << setprecision(3) << R.ms << " ms" << endl;
    vector<int> idx(g.n); for (int i = 0; i < g.n; i++) idx[i] = i;
    sort(idx.begin(), idx.end(), [&](int a, int b) { return R.r[a] > R.r[b]; });
    vector<double> ex; bool haveEx = false;
    if (g.n <= 300) { Mat G = googleMatrix(g, d); haveEx = exactSteady(G, ex); }
    out << "  Rank  Page   PageRank      OutDeg" << (haveEx ? "   Exact(linear solve)  AbsErr" : "") << endl;
    int show = min(g.n, 15);
    for (int k = 0; k < show; k++) {
        int p = idx[k];
        out << "  " << setw(4) << k + 1 << setw(6) << p << setw(14) << setprecision(8) << R.r[p] << setw(9) << g.nodes[p].outdeg;
        if (haveEx) out << setw(18) << ex[p] << setw(12) << scientific << setprecision(2) << fabs(ex[p] - R.r[p]) << fixed;
        out << endl;
    }
    if (g.n > show) out << "  ... (full ranking saved in ranks.csv)" << endl;
    double s = 0; for (int i = 0; i < g.n; i++) s += R.r[i];
    out << "  sum of ranks = " << setprecision(10) << s << endl;
    ofstream rk("ranks.csv"); rk << "rank,page,pagerank\n";
    for (int k = 0; k < g.n; k++) rk << k + 1 << "," << idx[k] << "," << setprecision(12) << R.r[idx[k]] << "\n";
    ofstream cv("pagerank_convergence.csv"); cv << "iteration,l1_diff\n";
    for (size_t i = 0; i < R.hist.size(); i++) cv << i + 1 << "," << R.hist[i] << "\n";
}

void dampingStudy(const Graph& g) {
    out << "\n=== Effect of damping factor ===\n  d       iterations   time(ms)   top page" << endl;
    double ds[] = {0.50, 0.70, 0.85, 0.95, 0.99};
    for (int k = 0; k < 5; k++) {
        PRResult R = pagerank(g, ds[k], 1e-10, 5000);
        int top = max_element(R.r.begin(), R.r.end()) - R.r.begin();
        out << "  " << fixed << setprecision(2) << ds[k] << setw(12) << R.iters << setw(12) << setprecision(3) << R.ms
            << setw(10) << top << (R.converged ? "" : "  (no convergence)") << endl;
    }
}
