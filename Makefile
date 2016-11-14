SHELL = /bin/sh
CXX = g++ -std=c++11
DIAGNOSTICS = -fdiagnostics-color -Wall
ARCHITECTURE = -mavx

all: linear-generator

linear-generator: linear-generator.o sequential.o optimize-wrapper.o \
    optimize.o random.o
	$(CXX) $(DIAGNOSTICS) $^ -o linear-generator

linear-generator.o: src/linear-generator.cc src/sequential.h \
    src/optimize-wrapper.h
	$(CXX) -c $(DIAGNOSTICS) -O3 $<

random.o: src/random.cc
	$(CXX) -c $(DIAGNOSTICS) -O3 $<

sequential.o: src/sequential.cc src/random.h
	$(CXX) -c $(DIAGNOSTICS) -O3 $<

optimize-wrapper.o: src/optimize-wrapper.cc src/optimize.h src/random.h
	$(CXX) -c $(DIAGNOSTICS) -O3 $<

optimize.o: src/optimize.cc
	$(CXX) -c $(DIAGNOSTICS) -O3 $(ARCHITECTURE) $< \
	    -fopt-info-vec-optimized

asm: src/optimize.cc
	$(CXX) -S $(DIAGNOSTICS) -O3 $(ARCHITECTURE) $< \
	    -fopt-info-vec-all=vec.all

clang: src/optimize.cc
	clang++ -std=c++11 -O3 -S $< \
	    -Rpass=loop-vectorize \
	    -Rpass-missed=loop-vectorize \
	    -Rpass-analysis=loop-vectorize

clean:
	$(RM) linear-generator *.o *.s vec.all
