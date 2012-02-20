#include <grid.h>
#include <field.h>
#include <elem.h>
#include <diffreader.h>
#include <diffsimu.h>
#include <stdio.h>
#include <vector>
#include <geom.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void printHelp()
{
	printf("usage: fieldinfo [-g grid_name] [-gn grid_nr] [-P px py pz] [-L L1x L1y L1z L2x L2y L2z ndL] \n");
	printf("    -l fldname                : list fields in file fldname\n");
	printf("    -gn grid_name             : name of file with grid definition.\n");
	printf("    -g grid_nr                : index of grid inside the file to \n");
	printf("                                be read (first index = 1)\n");
	printf("    -f fldfname fc f1 f2 ..   : read fields from specified indexes \n");
	printf("                                in file. fc - number of indexes\n");
	printf("    -avg                      : calculate avergae values (the average relative to volume)\n");
}

using namespace std;
#define sqr(x) ((x)*(x))

int main(int argc, char *argv[])
{
	int i;
	DpFieldFileInfo dffi;
	char * ffname;
	char * fieldName;
	Grid * g;
	bool bList = false;
	bool bAvg = false;

	printf("Starting reading simulation.\n");
	DiffSimu smgr( argc, argv );
	printf("Finished reading simulation.\n");

	if (argc < 2) {
		printHelp();
		exit(-1);
	}
	
	i = 1;
	while (i<argc) {
		
		if (strcmp("-l",argv[i]) == 0) {
			bList = true;
			ffname = argv[++i];
		}	

		if (strcmp("-avg",argv[i]) == 0) {
			bAvg = true;
			fieldName = argv[++i];
		}	
		
		i++;
	}
	
	if (bList) {
		printf("Reading information form %s.\n",ffname);
		dffi.ReadFromFile( ffname );
		printf("Finished reading info.");
		dffi.debug();

	
	}

	if (bAvg) {
		vector<Grid*> grids = smgr.getGrids();

		if (grids.size() == 0) {
			printf("Please use -gn [fname_grid] option to enter the name of file with grid.\n");
			exit(0);
		}

		// get the first grid from file
		// this assumes that the fieldavg operates on simulations with single grid
		g = grids[0];

		vector<Field*> fields = smgr.getFields();
		printf("Found %d fields.\n", fields.size() );
		for (i = 0; i < fields.size(); i++) {
			Field * f = fields[i];
			if (f->dim() > 1)
				printf("vector: %s\n", f->name().c_str());
			else
				printf("scalar: %s\n", f->name().c_str());
		}
	
		double min_f_component=1e40;
		double max_f_component=-1e40;
		double min_f_avg = 1e40;
		double max_f_avg = -1e40;

		if ( fields.size() != 0) {
			if (fields[0]->size() == g->getNoElems() ) {
				printf("Field defined over elements.\n");
				
				double totalVol=0;
				double vol;
				double totVal = 0;
				vector<double> totSub( g->_subdomains.size() );
				for (i =1; i <= g->getNoElems(); i++) {
					Elem * e = g->getElem( i );
					vol = e->volume();
					totalVol += vol;

					double v = 0;
					for (int j = 0; j < fields.size(); j++) {
						v += sqr( fields[j]->val(i) );
						if (min_f_component > fields[j]->val(i)) min_f_component = fields[j]->val(i);
						if (max_f_component < fields[j]->val(i)) max_f_component = fields[j]->val(i);
					}

					v = sqrt(v);

					if (min_f_avg > v) min_f_avg = v;
					if (max_f_avg < v) max_f_avg = v;

					totVal += v * vol;
				}

				printf("Total volume = %lf\n", totalVol);
				printf("Total value = %lf\n", totVal);
				printf("Min field component value = %lf\n", min_f_component);
				printf("Max field component value = %lf\n", max_f_component);
				printf("Min field module value = %lf\n", min_f_avg);
				printf("Max field module value = %lf\n", max_f_avg);
				printf("Average value = %lf\n", totVal / totalVol);


			} else if (fields[0]->size() == g->getNoNodes() ) {
				printf("Field defined over nodes.\n");

				printf("(%s,%d): NOT IMPLEMENTED!!! \n", __FILE__, __LINE__);


			} else {
				printf("Grid (n:%d,e:%d) and Field (%d) does not match: Incorrect number of field values, nodes and elements.\n", 
							g->getNoNodes(), g->getNoElems(), fields[0]->size()
					   );
			}
		}
	}
	
	return(0);
}
