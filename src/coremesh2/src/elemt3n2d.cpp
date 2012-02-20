//
// C++ Implementation: elemt3n2d
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "elemt3n2d.h"
#include "elem.h"
#include "grid.h"

class Field;

int T3n2D_Faces[4] ={
    1,2,3,0
};

int T3n2D_Edges[6] ={
    1,2,
    2,3,
    3,1,
};


ElemT3n2D::ElemT3n2D()
 : Elem()
{
    init();
}


ElemT3n2D::~ElemT3n2D()
{
    _nodes.resize(0);
}

ElemT3n2D::ElemT3n2D( Grid * AParent )
{
    init();
    _parent = AParent;
}

void ElemT3n2D::init( )
{
    _parent = 0;
    _nodes.resize(3);
    Tag = 0;
    _bshpFun = false;
    bbox = 0;
    _elemtype = T3n2D;

}

void ElemT3n2D::debug()
{
    Elem::debug();
}


double ElemT3n2D::volume()
{
    return (0);
}

void ElemT3n2D::getFacesOnBnd(vector<int> &vfaces, int , bool )
{
    vfaces.resize(0);
}

void ElemT3n2D::getFacesOnBnd(vector<CFace> &vfaces, int , bool )
{
    vfaces.resize(0);
}
	

int ElemT3n2D::whichFace(CFace face)
{
    CFace tmp(3);
	
    tmp = getFace(0);
    if (tmp.cmp(face.nodes[0],face.nodes[1],face.nodes[2]) == 0)
        return(0);
    return (-1);
}

void ElemT3n2D::fixNodesOrder()
{
    double n[3];
    double t;
    Node *ns[4];
    int tmp;
    double _tmpCoords[3];

    for (int i=0; i<3;i++)
        ns[i] =	&_parent->_nodes[_nodes[i]-1];
    
    SET(_tmpCoords,ns[0]->_Coords);
    _tmpCoords[2] = 1;

    t = normalny4p(ns[0]->_Coords,ns[1]->_Coords,ns[2]->_Coords,_tmpCoords,
                   n);

    if (t>=0) {
        tmp = _nodes[0];
        _nodes[0] = _nodes[1];
        _nodes[1] = tmp;
    }
	

}
		
int ElemT3n2D::whichEdge(CEdge edge)
{
    int i;
    CEdge ed;
    for (i=0;i<3;i++) {
        ed = getEdge(i);
        if (ed.cmp(edge.n1,edge.n2) >= 0)
            return(i);
    }
    return (-1);
	
}

void ElemT3n2D::getNormalOfFace(int ifNr, double n[3])
{
//    n[0] = 0;
//    n[1] = 0;
//    n[2] = 1;
    Node ns[3];
    int n1,n2,n3;
    for (int i=0; i<3;i++)
        ns[i] =	_parent->_nodes[_nodes[i]-1];

    n1 = T3n2D_Faces[ifNr*4]-1;
    n2 = T3n2D_Faces[ifNr*4+1]-1;
    n3 = T3n2D_Faces[ifNr*4+2]-1;

//    //liczenie wektora normalnego
//    double* w1 = ns[ n1 ]._Coords;
//    double* w2 = ns[ n2 ]._Coords;
//    double* w3 = ns[ n3 ]._Coords;

//    double a1, a2, a3, b1, b2, b3, nomin;

//    a1 = (w1)[0] - (w2)[0];
//    a2 = (w1)[1] - (w2)[1];
//    a3 = (w1)[2] - (w2)[2];

//    b1 = (w1)[0] - (w3)[0];
//    b2 = (w1)[1] - (w3)[1];
//    b3 = (w1)[2] - (w3)[2];

//    double A, B, C;
//    A = a2 * b3 - a3 * b2;
//    B = a3 * b1 - a1 * b3;
//    C = a1 * b2 - a2 * b1;
//    // from belonging of Node 1 to surface

//    nomin = A*A + B*B + C*C;

//        n[0] = A/sqrt(nomin);
//        n[1] = B/sqrt(nomin);
//        n[2] = C/sqrt(nomin);

    normalny4p(ns[ n1 ]._Coords, ns[ n2 ]._Coords, ns[ n3 ]._Coords, ns[ n1 ]._Coords, n);
}

CEdge ElemT3n2D::getEdge(int i)
{
    CEdge e;
    if (i < 3) {
        e.n1 = _nodes[ T3n2D_Edges[i*2] -1 ];
        e.n2 = _nodes[ T3n2D_Edges[i*2+1] -1 ];
    } else {
        printf("Incorrect edge number: %d",i);
    }
    return e;
}

int ElemT3n2D::getNeighbourWithFaceNr(int )
{
    return 0 ;
}

int ElemT3n2D::getNeighbourWithFace(CFace )
{
    return 0;
}

void ElemT3n2D::getNeighboursWithEdge( CEdge edge, ElemList &el)
{
    Elem *e;
    unsigned int i,j;
	vector<int> * _nbrs;
	
    el.clear();
	
    for (i = 0;i<3;i++) {
		_nbrs = _parent->getNodeNbrs( _nodes[i] );
        for (j = 0; j < _nbrs->size(); j++) {
            e = _parent->_elems[(*_nbrs)[j]-1];
            if (e != this) {
                if ((e->whichEdge(edge)) >= 0) {
                    if (el.find(e->nr) == 0)
                        el.addNew(e->nr);
                }
            }
        }
    }
}

CFace ElemT3n2D::getFace(int ifNr)
{
    CFace f(3);
    int i;
    for (i=0; i<3;i++)
        f.nodes[i] = _nodes[ T3n2D_Faces[ifNr*4+i] - 1 ];
    return f;
}

int ElemT3n2D::getFaceHash(int iFace)
{
	int hash = 0;
	for (unsigned int i=0; i<3;i++)
		hash += _nodes[ T3n2D_Faces[iFace*4+i] - 1 ];
	return hash;
}

CFace ElemT3n2D::getFaceOrder(int ifNr)
{
    int i;
	int tf[4];
	Node *ns[4];
	double n[3];
	int tmp;
	double t;
	double tc[3];


	for (i=0; i<3;i++)
		tf[i] = _nodes[ T3n2D_Faces[ifNr*4+i] - 1 ];

	for (int i=0; i<3;i++)
	    ns[i] = &_parent->_nodes[ tf[i]-1 ];

	tc[0] = 0;
	tc[1] = 0;
	tc[2] = -1;

	t = normalny4p(ns[0]->_Coords,ns[1]->_Coords,ns[2]->_Coords,tc, n);

	if (t>=0) {
		tmp = tf[0];
		tf[0] = tf[1];
		tf[1] = tmp;
	}
	CFace f(3);
	for (i=0; i<3;i++)
		f.nodes[i] = tf[ i ];
																									    return f;

}

bool ElemT3n2D::isPointInside( double P[3] )
{
   checkBBox();

   if (	(P[0] >= bbox[0]) &&
   		(P[1] >= bbox[1]) &&
		(P[0] <= bbox[3]) &&
		(P[1] <= bbox[4]) ) {
		double n[3][3];
		Node *ns[3];
		for (int i=0; i<(int)_nodes.size();i++) {
			ns[i] = &_parent->_nodes[_nodes[i]-1];
			SET(n[i], ns[i]->_Coords);
		}
		return IS_Point_In_Tri(P, n[0], n[1], n[2]);
	}
	return false;
}

double ElemT3n2D::calcValPtv( double P[3], vector<double> v )
{
	double n[3][3]; // third coordinate (z) will be zero
	double dv[3];
	SET(dv,v);
	for (int i=0; i<(int)_nodes.size();i++) {
		Node *nd = &_parent->_nodes[_nodes[i]-1];
		SET(n[i], nd->_Coords);
	}
	return valTriPtv( P, n, dv );
}

void ElemT3n2D::setNodes( int n1, int n2, int n3 )
{
    setNode(0,n1);
    setNode(1,n2);
    setNode(2,n3);
}

void ElemT3n2D::updateInfo()
{
}

int ElemT3n2D::getPointEdgeDistances( double P[3], vector<double> & dist )
{
    Node *n1, *n2;
	double dn[3];

	dist.resize(6);

	for (int i = 0 ; i < 3; i++) {
		n1 = &_parent->_nodes[ _nodes[T3n2D_Edges[i * 2]    -1] - 1];
		n2 = &_parent->_nodes[ _nodes[T3n2D_Edges[i * 2 + 1]-1] - 1];
		SUB(dn,n2->_Coords, n1->_Coords);
		dist[i] = DIST_Point_Line( P, dn, n1->_Coords );

	}
	return 3;
}

bool ElemT3n2D::isecPlane( Point , Point , vector<Tri> & , SimpleField*  )
{
	return false;
}

bool ElemT3n2D::isExternal()
{
	return true;
}
