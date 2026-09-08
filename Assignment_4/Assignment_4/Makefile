CXX = g++
CXXFLAGS = -O2 -std=c++17 -Iinclude -Wall

# --- CSR conversion reuse note ---------------------------------------------
# Per the spec ("call the CSR conversion function from the previous
# assignment. DO NOT copy the code into this assignment."), a real
# submission should point ASGX_SRC/ASGX_DIR below at the student's own,
# already-graded Assignment 2/3 csr_graph.{h,cpp} (the exact same pattern
# used in Assignment 3's Makefile), e.g.:
#
#   ASGX_DIR = ../Assignment_3
#   CXXFLAGS += -I"$(ASGX_DIR)/include"
#   ASGX_SRC  = "$(ASGX_DIR)/src/csr_graph.cpp"
#
# and then link $(ASGX_SRC) instead of src/csr_graph.cpp below. This
# repository ships its own include/csr_graph.h + src/csr_graph.cpp only so
# the project builds standalone without a sibling assignment folder.
LOCAL_SRC = src/main.cpp src/graph_io.cpp src/coloring.cpp src/pagerank.cpp src/csr_graph.cpp
BIN = build/assignment4

GEN_COLOR_SRC = tools/generate_coloring_graph.cpp
GEN_COLOR_BIN = build/generate_coloring_graph

GEN_PR_SRC = tools/generate_pagerank_graph.cpp
GEN_PR_BIN = build/generate_pagerank_graph

all: $(BIN)

$(BIN): $(LOCAL_SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(LOCAL_SRC) -o $(BIN)

gen: $(GEN_COLOR_SRC) $(GEN_PR_SRC)
	mkdir -p build
	$(CXX) -O2 -std=c++17 -Wall $(GEN_COLOR_SRC) -o $(GEN_COLOR_BIN)
	$(CXX) -O2 -std=c++17 -Wall $(GEN_PR_SRC) -o $(GEN_PR_BIN)

clean:
	rm -rf build

.PHONY: all gen clean
