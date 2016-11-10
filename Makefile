SHELL = /bin/sh
CXX = g++ -std=c++11
DIAGNOSTICS = -fdiagnostics-color -Wall
ARCHITECTURE = -mavx

all: linear-generator

asm: optimize.cc
	$(CXX) -S $(DIAGNOSTICS) -O3 $(ARCHITECTURE) $< \
	    -fopt-info-vec-all=vec.all

clang: optimize.cc
	clang++ -std=c++11 -O3 -S $< \
	    -Rpass=loop-vectorize \
	    -Rpass-missed=loop-vectorize \
	    -Rpass-analysis=loop-vectorize

linear-generator: linear-generator.o sequential.o optimize-wrapper.o optimize.o
	$(CXX) $(DIAGNOSTICS) $^ -o linear-generator

linear-generator.o: linear-generator.cc sequential.h optimize-wrapper.h
	$(CXX) -c -O3 $(DIAGNOSTICS) $<

sequential.o: sequential.cc
	$(CXX) -c -O3 $(DIAGNOSTICS) $<

optimize-wrapper.o: optimize-wrapper.cc optimize.h
	$(CXX) -c $(DIAGNOSTICS) -O3 $<

optimize.o: optimize.cc
	$(CXX) -c $(DIAGNOSTICS) -O3 $(ARCHITECTURE) $< \
	    -fopt-info-vec-optimized

clean:
	$(RM) linear-generator *.o *.s vec.all
