# Build: make   |  Run: make run   |  Clean: make clean
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SRC = src/main.cpp src/common.cpp src/graph.cpp src/pagerank.cpp src/markov.cpp
p8: $(SRC) src/common.h src/graph.h src/pagerank.h src/markov.h
	$(CXX) $(CXXFLAGS) -o p8 $(SRC)
run: p8
	./p8
clean:
	rm -f p8 results.txt *.csv
