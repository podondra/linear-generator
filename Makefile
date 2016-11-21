SHELL = /bin/sh
DIAG = -fdiagnostics-color -Wall
OBJ = lg.o random.o seq.o opt.o
ARCH = -march=ivybridge
CXX = g++ -std=c++11 $(ARCH)

all: $(OBJ)
	$(CXX) $(DIAG) $(OBJ) -o lg -L/usr/lib64 -lpapi

lg.o: src/lg.cc src/seq.h src/opt.h
	$(CXX) $(DIAG) -O3 -c $<

random.o: src/random.cc
	$(CXX) $(DIAG) -O3 -c $<

seq.o: src/seq.cc src/random.h
	$(CXX) $(DIAG) -O3 -c $<

opt.o: src/opt.cc src/random.h
	$(CXX) $(DIAG) -O3 -c $< -fopt-info-vec-optimized -mavx -ffast-math \
	    -DPAPI -I/usr/include

doc: doc/header.html README.md doc/footer.html
	cat doc/header.html > index.html
	markdown README.md >> index.html
	cat doc/footer.html >> index.html

clean:
	$(RM) lg *.o
