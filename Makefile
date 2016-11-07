SHELL = /bin/sh
CXX = g++ -std=c++11
DIAGNOSTICS = -fdiagnostics-color -Wall

all: linear-generator

linear-generator: linear-generator.o sequential.o optimize.o
	$(CXX) $(DIAGNOSTICS) $^ -o linear-generator

linear-generator.o: linear-generator.cc sequential.h optimize.h
	$(CXX) -c -O3 $(DIAGNOSTICS) $<

sequential.o: sequential.cc
	$(CXX) -c -O3 $(DIAGNOSTICS) $<

optimize.o: optimize.cc
	$(CXX) -c $(DIAGNOSTICS) -O3 -march=native -fopt-info-vec-all \
	    $<

clean:
	$(RM) linear-generator *.o *.s
