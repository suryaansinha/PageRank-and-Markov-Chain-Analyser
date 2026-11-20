// common.cpp
#include "common.h"
#include <cmath>
#include <algorithm>
using namespace std;

Tee out;

double l1(const vector<double>& a, const vector<double>& b) {
    double s = 0; for (size_t i = 0; i < a.size(); i++) s += fabs(a[i] - b[i]); return s; }

vector<double> vecMat(const vector<double>& x, const Mat& P) {
    int n = x.size(); vector<double> r(n, 0.0);
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) r[j] += x[i] * P[i][j];
    return r; }

// Solve pi(P-I)=0 with sum(pi)=1 (last equation replaced by the normalisation row)
bool exactSteady(const Mat& P, vector<double>& pi) {
    int n = P.size(); Mat A(n, vector<double>(n + 1, 0.0));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) A[i][j] = P[j][i] - (i == j ? 1.0 : 0.0);
    for (int j = 0; j < n; j++) A[n-1][j] = 1.0;
    A[n-1][n] = 1.0;
    for (int c = 0; c < n; c++) {
        int p = c; for (int r = c + 1; r < n; r++) if (fabs(A[r][c]) > fabs(A[p][c])) p = r;
        if (fabs(A[p][c]) < 1e-12) return false;                       // singular
        swap(A[p], A[c]);
        for (int r = 0; r < n; r++) if (r != c) { double f = A[r][c] / A[c][c];
            for (int k = c; k <= n; k++) A[r][k] -= f * A[c][k]; }
    }
    pi.assign(n, 0.0); for (int i = 0; i < n; i++) pi[i] = A[i][n] / A[i][i];
    return true;
}

bool validateStochastic(const Mat& P, string& err) {
    for (size_t i = 0; i < P.size(); i++) {
        double s = 0;
        for (size_t j = 0; j < P.size(); j++) {
            if (P[i][j] < 0) { err = "negative probability in row " + to_string(i+1); return false; }
            s += P[i][j]; }
        if (fabs(s - 1.0) > 1e-6) { err = "row " + to_string(i+1) + " sums to " + to_string(s) + ", not 1"; return false; }
    }
    return true;
}
