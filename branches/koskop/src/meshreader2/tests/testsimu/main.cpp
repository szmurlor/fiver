#include <cstdio>
#include <cstdlib>

#include <diffsimu.h>
#include <grid.h>
#include <field.h>

int main(int argc, char ** argv)
{
	printf("Before constructing.\n");
	//DiffSimu ds(argc,argv);
	DiffSimu ds;
	ds.loadFromFile( argv[1]);
	printf("After constructing.\n");

	return 0;
}
