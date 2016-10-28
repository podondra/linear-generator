SHELL = /bin/sh
CXX = g++
CXXFLAGS = -std=c++11 -Wall -O3

all: sequential

sequential: sequential.cc
	$(CXX) -fdiagnostics-color $(CXXFLAGS) $< -o sequential

clean:
	$(RM) sequential
