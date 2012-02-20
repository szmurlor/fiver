CXXINC=-lcoremesh2 -lmeshreader2
CXXINCP=-I../../src -I../../../meshreader2
CXXLIBP=-L../../lib -L../../../meshreader2

all:
	g++ -o main main.cpp $(CXXINC) $(CXXINCP) $(CXXLIBP)

# usage: cmd grid Px Py Pz Nx Ny Nz
test: all
	(export LD_LIBRARY_PATH=../../lib:../../../meshreader2; ./main ../grids/.kond_opt.grid 0 0 2 0 0 1 out.grid ../grids/.kond_opt.field out.field)
	(export LD_LIBRARY_PATH=../../lib:../../../meshreader2; ./main ../grids/.kond_opt.grid 1 1 2 0.1 1 1 out.grid ../grids/.kond_opt.field out.field)
	(export LD_LIBRARY_PATH=../../lib:../../../meshreader2; ./main ../grids/.kond_opt.grid 4.94846 -1.10462 1.27598 0.1 1 1 out.grid ../grids/.kond_opt.field out.field)
	(export LD_LIBRARY_PATH=../../lib:../../../meshreader2; ./main ../grids/.kond_opt.grid 0 0 2 0 0 1 out.grid ../grids/.kond_opt.field out.field)
#	./main ../../grids/cube.grid
