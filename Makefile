SHELL = /bin/sh
CXX = g++ -std=c++11
CXXFLAGS = -O3
DIAGNOSTICS = -fdiagnostics-color -Wall

all: linear-generator

linear-generator: linear-generator.o sequential.o
	$(CXX) $(DIAGNOSTICS) $^ -o linear-generator

linear-generator.o: linear-generator.cc sequential.h
	$(CXX) -c $(CXXFLAGS) $(DIAGNOSTICS) $<

sequential.o: sequential.cc
	$(CXX) -c $(CXXFLAGS) $(DIAGNOSTICS) $<

clean:
	$(RM) linear-generator *.o
