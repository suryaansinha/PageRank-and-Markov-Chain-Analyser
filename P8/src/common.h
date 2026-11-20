// common.h -- shared types and numerical helpers (used by all modules)
#ifndef COMMON_H
#define COMMON_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

typedef std::vector<std::vector<double> > Mat;

// Tee: everything written to `out` goes to the console AND to results.txt
struct Tee {
    std::ofstream f;
    template <class T> Tee& operator<<(const T& v) { std::cout << v; if (f.is_open()) f << v; return *this; }
    Tee& operator<<(std::ostream& (*m)(std::ostream&)) { m(std::cout); if (f.is_open()) m(f); return *this; }
};
extern Tee out;

double l1(const std::vector<double>& a, const std::vector<double>& b);               // L1 distance
std::vector<double> vecMat(const std::vector<double>& x, const Mat& P);              // row vector * matrix
bool exactSteady(const Mat& P, std::vector<double>& pi);                             // exact pi via Gaussian elimination
bool validateStochastic(const Mat& P, std::string& err);                             // rows >= 0 and sum to 1
#endif
