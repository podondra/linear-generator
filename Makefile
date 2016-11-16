SHELL = /bin/sh
CXX = g++ -std=c++11
DIAG = -fdiagnostics-color -Wall
OBJ = lg.o random.o seq.o

all: $(OBJ)
	$(CXX) $(DIAG) $(OBJ) -o lg

lg.o: src/lg.cc src/seq.h
	$(CXX) $(DIAG) -O3 -c $<

random.o: src/random.cc
	$(CXX) $(DIAG) -O3 -c $<

seq.o: src/seq.cc src/random.h
	$(CXX) $(DIAG) -O3 -c $<

clean:
	$(RM) lg *.o
