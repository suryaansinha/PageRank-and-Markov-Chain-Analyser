// markov.cpp
#include "markov.h"
#include <cmath>
#include <ctime>
#include <iomanip>
using namespace std;

void analyzeMarkov(Mat P, vector<double> pi0, bool lazy, const string& name) {
    int n = P.size(); string err;
    out << "\n=== Markov chain [" << name << "]  states=" << n << (lazy ? "  (lazy chain (P+I)/2)" : "") << " ===" << endl;
    if (!validateStochastic(P, err)) { out << "  ERROR: invalid matrix: " << err << endl; return; }
    double s0 = 0;
    for (int i = 0; i < n; i++) { if (pi0[i] < 0) { out << "  ERROR: negative initial probability" << endl; return; } s0 += pi0[i]; }
    if (fabs(s0 - 1) > 1e-6) { out << "  ERROR: initial distribution sums to " << s0 << endl; return; }
    if (lazy) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) P[i][j] = 0.5 * P[i][j] + (i == j ? 0.5 : 0.0);

    vector<double> ex; bool haveEx = exactSteady(P, ex);
    if (!haveEx) out << "  note: exact solve failed (singular system: chain is reducible, no unique steady state)" << endl;

    // ---- power method ----
    clock_t t0 = clock(); vector<double> x = pi0; int it = 0; bool conv = false; vector<double> hist;
    for (it = 1; it <= 100000; it++) {
        vector<double> y = vecMat(x, P); double sum = 0; for (int i = 0; i < n; i++) sum += y[i];
        for (int i = 0; i < n; i++) y[i] /= sum;
        double d = l1(x, y); x = y; hist.push_back(d);
        if (d < 1e-12) { conv = true; break; }
    }
    double ms = 1000.0 * (clock() - t0) / CLOCKS_PER_SEC;
    if (!conv) {
        out << "  Power method did NOT converge in 100000 iterations (periodic or reducible chain)." << endl
            << "  Fix: rerun with the lazy-chain option (same steady state, aperiodic)." << endl;
    } else out << "  Power method converged in " << it << " iterations (" << setprecision(3) << ms << " ms)" << endl;

    out << "  State   Power-method   Exact   AbsErr" << endl;
    for (int i = 0; i < n; i++) {
        out << "  " << setw(5) << i + 1 << setw(15) << setprecision(8) << x[i];
        if (haveEx) out << setw(12) << ex[i] << setw(12) << scientific << setprecision(2) << fabs(ex[i] - x[i]) << fixed;
        out << endl;
    }

    // ---- n-step comparison ----
    out << "\n  n-step distribution  pi0*P^n  vs steady state" << endl << "  n      ";
    for (int i = 0; i < n; i++) out << "state" << i + 1 << "      ";
    out << "L1 error vs exact steady state" << endl;
    vector<double> v = pi0; int marks[] = {1,2,3,5,10,20,50,100,200}; int mi = 0;
    ofstream cv("markov_nstep.csv"); cv << "n,l1_error\n";
    for (int step = 1; step <= 200; step++) {
        v = vecMat(v, P); double e = haveEx ? l1(v, ex) : l1(v, x); cv << step << "," << e << "\n";
        if (mi < 9 && step == marks[mi]) {
            out << "  " << setw(4) << step << "   ";
            for (int i = 0; i < n; i++) out << setw(10) << setprecision(6) << v[i] << "  ";
            out << scientific << setprecision(3) << e << fixed << endl; mi++;
        }
    }
    ofstream ch("markov_power_convergence.csv"); ch << "iteration,l1_diff\n";
    for (size_t i = 0; i < hist.size() && i < 5000; i++) ch << i + 1 << "," << hist[i] << "\n";
}

bool loadMarkov(const string& fname, Mat& P, vector<double>& pi0) {
    ifstream in(fname.c_str()); if (!in) { out << "  ERROR: cannot open '" << fname << "'" << endl; return false; }
    int n; if (!(in >> n) || n < 1 || n > 500) { out << "  ERROR: bad state count" << endl; return false; }
    P.assign(n, vector<double>(n));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
        if (!(in >> P[i][j])) { out << "  ERROR: matrix incomplete or non-numeric" << endl; return false; }
    pi0.assign(n, 1.0 / n);
    double t; vector<double> tmp; while (tmp.size() < (size_t)n && (in >> t)) tmp.push_back(t);
    if (tmp.size() == (size_t)n) pi0 = tmp;
    return true;
}
