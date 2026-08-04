CXX = g++
CXXFLAGS = -O2 -std=c++17 -Isrc -Wall
SRC = driver/main.cpp src/gemm_simple.cpp src/gemm_blocking.cpp
TARGET = build/assignment1

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf build
