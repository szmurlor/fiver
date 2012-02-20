CXXINC=-lcoremesh2 -lmeshreader2
CXXINCP=-I../../src -I../../../meshreader2
CXXLIBP=-L../../lib -L../../../meshreader2

all:
	g++ --pedantic -Wall -o main main.cpp -lm $(CXXINC) $(CXXINCP) $(CXXLIBP)

# usage: cmd grid Px Py Pz Nx Ny Nz
test: all
	(export LD_LIBRARY_PATH=../../lib:../../../meshreader2; ./main 0 0 0 1 0 0 1 0 0)
	(export LD_LIBRARY_PATH=../../lib:../../../meshreader2; ./main 0.1 0 0 1 0 0 1 0 0)
	(export LD_LIBRARY_PATH=../../lib:../../../meshreader2; ./main 0.1 0 0 1 0 0 1 0.1 0)
