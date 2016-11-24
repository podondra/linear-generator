SHELL = /bin/sh
CXX = g++
CC = g++
ARCH = -march=ivybridge
CXXFLAGS = -std=c++11 -O3 $(ARCH) -fdiagnostics-color -Wall
LDFLAGS = -L/ust/lib64
LDLIBS = -lpapi
VPATH = src
OBJ = lg.o random.o seq.o opt.o

lg: $(OBJ)

lg.o: src/lg.cc src/seq.h src/opt.h
random.o: src/random.cc
seq.o: src/seq.cc src/random.h

opt.o: src/opt.cc src/random.h
	$(CXX) $(CXXFLAGS) -c $< -fopt-info-vec-optimized -mavx -ffast-math \
	    -DPAPI -I/usr/include

.PHONY: doc
doc: doc/header.html README.md doc/footer.html
	cat doc/header.html > index.html
	markdown README.md >> index.html
	cat doc/footer.html >> index.html

.PHONY: clean
clean:
	$(RM) lg *.o
