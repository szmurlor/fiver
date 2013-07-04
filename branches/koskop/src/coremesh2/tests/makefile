CXXINC=-lcoremesh -lreadmesh
CXXINCP=-I/usr/local/include -I/usr/local/include/meshreader
CXXLIBP=-L/usr/local/lib

all:
	g++ -o main main.cpp $(CXXINC) $(CXXINCP) $(CXXLIBP)

test1:
	./main ../../grids/cone.grid
#	./main ../../grids/cube.grid
