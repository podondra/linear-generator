SHELL = /bin/sh
CXX = g++
CXXFLAGS = -Wall -O3

all: sequential.cc
	$(CXX) -std=c++11 -fdiagnostics-color $(CXXFLAGS) $<

run: all
	./a.out
