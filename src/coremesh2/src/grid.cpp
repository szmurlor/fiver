#include "grid.h"
#include <string.h>
#include <math.h>
#include <geom.h>
#include <time.h>
#include <iostream>

using namespace Helpers;

void Grid::prepareElemPtv(int size)
{
	unsigned int ie,in,i;
	double p1[3],p2[3];
	int vox[3];
	vector<IPoint> vtmp;
	bool bf;
	Elem * e;
	Node * n;


    getBBox(p1,p2);
    if (voxc != 0) delete voxc;
	voxc = new VoxelCont( size, p1, p2 );

	for (ie = 0; ie < _elems.size(); ie++) {
		vtmp.resize(0);
		e = getElem( ie+1 );
        for (in=0; in < e->_nodes.size(); in++) {
			n = getNode( e->_nodes[in] );
			voxc->getVoxCrd(vox, n->_Coords);
			bf = false;	
			for (i=0; i< vtmp.size(); i++)
				if ( (vtmp[i].x == vox[0]) && (vtmp[i].y == vox[1]) && (vtmp[i].z == vox[2]) ) 
					bf = true;
			if (bf == false ) {
				vtmp.resize( vtmp.size() + 1 );
				voxc->addVoxInt( vox, e->nr );
				vtmp.back().x = vox[0];
				vtmp.back().y = vox[1];
				vtmp.back().z = vox[2];
			}
		}
	}
}

void Grid::getBBox(double minP[3], double maxP[3])
{
	unsigned int in;
	Node *n;
	minP[0]=minP[1]=minP[2]=1e20;
	maxP[0]=maxP[1]=maxP[2]=-1e20;
	
	for (in=0; in < _nodes.size(); in++) {
		n = &_nodes[in];
		if (n->_Coords[0] < minP[0]) minP[0] = n->_Coords[0];
		if (n->_Coords[1] < minP[1]) minP[1] = n->_Coords[1];
		if (n->_Coords[2] < minP[2]) minP[2] = n->_Coords[2];
		if (n->_Coords[0] > maxP[0]) maxP[0] = n->_Coords[0];
		if (n->_Coords[1] > maxP[1]) maxP[1] = n->_Coords[1];
		if (n->_Coords[2] > maxP[2]) maxP[2] = n->_Coords[2];
	//printf("min = %lf,%lf,%lf\n",minP[0], minP[1], minP[2]);
	//printf("max = %lf,%lf,%lf\n",maxP[0], maxP[1], maxP[2]);
	}

        SET(bb_min, minP);
        SET(bb_max, maxP);
	//printf("min = %lf,%lf,%lf\n",minP[0], minP[1], minP[2]);
	//printf("max = %lf,%lf,%lf\n",maxP[0], maxP[1], maxP[2]);
}

void Grid::getCenter( double P[3] )
{
    unsigned int ie,ic;
    double cc[3];
    cc[0] = cc[1] = cc[2] = 0;
    vector< double > c;
    
    for (ie = 0; ie < _elems.size(); ie++) {
        c = _elems[ie]->center();
        for (ic=0; ic < c.size(); ic++)
            cc[ic] += c[ic];
    }
    P[0] = cc[0] / (double) _elems.size();
    P[1] = cc[1] / (double) _elems.size();
    P[2] = cc[2] / (double) _elems.size();
}


Grid::Grid()
{
    // Tworze grid.
    //etype = T4n3D;
	noFacesInElem = 4;
    _gridname[0] = 0;
    voxc = 0;
}

Grid::~Grid()
{
	clear();
}

void Grid::clear( )
{
	_nodes.resize(0);
	for (unsigned int i=0; i< _elems.size(); i++)
		delete _elems[i];
	_elems.resize(0);
	_bnds.resize(0);
	_subdomains.resize(0);
	
	if (voxc != 0)
		delete voxc;
	_nodeNbrs.resize(0);
	_elemNbrs.resize(0);
	
}

void Grid::debug()
{
    cout << ">>> Grid " << endl;
    cout << "Name: " << gridName() << endl;
    unsigned int i =0;
    while (i<_nodes.size()){
        _nodes[i].debug();  
        i++;
    }
    cout << endl;
    i =0;
    while (i<_elems.size()){
        _elems[i]->debug();  
        i++;
    }

    i = 0;
    while (i<_subdomains.size()){
        cout << "Subdomain:" << _subdomains[i].nr << endl;
        _subdomains[i].debug();
        i++;
    };
    cout << endl;
    i = 0;
    while (i<_bnds.size()){
        _bnds[i].debug();
        i++;
    };
    
    cout <<" endof GRID" << endl;
    
    map<string,string>::iterator it;
    
    for (it = attr.begin(); it != attr.end(); it++ ) {
        cout << "[\"" << it->first << "\"] = " << it->second << endl;
    }
}

void Grid::setGridName(string aname)
{
       _gridname = aname;
}

void Grid::addNodeToBoundaryIndex(int nodenr, int boundary_index)
{
	//printf("Adding to boundary: %d\n", boundary_index);
	//printf("Adding to node: %d\n",nodenr);
	//printf("boundary nr = %d\n",_bnds[boundary_index].nr);
	//printf("_bnds[boundary_index]._nodes.size() = %d\n", _bnds[boundary_index]._nodes.size());
	//printf("&_nodes[ nodenr ] = %p\n",&_nodes[ nodenr ]);
    _nodes[ nodenr ].addBoundary( _bnds[boundary_index].nr, false );
    _bnds[boundary_index]._nodes.push_back(nodenr+1);
}

void Grid::addNodeToBoundary(int nodenr, int boundary_nr)
{
    int j;
	j = indexOfBoundary( boundary_nr );
    if (j==-1) {        
        Boundary bnd;
        bnd.nr = boundary_nr;
        _bnds.push_back(bnd);
        j = _bnds.size() - 1;
    }
    _bnds[j]._nodes.push_back(nodenr);
}                   

int Grid::indexOfBoundary( int boundary_nr )
{
    unsigned int i = 0;
    int j = -1;
    while (i<_bnds.size()){
        if (_bnds[i].nr == boundary_nr) 
        {
            j = i;
            break;
        }
        i++;
    }
    return(j);
}

int Grid::indexOfSubdomain( int subdomain_nr )
{
    unsigned int i = 0;
    int j = -1;
    while (i<_subdomains.size()){
        if (_subdomains[i].nr == subdomain_nr) 
        {
            j = i;
            break;
        }
        i++;
    }
    return(j);
}



void Grid::addElemToSubdomain(int elemnr,int subdomain_nr)
{
    unsigned int i = 0;
    while (i < _subdomains.size()) {
        if (_subdomains[i].nr == subdomain_nr) {
            _subdomains[i].addElem( elemnr );
            break;
        }
        i++;
    }
    if (i>=_subdomains.size()) {        
        Subdomain s;
        _subdomains.push_back(s);
        _subdomains[_subdomains.size()-1].nr = subdomain_nr;
        _subdomains[_subdomains.size()-1].addElem( elemnr );
    }
    _elems[elemnr-1]->setSubdomain(subdomain_nr,false);
}

void Grid::compactGrid()
{
    unsigned int i;
    int j;

    vector<Elem*> tmpElems;
    
    j = 0;
    for (i=0; i<_elems.size(); i++)
    {
        if (_elems[i]->Tag == 0)
        {
            _elems[i]->nr = j+1;
            j++;
        }
        else {
            _elems[i]->nr = -1;
        }
    }
    for (i=0;i<_nodes.size();i++)
        _nodes[i].Tag = 0;

    tmpElems.resize(j);
    j = 0;
    for (i=0;i<_elems.size();i++)
    {
        if (_elems[i]->Tag == 0)
        {
            for (unsigned int k=0; k < _elems[i]->_nodes.size(); k++) {
                (*(_elems[i]))[k].Tag = 1;
            }
            
            tmpElems[j] = _elems[i];
            j++;
        } else {
            delete _elems[i];
        }
    }
    
    j = 0;
    for (i=0;i<_nodes.size();i++)
    {
        if (_nodes[i].Tag == 1) {
            j++;
            _nodes[i].nr = j;
        }
    }

    vector<Node> tmpNodes;
    tmpNodes.resize(j);

    j = 0;
    for (i=0;i<_nodes.size();i++)
    {
        if (_nodes[i].Tag == 1) {
            tmpNodes[j] = _nodes[i];
            j++;
        }
    }

    for(i=0;i<tmpElems.size();i++) {
        for (unsigned int k=0; k < tmpElems[i]->_nodes.size(); k++)
            (tmpElems[i])->_nodes[k] = (*tmpElems[i])[k].nr;
    }

    _elems = tmpElems;
    _nodes = tmpNodes;
    
    tmpElems.resize(0);
    tmpNodes.resize(0);
    
}


void Grid::nodes2Elems()
{
    unsigned int i;

	time_t t1;
	time_t t2;
	time(&t1);
        printf("Started nodes2Elems: %s [n:%d,e:%d]\n", ctime(&t1), _nodes.size(), _elems.size());

	_nodeNbrs.resize( _nodes.size() );
    
	for (i=0;i<_elems.size();i++)
	{
		for (unsigned int j=0;j<_elems[i]->_nodes.size();j++)
		{
			_nodeNbrs[ _elems[i]->_nodes[j] - 1 ].push_back(i+1);
		}
	}

	time(&t2);
	printf("Finished nodes2Elems: %s", ctime(&t2));
	printf("Time elapsed: %f\n", difftime( t1, t2));

}

void Grid::nbrElems()
{
    unsigned int k;

	if (_nodeNbrs.size() == 0) nbrNodes();

	// skip all elements which are less than tetrahedra.
	// skip 2D elements
	if (_elems[0]->_nodes.size() < 4) return;

/*
    time_t t;
    time(&t);
    printf("Started: %s", ctime(&t));
*/
	_elemNbrs.resize( noFacesInElem * _elems.size(), 0 );
	
#define ALG_20060816

#ifdef ALG_20060816
	_elemFaceHashes.resize( noFacesInElem * _elems.size(), 0 );
	
	// compute Element faces hashes...
	unsigned int ie;
	CFace face;
	for (ie = 1; ie <= _elems.size(); ie++)
	{
		Elem * e1;
		e1 = getElem( ie );
		int iface;
		
		for (iface = 0; iface < e1->getFacesCount(); iface++) {
			setElemFaceHash(ie,iface, e1->getFaceHash( iface ) );
		}
	}
	
	//for (i=0; i < _elemFaceHashes.size(); i++) 
	printf("Calculated hashes.\n");
	
	setElemsTagForNbr(0);
	
	unsigned int node;
	unsigned int nbr;
	unsigned int nbr_iter;
	Elem * e1;
	Elem * e2;
	unsigned int ie1;
	unsigned int ie2;
	
	for (node = 1; node <= _nodes.size(); node++) {
		//printf("1\n");
		for (nbr = 0; nbr < getNodeNbrs(node)->size(); nbr++) {
			//printf("2\n");
			ie1 = (*getNodeNbrs(node))[nbr];
			e1 = getElem(ie1);
			//printf("3\n");
			if (e1->TagForNbr < 4) { // analyse
				//e1->Tag = 1;
				//printf("4\n");
				for (nbr_iter = 0; nbr_iter < getNodeNbrs(node)->size(); nbr_iter++) {
					//printf("5\n");
					ie2 = (*getNodeNbrs(node))[nbr_iter];
					e2 = getElem(ie2);
					//printf("6\n");
					
					if ( (e1 != e2) && (e2->TagForNbr < 4)) {
						//printf("7\n");
						
						for (int iFace = 0; iFace < e1->getFacesCount(); iFace++) {
							if (getElemNbr( ie1, iFace) == 0) {
								for (int iFace2 = 0; iFace2 < e2->getFacesCount(); iFace2++) {
									if (getElemFaceHash(ie1, iFace) == getElemFaceHash( ie2, iFace2) ) {
										face = e1->getFace(iFace);
										int wf = e2->whichFace(face);
										if (wf >= 0) {
											//printf("Setting ElemNbr (%d,%d,%d)\n",ie1, iFace, ie2);
											setElemNbr( ie1, iFace, ie2 );
											//printf("Setting ElemNbr (%d,%d,%d)\n",ie2, wf, ie1);
											setElemNbr( ie2, wf, ie1 );
											
											e1->TagForNbr += 1;
											e2->TagForNbr += 1;
										}
									}
								}
							}
						}
					}
				}
				
				e1->setExternal( false );
				for (k = 0; k < (unsigned int) noFacesInElem; k++) {
					int inbr = getElemNbr( e1->nr, k );
					if ( inbr == 0 ) {
						e1->setExternal(true);
						//printf("E->nr = %d, External\n", e1->nr);
						break;
					} else {
						e2 = getElem( inbr );
						if (e2->subdomain() != e1->subdomain()) {
							//printf("E->nr = %d, External\n", e1->nr);
							e1->setExternal(true);
							break;
						}
					}
				}
				
			}
		}
	}
	
	/*
	for (ie1 = 1; ie1 <= _elems.size(); ie1++) {
		e1 = getElem(ie1);
		if (e1->isExternal() == true) 
			printf("%d: external\n", ie1);
	}
	
	for (ie1 = 1; ie1 <= _elems.size(); ie1++) {
		printf("%d: ", ie1);
		for (k = 0; k < (unsigned int) noFacesInElem; k++) {
			printf(" %d, ", getElemNbr(ie1,k));
		}
		printf("\n");
	}
	*/
	
#else
    unsigned int nb,l,c,i;
    Elem * e1, *e2;
    int node;
    CFace face;

    for (i=0;i<_elems.size();i++)
    {
		e1 = _elems[i];
		// get the first node in element
		node = e1->_nodes[0];
		
		// iterate over all elements touching the node.
		for (nb=0; nb < _nodeNbrs[node-1].size(); nb++) {
			e2 = _elems[ _nodeNbrs[node-1][nb] - 1 ];
			e2->Tag = e1->nr;

			if (e2 != e1) {
				//e2->checkNeighbour( e1 );
				c = 0;
				for (k = 0; k < e1->_nodes.size(); k++) {
					for (l = 0; l < e2->_nodes.size(); l++) {
						if (e1->_nodes[k] == e2->_nodes[l])
							face.nodes[c++] = e1->_nodes[k];
					}
					if ((k==2) && (c<2))
						k = 5;
				}

				if (c==3) {
					setElemNbr( e1->nr, e1->whichFace( face ), e2->nr );
				}
			}
		}

		node = e1->_nodes[1];
		
		for (nb=0; nb < _nodeNbrs[node-1].size(); nb++) {
			e2 = _elems[ _nodeNbrs[node-1][nb] - 1 ];
			if ((e2!=e1)&&(e2->Tag != e1->nr)) {

				//e2->checkNeighbour( e1 );
				c = 0;
				for (k = 0; k < e1->_nodes.size(); k++) {
					for (l = 0; l < e2->_nodes.size(); l++) {
						if (e1->_nodes[k] == e2->_nodes[l])
							face.nodes[c++] = e1->_nodes[k];
					}
					if ((k==2) && (c<2))
						k = 5;
				}

				if (c==3) {
					setElemNbr( e1->nr, e1->whichFace( face ), e2->nr );
				}
			}
		}

		e1->setExternal( false );
		for (k = 0; k < (unsigned int) noFacesInElem; k++) {
			int inbr = getElemNbr( e1->nr, k );
			if ( inbr == 0 ) {
				e1->setExternal(true);
				break;
			} else {
				e2 = getElem( inbr );
				if (e2->subdomain() != e1->subdomain()) {
					e1->setExternal(true);
					break;
				}
			}
		}
    }
#endif
	
    printf("Finished: \n");
/*
    time(&t);
    printf("Finished: %s", ctime(&t));
	for (i=0; i<_elems.size(); i++) {
		printf("%d \t%d \t%d \t%d\n",nbrs[i*4+0],nbrs[i*4+1],nbrs[i*4+2],nbrs[i*4+3]);
	}
*/
}

void Grid::boundaryNodes(int _bnd)
{
    unsigned int i,j;
    int enr;

    nodes2Elems();

    for (i=0; i<_elems.size(); i++) {
        for (j=0; j < _elems[i]->_nodes.size(); j++) {
			if ((enr=_elems[i]->getNeighbourWithFaceNr( j )) == 0 ) {
				CFace f = _elems[i]->getFace( j );
				_nodes[ f.nodes[0] - 1 ].addBoundary(_bnd,true);
				_nodes[ f.nodes[1] - 1 ].addBoundary(_bnd,true);
				_nodes[ f.nodes[2] - 1 ].addBoundary(_bnd,true);
				
			}
		}
    }
    
}

void Grid::boundaryNodesSub(int _bnd, int _sub)
{
    unsigned int i,j;
    int enr;
	bool bAdd;

    nodes2Elems();

    for (i=0; i<_elems.size(); i++) {
		if ( _elems[i]->subdomain() == _sub ) {
	        for (j=0; j < _elems[i]->_nodes.size(); j++) {
				enr = _elems[i]->getNeighbourWithFaceNr( j );
				bAdd = false;
				if (enr == 0 ) 
					bAdd = true;
			 	else 
					if (_elems[ enr ]->subdomain() != _elems[i]->subdomain()) 
						bAdd = true;
				if (bAdd) {
					CFace f = _elems[i]->getFace( j );
					_nodes[ f.nodes[0] - 1 ].addBoundary(_bnd,true);
					_nodes[ f.nodes[1] - 1 ].addBoundary(_bnd,true);
					_nodes[ f.nodes[2] - 1 ].addBoundary(_bnd,true);
				}
			}
		}
    }
    
}

void Grid::scanBoundaries()
{
    unsigned int i,j=0;
    for (i=0; i< _nodes.size();i++) {
        for (j=0;j<_nodes[i]._bnds.size();j++)
            addBoundary( _nodes[i]._bnds[j] );
    }
}

void Grid::scanSubdomains()
{
    unsigned int i;
    for (i=0; i< _elems.size();i++) {
        addSubdomain( _elems[i]->subdomain() );
    }

}

void Grid::addSubdomain(int _snr)
{   
    unsigned int i,b = 0;
    for (i=0;i<_subdomains.size();i++)
        if (_subdomains[i].nr == _snr) 
            b = 1;

    if (b==0) {
        Subdomain sbm(_snr);
        _subdomains.push_back(sbm);
    }
}

void Grid::addBoundary(int _bnr)
{
    unsigned int i,b = 0;
    for (i=0;i<_bnds.size();i++)
        if (_bnds[i].nr == _bnr) 
            b = 1;

    if (b==0) {
        Boundary bnd;
        bnd.nr = _bnr;
        _bnds.push_back(bnd);
    }
    
}

void Grid::sortBoundaries()
{
	int tmp;
	for (unsigned int j = 0; j < _bnds.size(); j++)
		for (unsigned int i = 0; i < (_bnds.size()-1); i++) {
			if (_bnds[i].nr > _bnds[i+1].nr) {
				tmp = _bnds[i].nr;
				_bnds[i].nr = _bnds[i+1].nr;
				_bnds[i+1].nr = tmp;
			}
		}
}

Node * Grid::addNodeOnFace(int ElemNr, int iNode1, int iNode2, int iNode3, double Coords[3] )
{
    Elem *enr;  // ElemNr.
    Elem *elem;
    CFace face;
    int iface, i4node, ie;
    Node *n = 0;
    
    enr = _elems[ElemNr - 1];
    face.nodes[0] = iNode1;
    face.nodes[1] = iNode2;
    face.nodes[2] = iNode3;
    
    ie = enr->getNeighbourWithFace(face);
    if (ie == 0) {
        printf("Can not find neigbour of element %d, with faces: %d,%d,%d\n",ElemNr,iNode1,iNode2,iNode3);      
    }
    
    
    
    enr->Tag = 1;   
    //enr->debug();
    iface = enr->whichFace(face);
    if (enr->elemType() == T4n3D) {
        i4node = ((ElemT4n3D*)enr)->get4thofFace(iface);
        n = addNewNode(Coords);
        elem = addNewElemT4n3D( enr->subdomain(), i4node, face.nodes[0], face.nodes[1], n->nr);
        elem->updateNeighbours();
        elem = addNewElemT4n3D( enr->subdomain(), i4node, face.nodes[1], face.nodes[2], n->nr);
        elem->updateNeighbours();
        elem = addNewElemT4n3D( enr->subdomain(), i4node, face.nodes[2], face.nodes[0], n->nr);
        elem->updateNeighbours();
    
        if (ie != 0) {
            enr = _elems[ ie - 1 ];
            enr->Tag = 1;
            enr->debug();
            iface = enr->whichFace(face);
            i4node = ((ElemT4n3D*)enr)->get4thofFace(iface);  
            elem = addNewElemT4n3D( enr->subdomain(), i4node, face.nodes[0], face.nodes[1], n->nr);
            elem->updateNeighbours();
            elem = addNewElemT4n3D( enr->subdomain(), i4node, face.nodes[1], face.nodes[2], n->nr);
            elem->updateNeighbours();
            elem = addNewElemT4n3D( enr->subdomain(), i4node, face.nodes[2], face.nodes[0], n->nr);
            elem->updateNeighbours();
        }
    }
    return n;
}

/**
    Konsekwnecje:
    - pobierz wszystkie elementy, do ktorych nalezy krawedz
    - zamiast kazdego elementu utworz dwa nowe elementy
    - (pamietaj o usunieciu ich z neighbour nodes)
  */
Node * Grid::addNodeOnEdge(int ElemNr, int iNode1, int iNode2, double Coords[3])
{
	int iSub;
    Elem *enr;  // ElemNr.
    Elem *elem;
    int ie;
    Node *n;
    CElem *ce;
    CEdge alted;
    ElemList el;
    CEdge edge;
    
    edge.n1 = iNode1;
    edge.n2 = iNode2;
    
    enr = _elems[ElemNr - 1];
    enr->getNeighboursWithEdge(edge,el);
    el.addNew(enr->nr);
    
    /* Dodaje wezel do siatki! */
    n = addNewNode(Coords);
		printf("Dodalem wezel\n");
    
    if (enr->elemType() == T4n3D) {
        for (ce = el.getFirst(); ce != 0; ce = ce->next) {
            ElemT4n3D * et4 = (ElemT4n3D *) _elems[ce->nr - 1];
						printf("Analizuje element %d\n", et4->nr);
            if (et4->Tag != 1) {
                et4->Tag = 1; // zaznaczam ze element jest do usuniecia 

                ie = et4->whichEdge(edge);
                alted = et4->getAlternateEdge(ie);
                iSub = et4->subdomain();

								printf("Zaczynam dodawanie dwoch nowych elementow.\n");
                elem = addNewElemT4n3D(iSub,iNode1,n->nr, alted.n1, alted.n2);
								printf("Dodalem element %d\n", elem->nr);
								//nbrNodes();
								//nbrElems();
                //elem->updateNeighbours();
                elem = addNewElemT4n3D(iSub,iNode2,n->nr, alted.n1, alted.n2);
								printf("Dodalem element %d\n", elem->nr);
								//nbrNodes();
								//nbrElems();
                //elem->updateNeighbours();
            }
        }
    }

	return( n );

}

Node * Grid::addNewNode(double coords[3])
{
    return(addNewNode( coords[0], coords[1], coords[2] ) );
}

Elem * Grid::addNewElemT3n2D(int SubNr, int n1, int n2, int n3)
{
    ElemT3n2D * e = new ElemT3n2D( this );
    _elems.push_back( e );;
    e->nr = _elems.size();
    e->Tag = 0;
    e->setNode(0,n1);
    e->setNode(1,n2);
    e->setNode(2,n3);
    e->setSubdomain(SubNr,false);
    e->fixNodesOrder();
    return(e);
}

Elem * Grid::addNewElemT4n3D(int SubNr, int n1, int n2, int n3, int n4)
{
    ElemT4n3D * e = new ElemT4n3D( this );
    _elems.push_back( e );;
    e->nr = _elems.size();
    e->Tag = 0;
    e->setNode(0,n1);
    e->setNode(1,n2);
    e->setNode(2,n3);
    e->setNode(3,n4);
    e->setSubdomain(SubNr,false);
	e->fixNodesOrder();
    return(e);
}

void Grid::STLSaveFacesOnBnd(int _bnr, string FName, bool OnlyExternal)
{
    FILE * fout;
    Elem *e;
    unsigned int i,j,k;
    vector<int> faces;
    CFace face;
    Node *n[3];
    double norm[3];
    int iElem;

	if ((fout = fopen(FName.c_str(),"w")) == NULL) {
		fprintf(stderr,"Can not open file for writing: %s",FName.c_str());
    } else {
    
        if (OnlyExternal) {
			nodes2Elems();
			nbrElems();
		}
        
		fprintf(fout,"solid %s\n",FName.c_str());
        for (i=0; i<_elems.size(); i++){
            e = _elems[ i ];
            e->getFacesOnBnd(faces,_bnr);

            for (j=0;j<faces.size();j++){
                face = e->getFace(faces[j]);
                if (OnlyExternal) 
                        iElem = 0;
                else 
                        iElem = e->getNeighbourWithFace(face);
                if (iElem==0) {
                    e->getNormalOfFace(faces[j],norm);
                    for (k=0; k<3;k++)
                        n[k] = &_nodes[ face.nodes[k] - 1];
                    fprintf(fout," facet normal %f %f %f\n",norm[0], norm[1], norm[2]);
                    fprintf(fout,"  outer loop\n");
                    for (k=0; k<3;k++)
                        fprintf(fout,"   vertex %f %f %f\n",
                                        n[k]->_Coords[0],
                                        n[k]->_Coords[1],
                                        n[k]->_Coords[2] );
                    fprintf(fout,"  endloop\n");
                    fprintf(fout," endfacet\n");
                }
            }
        }
        fclose(fout);   
    }
            
}


Elem * Grid::getElemPtv( double P[3] )
{
	unsigned int i;
	if (voxc != 0) {
		int f;
		VolVoxelPtv * vv = voxc->getVox( P );
		f = vv->first();
		while ( f >= 0) {
			if (getElem(f)->isPointInside( P ) == true)
				return _elems[f];
			f = vv->next();
		}
	} else {
		for (i=0; i <_elems.size(); i++){
			//printf("%d\n",i);
			if (_elems[i]->isPointInside( P ) == true)
				return _elems[i];
		}
	}
	return 0;
}



void Subdomain::addElem(int ie)
{
//  printf("Starting Subdomain::addElem()\n");
    _elems.push_back(ie);
//  printf("Finished Subdomain::addElem()\n"); 
}

Subdomain::Subdomain(int nr)
{
    this->nr = nr;
}

Subdomain & Subdomain::operator=(const Subdomain & src)
{
    _elems = src._elems;
    nr = src.nr;
    strcpy(name,src.name);
	return (*this);
}

void Subdomain::debug()
{
    cout << "Subdomain: " << nr << endl;
    cout << "_elemscount = " << _elems.size() << endl;
    cout << "Elems registered to subdomain ( \n";
    unsigned int i=0;
    while (i<_elems.size()) 
    {
        cout << _elems[i] << ",";
        i++;
    }
    cout << ")" << endl;
    
}
                                
void Boundary::debug()
{
    cout << "Boundary: " << nr << endl;
    unsigned int i = 0;
    cout << "[ ";
    while (i<_nodes.size()) {
        cout << _nodes[i] << ",";
        i++;
    }
    cout << "]" << endl;
}

Boundary & Boundary::operator=(const Boundary & src)
{
    this->_nodes = src._nodes;
    this->nr = src.nr;
    strcpy(this->name,src.name);
	return (*this);
}

Node * Grid::addNewNode( double x, double y, double z )
{
    Node *n;
    _nodes.resize( _nodes.size() + 1 );
    n = &_nodes[_nodes.size()-1];
    n->nr = _nodes.size();
    n->_parent = this;
    n->_Coords[0] = x;
    n->_Coords[1] = y;
    n->_Coords[2] = z;
    
    return (n);
}

Elem * Grid::addElem( Elem * e )
{
    _elems.push_back( e );
    e->nr = _elems.size();
    return (e);
}

Node * Grid::setNodeCoords( int in, double x, double y, double z )
{
    Node * n = &_nodes[ in - 1 ];
    n->nr = in;
    n->_parent = this;
    n->_Coords[0]  = x;
    n->_Coords[1]  = y;
    n->_Coords[2]  = z;
	return n;
}

void Grid::setElemsTag( int _Tag )
{
    unsigned int i;
    for (i = 0; i < _elems.size(); i++)
        _elems[i]->Tag = _Tag;
}

void Grid::setElemsTagForNbr( int _TagForNbr )
{
    unsigned int i;
    for (i = 0; i < _elems.size(); i++)
        _elems[i]->TagForNbr = _TagForNbr;
}


void Grid::addNodeNbr( int nr , int elem_nr )
{
	if (hasNodeNbr( nr, elem_nr ) == false )
		_nodeNbrs[nr-1].push_back( elem_nr );
}

bool Grid::hasNodeNbr( int nr , int elem_nr )
{
	vector<int>::iterator i;
	for (i=_nodeNbrs[nr-1].begin(); i != _nodeNbrs[nr-1].end(); i++){
		if (*i == elem_nr) 
			return true;
	}
	return false;
}

void Grid::removeNodeNbr( int nr , int elem_nr )
{
	vector<int>::iterator i;
	for (i=_nodeNbrs[nr-1].begin(); i != _nodeNbrs[nr-1].end(); i++){
		if (*i == elem_nr) 
			_nodeNbrs[nr-1].erase( i );
	}
}

vector< int > * Grid::getNodeNbrs( int nr )
{
	if (_nodeNbrs.size() == 0)
		nbrNodes();
	return &_nodeNbrs[nr-1];
}

int Grid::getElemNbr( int nr, int iface )
{
	if (_elemNbrs.size() == 0)
		nbrElems();

	return _elemNbrs[ (nr - 1) * noFacesInElem + iface ] ;
}

void Grid::setElemNbr( int nr, int iface, int inbr )
{
	if (_elemNbrs.size() == 0)
		nbrElems();
	_elemNbrs[ (nr - 1) * noFacesInElem + iface ]  = inbr;
}

int Grid::getElemFaceHash( int enr, int facenr)
{
	return _elemFaceHashes[ (enr - 1) * noFacesInElem + facenr ];
}

void Grid::setElemFaceHash( int enr, int facenr, int hash)
{
	_elemFaceHashes[ (enr - 1) * noFacesInElem + facenr ] = hash;
}


// This function copies all data from the source grid.
void Grid::copyFrom( Grid & src )
{
	clear();
	
	_nodes =  src._nodes;
	for (unsigned int in = 0; in < _nodes.size(); in++) {
		_nodes[in]._parent = this;
	}
	
	attr = src.attr;
	
	_elems.resize( src._elems.size() );
	for (unsigned int ie = 0; ie < src._elems.size(); ie++) {
		if (src._elems[ie]->elemType() == T4n3D) 
			_elems[ie] = new ElemT4n3D;
		if (src._elems[ie]->elemType() == T3n2D) 
			_elems[ie] = new ElemT3n2D;
		
		(*_elems[ie]) = (*src._elems[ie]);
		_elems[ie]->_parent = this;
	}
	
	_bnds = src._bnds;
	_subdomains = src._subdomains;
	NumberOfSpaceDim = src.NumberOfSpaceDim;
	noFacesInElem = src.noFacesInElem;

	_gridname = src._gridname;
	SET(bb_min, src.bb_min);
	SET(bb_max, src.bb_max);
	
	_nodeNbrs = src._nodeNbrs;
	_elemNbrs = src._elemNbrs;

}

void Grid::checkNbrs()
{
        if (_nodeNbrs.size() == 0)
                nbrNodes();
        if (_elemNbrs.size() == 0)
                nbrElems();
}

void Grid::translate(double translation[3])
{
    for(unsigned int i=0; i < _nodes.size(); i++)
    {
        Node *n = &(_nodes.at(i));
        double u[3];
        u[0] = n->x() + translation[0];
        u[1] = n->y() + translation[1];
        u[2] = n->z() + translation[2];
        n->setCoords(u);
    }
}

void Grid::rotate(double center[3], double axis[3], double angle)
{
    cout << "gridRotate (" << _nodes.size() << ") "
             << "c(" << center[0] << "," << center[1] << "," << center[2] << "), "
             << "a(" << axis[0] << ", " << axis[1] << ", " << axis[2] << "), "
             << "@" << angle << "\n";
    for(unsigned int i=0; i < _nodes.size(); i++)
    {
        Node *n = &(_nodes.at(i));
        double u[3];
        u[0] =  n->x();
        u[1] =  n->y();
        u[2] =  n->z();
        if(i == 0)
            cout << "#U: " << u[0] << ",  " << u[1] << ",  " << u[2] << "\n";
        SUB(u,u,center);
        Helpers::rotate(u, axis, angle);
        ADD(u,u,center);
        if(i == 0)
            cout << "@U: " << u[0] << ",  " << u[1] << ",  " << u[2] << "\n";
        n->setCoords(u);
    }
}

void Grid::nodeSubdomains( int n, set<int>& nsubs )
{
    nsubs.clear();
    vector<int> *elems= getNodeNbrs(n+1);

    if( elems == 0 || elems->size() == 0 ) {
        std::cerr << "Grid::nodeSubdomains: WARNING: no neighbouring elements for node " << n << " !?!" << std::endl;
        return;
    }

    //std::cerr << "Grid::nodeSubdomains for node " << n << ": ";

    for( vector<int>::iterator i= elems->begin(); i != elems->end(); i++ ) {
        //std::cerr << " e# " << *i << " ";
        Elem * el = _elems[ *i - 1 ];
        nsubs.insert( el->subdomain() );
        //std::cerr << " sub: " << el->subdomain();
    }
    //std::cerr << std::endl;
}

