CXX = g++
# CSR conversion is REUSED directly from Assignment 2 - not copied. The
# header and the .cpp are compiled straight from Assignment_2(single)/, via
# a relative path, per the spec: "call the CSR conversion function from the
# previous assignment. DO NOT copy the code into this assignment."
ASG2_DIR = ../Assignment_2(single)
CXXFLAGS = -O2 -std=c++17 -Iinclude -I"$(ASG2_DIR)/include" -Wall

# Prerequisites only list LOCAL sources (paren-free paths) so make's
# dependency parser doesn't choke; the Assignment 2 file is referenced,
# quoted, only inside the recipe text below.
LOCAL_SRC = src/main.cpp src/mst_io.cpp src/mst_kruskal.cpp src/mst_prim.cpp
ASG2_SRC = "$(ASG2_DIR)/src/csr_graph.cpp"
BIN = build/assignment3

GEN_SRC = tools/generate_mst_graph.cpp
GEN_BIN = build/generate_mst_graph

all: $(BIN)

$(BIN): $(LOCAL_SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(LOCAL_SRC) $(ASG2_SRC) -o $(BIN)

gen: $(GEN_SRC)
	mkdir -p build
	$(CXX) -O2 -std=c++17 -Wall $(GEN_SRC) -o $(GEN_BIN)

clean:
	rm -rf build

.PHONY: all gen clean
