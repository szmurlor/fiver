#include <stdio.h>
#include <stdlib.h>
#include <grid.h>
#include <diffreader.h>
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;


int getExternalSurface( Grid * g, bool bMatlab, bool bMatlabNorm )
{
	int i,j;
	vector< CFace > faces;
	CFace fc;

	g->nbrElems();
	for (i=0; i < g->_elems.size(); i++) {
		for (j=0; j < 4; j++)
			if (g->_elems[i]->getNeighbourWithFaceNr( j ) == 0) {
				fc = g->_elems[i]->getFaceOrder( j );
				g->_nodes[ fc.nodes[0] - 1].Tag = 1;
				g->_nodes[ fc.nodes[1] - 1].Tag = 1;
				g->_nodes[ fc.nodes[2] - 1].Tag = 1;

				faces.push_back( fc );
			}
	}

	if (bMatlab) 
		printf("Nodes = [\n");
	j = 1;
	for (i=0; i< g->_nodes.size(); i++) {
		if (g->_nodes[i].Tag != 0) {
			g->_nodes[i].Tag = j;
			printf("%g %g %g\n",g->_nodes[i].x(), g->_nodes[i].y(), g->_nodes[i].z());
			j++;
		}
	}

	if (bMatlab) 
		printf("];\nFaces = [\n");

	for (i=0; i< faces.size(); i++) {
		int tmp;
		for (j=0; j<3; j++) {
			tmp = g->_nodes[ faces[i].nodes[j] - 1 ].Tag;
			printf("%d ",tmp);
		}
		printf("\n");
	}

	if (bMatlab) {
		printf("];\ntrisurf(Faces, Nodes(:,1), Nodes(:,2), Nodes(:,3) );\n");
		if (bMatlabNorm) {
			printf("hold on\n");
			printf("trisurf(Faces, Nodes(:,1), Nodes(:,2), Nodes(:,3) );\n");
			printf("N = size(Faces,1)\n");
			printf("for i = 1:N,\n");
			printf("  v1 = ( Nodes( Faces(i,1) ,:) - Nodes( Faces(i,2), :) );\n");
			printf("  v2 = ( Nodes( Faces(i,3) ,:) - Nodes( Faces(i,2), :) );\n");
			printf("  p = Nodes( Faces(i,1),:) + Nodes( Faces(i,2),:) + Nodes( Faces(i,3),:);\n");
			printf("  p = p / 3;\n");
			printf("  n = cross(v1,v2);\n");
			printf("  n = n / norm(n);\n");
			printf("  quiver3(p(1),p(2),p(3), n(1),n(2),n(3),0.1);\n");
			printf("end\n");
			printf("hold off\n");
		}
	}

}
	


void printHelp(char **argv) 
{
  cout << "usage: ";
  cout << argv[0] << " input.grid  [-e] [-matlab]" << endl; 
	cout << " -e            - save external surface" << endl;
	cout << " -matlab       - add matlab visualisation" << endl;
	cout << " -matlab_norm  - add matlab normal to surface faces code" << endl;
}

int main(int argc, char **argv)
{
    int i;
		char * fin;
		int iGridNum = 1;
		bool bExternal = false;
		bool bMatlab = false;
		bool bMatlabNorm = false;
		

    if (argc<3) {
        printHelp(argv);
        exit(0);
    }

		for (i = 2; i < argc; i++) {
			if (strcmp(argv[i], "-h") == 0) {
				printHelp(argv);
				return(0);
			}

			if (strcmp(argv[i], "-n") == 0) {
				iGridNum = atoi(argv[++i]);
			}

			if (strcmp(argv[i], "-e") == 0) {
				bExternal = true;
			}

			if (strcmp(argv[i], "-matlab") == 0) {
				bMatlab = true;
			}

			if (strcmp(argv[i], "-matlab_norm") == 0) {
				bMatlabNorm = true;
			}
		}

		fin = argv[1];

	  Grid g;
    DpGridFileInfo dgfi;
    DpReader dr;


    dgfi.ReadFromFile(fin);
    dr.ReadFromFile(&g,fin, dgfi.getInfo(iGridNum - 1)->position);
   
		if (bExternal) {
			getExternalSurface( &g, bMatlab, bMatlabNorm );
		}

    return(0);
}
