#include <stdio.h>
#include <stdlib.h>
#include <grid.h>
#include <meshreader/diffreader.h>
#include <voxelcont.h>

int main(int argc, char **argv)
{
        Grid g;
        DpGridFileInfo dgfi;
        DpReader dr;
	Node *n;
	int i;
	int size = 10;
	int nc=0;
	

        dgfi.ReadFromFile(argv[1]);
        dr.ReadFromFile(&g,argv[1], dgfi.getInfo(0).position);

	double p1[3], p2[3];
	double p[3];

	g.getBBox(p1,p2);

	printf("Boundig box = (%lf, %lf, %lf) x (%lf, %lf, %lf)\n", p1[0], p1[1], p1[2],p2[0], p2[1], p2[2]);

	VoxelCont * vc = new VoxelCont(size, p1 ,p2);
	int v[3];

	for (i=0; i<g._nodes.size(); i++) {
		n = g.getNode(i + 1);
		SET(p, n->_Coords);

		vc->getVoxCrd(v, p);
		vc->addVoxInt(v, n->nr);
	}

	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
			for (int z = 0; z < size; z++){
				VolVoxelPtv * vv = vc->getVox(x,y,z);
				printf("Voxel: %d, %d, %d: ",x,y,z);

				int f = vv->first();
				while (f >= 0) {
					n = g.getNode( f );
					printf("(%lf, %lf, %lf) ", n->x(), n->y(), n->z());
					f = vv->next();
					nc++;
				}
				printf("\n");
				printf("nc = %d\n", nc);
					
				
			}
				

	delete vc;
	
	Elem * e;
	nc = 0;
	g.prepareElemPtv(size);
	vc = g._getVoxelPtv();
	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
			for (int z = 0; z < size; z++){
				VolVoxelPtv * vv = vc->getVox(x,y,z);
				printf("Voxel: %d, %d, %d: ",x,y,z);

				int f = vv->first();
				while (f >= 0) {
					e = g.getElem( f );
					printf("%d ", f);
					f = vv->next();
					nc++;
				}
				printf("\n");
				printf("nc = %d\n", nc);
					
				
			}
	
	return 0;
	
}
