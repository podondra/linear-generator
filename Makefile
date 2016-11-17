SHELL = /bin/sh
DIAG = -fdiagnostics-color -Wall
OBJ = lg.o random.o seq.o
ARCH = -march=ivybridge
CXX = g++ -std=c++11 $(ARCH)

all: $(OBJ)
	$(CXX) $(DIAG) $(OBJ) -o lg

lg.o: src/lg.cc src/seq.h
	$(CXX) $(DIAG) -O3 -c $<

random.o: src/random.cc
	$(CXX) $(DIAG) -O3 -c $<

seq.o: src/seq.cc src/random.h
	$(CXX) $(DIAG) -O3 -c $<

doc:
	markdown README.md > index.html

clean:
	$(RM) lg *.o index.html
