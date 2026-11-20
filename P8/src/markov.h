// markov.h -- steady state of Markov chains (power method) and n-step comparison
#ifndef MARKOV_H
#define MARKOV_H
#include "common.h"

// Validates P, solves exactly, runs power method, prints n-step table, writes CSVs.
// lazy=true replaces P by (P+I)/2 (same steady state, removes periodicity).
void analyzeMarkov(Mat P, std::vector<double> pi0, bool lazy, const std::string& name);
// File: n, then n*n matrix entries, then optional n initial probabilities.
bool loadMarkov(const std::string& fname, Mat& P, std::vector<double>& pi0);
#endif
