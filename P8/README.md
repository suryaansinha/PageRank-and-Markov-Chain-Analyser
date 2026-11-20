# P8 - PageRank and Markov Chain Analyzer (Power Method)

Group No: ____   Project: P8   Course outcome: CO3   Language: C++ (standard headers only)
Submission name: `Group<No>_P8` (source .cpp/.h + sample input files + report PDF)

## 1. Build and run

    make          # or: g++ -std=c++11 -Wall -O2 -o p8 src/*.cpp
    ./p8          # run from this folder so the samples/... paths work

Menu: 1 PageRank from file | 2 Markov chain from file | 3 built-in test suite | 4 damping study | 5 random graph generator | 0 exit

## 2. Input formats
- Edge list: one `from to` pair per line, 0-based page ids, `#` starts a comment.
- Markov file: n, then the n x n matrix, then (optional) n initial probabilities.

## 3. Output files (written to the current folder)
`results.txt` (everything shown on the console), `ranks.csv`, `pagerank_convergence.csv`,
`markov_nstep.csv`, `markov_power_convergence.csv` (plot these in Excel/gnuplot for the report).

## 4. Folder layout
    src/         main.cpp  common.h/.cpp  graph.h/.cpp  pagerank.h/.cpp  markov.h/.cpp
    samples/     graph4.txt graph_dangling.txt weather.txt market.txt periodic.txt bad_matrix.txt
    flowcharts/  fc1..fc4 (.dot source, .png, .svg)
    Makefile  README.md

## 5. Division of work (5 members)

Every member owns one role and must also write and be able to explain some code
(guideline: "every member must have written some code" and "must know the whole project").

| # | Role (from guidelines) | Member name / roll no. | Code owned | Other duties |
|---|---|---|---|---|
| 1 | Group leader and integration | ____________ | `main.cpp` (menu loop, input prompts, `randomGraphFile`), header interfaces (`*.h`), `Makefile` | Merge everyone's code, keep the project compiling with `g++` without errors, final regression run of all menu options, collect the contribution table |
| 2 | Numerical method coding | ____________ | `pagerank()` in `pagerank.cpp` (power iteration, damping, dangling-node mass); the power-method loop and lazy-chain step `(P+I)/2` in `analyzeMarkov()` | Derive and write the theory section (power method, convergence rate, damping) |
| 3 | Input/output, file handling, ASCII/CSV output | ____________ | `graph.cpp` (`Graph::load`, `addEdge`, destructor), `loadMarkov()`, `Tee` in `common.h`, the table printing and CSV writing in `runPageRank()` / `analyzeMarkov()` | Prepare `samples/` files, optional ASCII convergence plot |
| 4 | Testing, error analysis and comparison | ____________ | `exactSteady()` and `validateStochastic()` in `common.cpp`, `googleMatrix()`, the n-step table (pi0*P^n) in `analyzeMarkov()`, `testSuite()` in `main.cpp` | Error tables (power method vs exact vs n-step), error-handling test cases (bad file, bad matrix, no convergence) |
| 5 | Report and demo slides | ____________ | `dampingStudy()` in `pagerank.cpp`; gnuplot/Excel plotting scripts for the CSV files | 8-12 page report (see section 8), flowcharts, slides for the 10-minute demo, the one-page contribution table |

Rules for the group:
- Each person commits code they wrote themselves; change names, comments and structure so the code is the group's own work.
- Members 2 and 4 review each other's numerical code; members 1 and 3 review each other's I/O code.
- Member 5 learns the algorithms by reading Figs 3-4 and running every test case.

## 6. Module map

| File | Contents |
|---|---|
| `common.h/.cpp` | `Tee` output (console + results.txt), `l1()`, `vecMat()`, `exactSteady()`, `validateStochastic()` |
| `graph.h/.cpp` | `Edge`/`Node` linked lists, `Graph` class, edge-list file input |
| `pagerank.h/.cpp` | `pagerank()`, `googleMatrix()`, `runPageRank()`, `dampingStudy()` |
| `markov.h/.cpp` | `analyzeMarkov()` (power method + n-step), `loadMarkov()` |
| `main.cpp` | menu, built-in tests, random graph generator |

## 7. Flowcharts (put these in the report)
Fig 1 overall program (`fc1_overall_program.png`), Fig 2 file input and linked lists (`fc2_file_input.png`),
Fig 3 PageRank (`fc3_pagerank.png`), Fig 4 Markov and n-step (`fc4_markov.png`).
Regenerate: `dot -Tpng -Gdpi=170 flowcharts/fc3_pagerank.dot -o flowcharts/fc3_pagerank.png`

## 8. Guideline checklist (tick before submission)

| Requirement | Where it is covered | Done |
|---|---|---|
| C/C++ only, no solver libraries, standard headers | all `src/` files | [ ] |
| Menu-driven console program | `main.cpp` | [ ] |
| Method in separate functions, header files | `pagerank.*`, `markov.*`, `graph.*`, `common.*` | [ ] |
| At least 3 test cases, one with exact answer | menu 3: two-state chain with exact pi = (5/6, 1/6) | [ ] |
| Input validation and error handling | bad file, bad lines, bad matrix, non-convergence, singular system | [ ] |
| Table on console and saved to file | `results.txt` + CSV files | [ ] |
| Graphs | CSV plotted in Excel/gnuplot (convergence, n-step error, damping) | [ ] |
| Analysis: error vs exact; iterations/accuracy/time | printed in each table; damping study | [ ] |
| Report 8-12 pages: problem, theory, algorithm/flowchart, source code, sample I/O, plots, analysis, conclusion, references | member 5 (with inputs from all) | [ ] |
| 10-minute demo, every member can explain the code | see section 9 | [ ] |
| One-page contribution table (who coded/tested/wrote what) | section 10 | [ ] |
| Compiles with gcc/g++ without errors; file name `Group<No>_P8` | member 1 | [ ] |

Marking: method correctness 40%, working program and testing 25%, analysis and output 15%, report 10%, demo and viva 10%.

## 9. Demo plan (10 minutes) and viva topics

| Time | Speaker | Content |
|---|---|---|
| 0-1 min | 5 | Problem statement, what P8 asks for |
| 1-3 min | 2 | Power method, PageRank formula, damping and dangling pages (Fig 3) |
| 3-5 min | 3 | Edge-list file, linked lists, live run of menu 1 on `graph4.txt` and `graph_dangling.txt` (Fig 2) |
| 5-7 min | 2 and 4 | Markov steady state, n-step comparison, live run of menu 2 on `weather.txt` (Fig 4) |
| 7-9 min | 4 | Test suite (menu 3), exact answer, error handling (`bad_matrix.txt`, missing file), periodic chain and the lazy fix |
| 9-10 min | 1 | Damping study, 3000-page graph timing (menu 5 then 4), conclusion |

Likely questions (anyone may be asked): why does the power method converge and what sets the rate (|lambda2|)?
Why add teleportation (1-d)/N? What happens to a dangling page? Why linked lists instead of an N x N matrix
(O(N+E) vs O(N^2))? Why does `[[0,1],[1,0]]` fail and how does (P+I)/2 fix it? How is the exact answer computed
(Gaussian elimination, partial pivoting)? Why does pi0*P^n approach the steady state, and how fast?

## 10. Contribution table (one page, submit with the report)

| Member | Roll no. | Coded | Tested | Wrote (report/slides) | Estimated % |
|---|---|---|---|---|---|
| 1 | | main.cpp, headers, Makefile | integration runs | report: introduction, conclusion | |
| 2 | | pagerank(), power loop, lazy chain | convergence checks | report: theory section | |
| 3 | | graph.cpp, loadMarkov, Tee, CSV output | file-input and bad-file tests | report: input/output description, sample I/O | |
| 4 | | exactSteady, validateStochastic, n-step, testSuite | all test cases, error analysis | report: result analysis | |
| 5 | | dampingStudy, plotting scripts | plots check | report layout, flowcharts, slides | |

Edit this table to match what each member actually did; marks within a group may differ by contribution.

## 11. Suggested timeline
1. Day 1-2: everyone builds and runs the starter; each member reads their files and traces one example by hand.
2. Day 3-5: members rewrite/extend their own parts (comments, names, extra checks); member 1 merges.
3. Day 6-7: member 4 runs all tests and fills the error tables; member 5 makes plots.
4. Day 8-9: report draft, flowcharts, slides; cross-review of code.
5. Day 10: full dry run of the 10-minute demo with questions from other members.
