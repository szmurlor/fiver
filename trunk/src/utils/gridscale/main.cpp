#include <grid.h>
#include <field.h>
#include <elem.h>
#include <diffreader.h>
#include <diffsimu.h>
#include <stdio.h>
#include <vector>
#include <geom.h>
#include <math.h>

void scaleGrid( Grid * g, double fScale)
{
	int in;
	for (in = 1; in <= g->getNoNodes(); in++) {
		Node * n = g->getNode( in );
		for (int c = 0; c < g->NumberOfSpaceDim; c++)
			n->_Coords[c] = n->_Coords[c] * fScale;
	}
}

void printHelp()
{
	printf("usage: gridscale -g grid_name [-gn grid_nr] [-s fScale newFName] \n");
	printf("    -gn grid_name      : name of file with grid definition.\n");
	printf("    -g grid_nr         : index of grid inside the file to be read (first index = 1)\n");
	printf("    -s fScale mewFName : rescale grid with fScale factor and save new grid to newFName\n");

}

int main(int argc, char *argv[])
{
	int i;
	bool bScale = false;
	double fScale = 0.0f;
	char * newFName = 0;

	DiffSimu smgr( argc, argv );

	if (argc < 2) {
		printHelp();
		exit(-1);
	}
	
	i = 1;
	while (i<argc) {
		if (strcmp("-s", argv[i]) == 0) {
			bScale = true;
			fScale =  atof( argv[++i] );
			newFName = argv[++i];
		}

		i++;
	}
	
	if (bScale) {
		Grid * g = smgr.getGrid();
		if (g != 0) {
			scaleGrid(g,fScale);
			DpReader dpr;
			dpr.SaveToFile(g,newFName);
		}
	}
	
	return(0);
}
