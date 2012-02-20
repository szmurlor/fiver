#include "fieldsection.h"
#include <geom.h>
#include <elem.h>
#include <grid.h>
#include <simplefield.h>
#include <elemt3n2d.h>

FieldSection::FieldSection( )
{
	P[0] = P[1] = P[2] = 0;
	N[0] = N[1] = N[2] = 0;
}

FieldSection::~ FieldSection( )
{
}

void FieldSection::setParams( double aP[3], double aN[3] )
{
	SET(P, aP);
	SET(N, aN);
}

void FieldSection::section( Grid * srcGrid, SimpleField * srcField, Grid * dstGrid, SimpleField * dstField )
{
	int ie;
	Elem * e;
	Point _P, _N;
	Node* ns[3];

	SET(_P.c, P);
	SET(_N.c, N);
	
	if (dstField != 0)
		dstField->dim( srcField->dim() );

        dstGrid->_subdomains = srcGrid->_subdomains;

	std::vector<Tri> tri;
	std::vector<Tri>::iterator it;
	for (ie = 0; ie < (int) srcGrid->_elems.size(); ie++) {
		e = srcGrid->_elems[ ie ];
		if ( e->isecPlane(_P,_N,tri,srcField) == true ) {
			for (it = tri.begin(); it != tri.end(); it++) {
				dstGrid->addNewNode( (*it).p[0].c );
				dstGrid->addNewNode( (*it).p[1].c );
				dstGrid->addNewNode( (*it).p[2].c );
				
				// addNewNode resizes the _nodes vector, and changes the 
				// allocation of Nodes, thus we need to take the new pointers
				ns[0] = &dstGrid->_nodes[ dstGrid->_nodes.size() - 3 ];
				ns[1] = &dstGrid->_nodes[ dstGrid->_nodes.size() - 2 ];
				ns[2] = &dstGrid->_nodes[ dstGrid->_nodes.size() - 1 ];
				
				ElemT3n2D * enew = new ElemT3n2D( dstGrid );
				enew->Tag = 0;
				enew->setNode(0,ns[0]->nr);
				enew->setNode(1,ns[1]->nr);
				enew->setNode(2,ns[2]->nr);
                                dstGrid->addElem( enew );

                                enew->setSubdomain(e->subdomain(),true);
				
				
				if (dstField != 0) {
					if (srcGrid->getNoNodes() == srcField->size() ) {
						// fields defined over vertices (nodes)
						for (int i = 0; i < (int) srcField->dim(); i++) {
							dstField->set(i, ns[0]->nr - 1, (*it).v[0][i]);
							dstField->set(i, ns[1]->nr - 1, (*it).v[1][i]);
							dstField->set(i, ns[2]->nr - 1, (*it).v[2][i]);
						}
					} else {
						// fields defined over elements:
						for (int i = 0; i < (int) srcField->dim(); i++) {
							dstField->set(i, enew->nr - 1, srcField->val(i, ie) );
						}
					}
				}
			}
		}
	}
	dstGrid->scanSubdomains();
}


