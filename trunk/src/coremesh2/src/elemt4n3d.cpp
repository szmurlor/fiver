//
// C++ Implementation: elemt4n3d
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "elemt4n3d.h"
#include "elem.h"
#include "grid.h"
#include "simplefield.h"

int T4n3D_Faces[16] ={
            1,2,3,4,
            2,3,4,1,
            1,3,4,2,
            1,2,4,3
};

int T4n3D_Edges[12] ={
    1,2,
    2,3,
    3,4,
    1,4,
    1,3,
    2,4
};

int T4n3D_Edges_Alternate[12] ={
    3,4,
    1,4,
    1,2,
    2,3,
    2,4,
    1,3
};

ElemT4n3D::ElemT4n3D(Grid * AParent)
{
    _parent = AParent;
    _nodes.resize(4);
    Tag = 0;
    bbox = 0;
    
    _elemtype = T4n3D;
}

ElemT4n3D::~ElemT4n3D()
{
    _nodes.resize(0);
}

void ElemT4n3D::debug()
{
    Elem::debug();
}


double ElemT4n3D::volume()
{
    Node n[4];
    for (int i=0; i<4;i++)
        n[i] =	_parent->_nodes[_nodes[i]-1];

    return ( FABS( VOLUME_tetra( n[0]._Coords, n[1]._Coords, n[2]._Coords, n[3]._Coords) ) ); 
}

void ElemT4n3D::getFacesOnBnd(vector<int> &vfaces, int bnd, bool all)
{
    Node n[4];
    int n1,n2,n3;
    for (int i=0; i<4;i++)
        n[i] =	_parent->_nodes[_nodes[i]-1];

    vfaces.resize(0);
    for (int i=0; i<4; i++) {
        n1 = T4n3D_Faces[i*4] - 1;
        n2 = T4n3D_Faces[i*4+1] - 1;
        n3 = T4n3D_Faces[i*4+2] - 1;
        if((n[n1].isOnBoundary(bnd) &&
            n[n2].isOnBoundary(bnd) &&
            n[n3].isOnBoundary(bnd)) || all) {
                vfaces.push_back(i);
            }
    }	
	
}

void ElemT4n3D::getFacesOnBnd(vector<CFace> &vfaces, int bnd, bool all)
{
    CFace face(3);
    double d;
    Node n[4];
    int n1,n2,n3,n4;
    for (int i=0; i<4;i++)
        n[i] =	_parent->_nodes[_nodes[i]-1];

    vfaces.resize(0);
    for (int i=0; i<4; i++) {
        n1 = T4n3D_Faces[i*4] - 1;
        n2 = T4n3D_Faces[i*4+1] - 1;
        n3 = T4n3D_Faces[i*4+2] - 1;
        n4 = T4n3D_Faces[i*4+3] - 1;
        if((n[n1].isOnBoundary(bnd) &&
            n[n2].isOnBoundary(bnd) &&
            n[n3].isOnBoundary(bnd)) || all) {
            d = DIST_Point_Plane(n[n4]._Coords,n[n1]._Coords,n[n2]._Coords,n[n3]._Coords);
            if (d < 0) {
                face.nodes[0] = n[n1].nr;
                face.nodes[1] = n[n2].nr;
                face.nodes[2] = n[n3].nr;
            } else {
                face.nodes[0] = n[n1].nr;
                face.nodes[1] = n[n3].nr;
                face.nodes[2] = n[n2].nr;
            }
            vfaces.push_back(face);
            }
    }	
}
	

int ElemT4n3D::whichFace(CFace face)
{
    int i;
    CFace tmp(3);
	
    for (i=0;i<4;i++) {
        tmp = getFace(i);
        if (tmp.cmp(face.nodes[0],face.nodes[1],face.nodes[2]) == 0)
            return(i);
    }
    return (-1);
}

int ElemT4n3D::getFaceHash(int iFace)
{
	int hash = 0;
	for (unsigned int i=0; i<3;i++)
		hash += _nodes[ T4n3D_Faces[iFace*4+i] - 1 ];
	return hash;
}

void ElemT4n3D::fixNodesOrder()
{
    double n[3];
    double t;
    Node *ns[4];
    int tmp;

    for (int i=0; i<4;i++)
        ns[i] =	&_parent->_nodes[_nodes[i]-1];

    t = normalny4p(ns[0]->_Coords,ns[1]->_Coords,ns[2]->_Coords,ns[3]->_Coords,
                   n);

    if (t>=0) {
        tmp = _nodes[0];
        _nodes[0] = _nodes[1];
        _nodes[1] = tmp;
    }
	

}
		
int ElemT4n3D::whichEdge(CEdge edge)
{
    int i;
    CEdge ed;
    for (i=0;i<6;i++) {
        ed = getEdge(i);
        if (ed.cmp(edge.n1,edge.n2) >= 0)
            return(i);
    }
    return (-1);
	
}

int ElemT4n3D::get4thofFace(int ifNr)
{
    return( _nodes[ T4n3D_Faces[ifNr*4+3] - 1] );
}

void ElemT4n3D::getNormalOfFace(int ifNr, double n[3])
{
    Node ns[4];
    int n1,n2,n3,n4;
    for (int i=0; i<4;i++)
        ns[i] =	_parent->_nodes[_nodes[i]-1];

    n1 = T4n3D_Faces[ifNr*4]-1;
    n2 = T4n3D_Faces[ifNr*4+1]-1;
    n3 = T4n3D_Faces[ifNr*4+2]-1;
    n4 = T4n3D_Faces[ifNr*4+3]-1;
    normalny4p(ns[ n1 ]._Coords, ns[ n2 ]._Coords, ns[ n3 ]._Coords, ns[ n4 ]._Coords, n);
}

CEdge ElemT4n3D::getAlternateEdge( int ieNr)
{
    CEdge ed;
    ed.n1 = _nodes[ T4n3D_Edges_Alternate[ieNr*2+0] - 1];
    ed.n2 = _nodes[ T4n3D_Edges_Alternate[ieNr*2+1] - 1];
	
    return ed;
}

CEdge ElemT4n3D::getEdge(int i)
{
    CEdge e;
    if (i < 6) {
        e.n1 = _nodes[ T4n3D_Edges[i*2] -1 ];
        e.n2 = _nodes[ T4n3D_Edges[i*2+1] -1 ];
    } else {
        printf("Incorrect edge number: %d",i);
    }
    return e;
}

int ElemT4n3D::getNeighbourWithFaceNr(int ifNr)
{
	return( _parent->getElemNbr( nr, ifNr) );
}

int ElemT4n3D::getNeighbourWithFace(CFace face)
{
	return( _parent->getElemNbr( nr, whichFace(face ) ) );
}

void ElemT4n3D::getNeighboursWithEdge( CEdge edge, ElemList &el)
{
    Elem *e;
    unsigned int i,j;
	vector<int> * _nbrs;
	
    el.clear();
	
	/** Przesuzkuje wszystkie wezly.
    Sprawdzam do ktorych elementow naleza wezly elementu glownego (this).  */
    for (i = 0;i<4;i++) {
		/** poniewaz Elem przechowuje numer wezla, musze pobrac
        referencje do wezla z siatki bedacej moim rodzicem.*/
		_nbrs = _parent->getNodeNbrs( _nodes[i] );
        for (j = 0; j < _nbrs->size(); j++) {
			/** teraz pobieram wszystkie elementy do ktorych nalezy dany wezel */
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

CFace ElemT4n3D::getFace(int ifNr)
{
    CFace f(3);
    int i;
    for (i=0; i<3;i++)
        f.nodes[i] = _nodes[ T4n3D_Faces[ifNr*4+i] - 1 ];
    return f;
}

CFace ElemT4n3D::getFaceOrder(int ifNr)
{
    int i;
    int tf[4];
    Node *ns[4];
    double n[3];
    int tmp;
    double t;

    for (i=0; i<4;i++)
        tf[i] = _nodes[ T4n3D_Faces[ifNr*4+i] - 1 ];

    for (int i=0; i<4;i++)
        ns[i] =	&_parent->_nodes[ tf[i]-1 ];

    t = normalny4p(ns[0]->_Coords,ns[1]->_Coords,ns[2]->_Coords,ns[3]->_Coords, n);

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

bool ElemT4n3D::isPointInside( double P[3] )
{
    checkBBox();

    if ((P[0] >= bbox[0]) &&
         (P[1] >= bbox[1]) &&
         (P[2] >= bbox[2]) &&
         (P[0] <= bbox[3]) &&
         (P[1] <= bbox[4]) &&
         (P[2] <= bbox[5])) {

        double n[4][3];
        Node *ns[4];
        for (int i=0; i<4;i++) {
            ns[i] =	&_parent->_nodes[_nodes[i]-1];
            SET(n[i], ns[i]->_Coords);
        }
        return IS_Point_In_Tetra(P, n[0], n[1], n[2], n[3]);
    }

    return false;
}

double ElemT4n3D::calcValPtv( double P[3], vector<double> v )
{
    int i;
    double nodes[4][3];
    double _v[4];
    Node *ns[4];
    //double tmp[3];
	
    for (int i=0; i<4;i++) {
        ns[i] =	&_parent->_nodes[_nodes[i]-1];
        SET(nodes[i], ns[i]->_Coords);
    }
    //SET(tmp,nodes[3]);
    //SET(nodes[3], nodes[2]);
    //SET(nodes[2],tmp);
	
    for (i=0;i<4;i++)
        _v[i] = v[i];

    return(valTetraPtv(P, nodes, _v));

}

void ElemT4n3D::setNodes( int n1, int n2, int n3, int n4 )
{
    setNode(0,n1);
    setNode(1,n2);
    setNode(2,n3);
    setNode(3,n4);
}

double ElemT4n3D::getMaxEdgeLength()
{
	Node *n1, *n2;
	double max = 0, d;
	for (int i = 0 ; i < 6; i++) {
		n1 = &_parent->_nodes[ _nodes[T4n3D_Edges[i * 2]    -1] - 1];
		n2 = &_parent->_nodes[ _nodes[T4n3D_Edges[i * 2 + 1]-1] - 1];
		d = DIST_Point_Point( n1->_Coords, n2->_Coords );
		if (max < d) max = d;
	}
	return max;
}

int ElemT4n3D::getPointEdgeDistances( double P[3], vector<double> & dist )
{
	Node *n1, *n2;
	double dn[3];

	dist.resize(6);

	for (int i = 0 ; i < 6; i++) {
		n1 = &_parent->_nodes[ _nodes[T4n3D_Edges[i * 2]    -1] - 1];
		n2 = &_parent->_nodes[ _nodes[T4n3D_Edges[i * 2 + 1]-1] - 1];
		SUB(dn,n2->_Coords, n1->_Coords);
		dist[i] = DIST_Point_Line( P, dn, n1->_Coords );
		
	}
	return 6;
}

void ElemT4n3D::updateInfo()
{

}

void setTriangle( Tri &tri, int n1, int n2 ,int n3, double pp[4][3], vector<double> vv[3], int fd)
{
	SET(tri.p[0].c, pp[n1] );
	SET(tri.p[1].c, pp[n2] );
	SET(tri.p[2].c, pp[n3] );

	tri.v[0].resize( 0 );
	tri.v[1].resize( 0 );
	tri.v[2].resize( 0 );
	for (int k = 0; k < fd; k++) {
		tri.v[0].push_back( vv[n1][k] );
		tri.v[1].push_back( vv[n2][k] );
		tri.v[2].push_back( vv[n3][k] );
	}
}

bool ElemT4n3D::isecPlane( Point p1, Point n, vector<Tri> & triangles, SimpleField* fields )
{
	Node *n1, *n2;
	double pp[4][3];
	double ppt[3];
	vector<double> vv[4];
	int j = 0;
	double t;
	int e1,e2;

	for (int i = 0 ; i < 6; i++) {
		e1 = _nodes[T4n3D_Edges[i * 2]    -1] - 1;
		e2 = _nodes[T4n3D_Edges[i * 2 + 1]-1] - 1;
		n1 = &_parent->_nodes[ e1 ];
		n2 = &_parent->_nodes[ e2 ];

		if (ISEC_Segment_Plane(p1.c, n.c, n1->_Coords,n2->_Coords,ppt,t) == 1) {
			bool bFound = false;
			for (int k = 0; k < j; k++) {
				if (DIST_Point_Point(ppt, pp[k]) < 1e-7)
					bFound = true;
			}

			if (bFound == false) {
				SET(pp[j],ppt);

				if (fields != 0) {
					vv[j].resize( fields->dim() );

					for (int k = 0; k < fields->dim(); k++) {
						//printf("before new val... %d %d %d\n",k, e1, e2);
						vv[j][k] = fields->val(k,e1) + t * (fields->val(k,e2) - fields->val(k, e1)); 
						//printf("new field val v[%d][%d] = %g", j,k, vv[j][k]);
					}
				}
				j++;
			}
		}
	}
	//printf("Finished intersecting.\n");
	if (j==3) {
		triangles.resize(1);
		
		if (fields == 0) {
			setTriangle( triangles[0], 0, 1 ,2, pp, vv, 0 );
		} else {
			setTriangle( triangles[0], 0, 1 ,2, pp, vv, fields->dim() );
		}
		return true;
	}
	if (j==4) {
		triangles.resize(2);
		
		if (fields == 0) {
			setTriangle( triangles[0], 0, 1 ,2, pp, vv, 0 );
		} else {
			setTriangle( triangles[0], 0, 1 ,2, pp, vv, fields->dim() );
		}

		// Find the maximum angle between vectors
		// constructed of 4th vertex and 1st, 2nd, 3rd
		double v1[3],v2[3],v3[3];
		double d1,d2,d3;
		SUB(v1,pp[0],pp[3]);
		SUB(v2,pp[1],pp[3]);
		SUB(v3,pp[2],pp[3]);
		VERSOR(v1,v1);
		VERSOR(v2,v2);
		VERSOR(v3,v3);
		d1 = DOT(v1,v2);
		d2 = DOT(v2,v3);
		d3 = DOT(v1,v3);
		if (d1 < d2) {
			if (d3 < d1) { // d3 min
				if (fields == 0) {
					setTriangle( triangles[1], 3, 0 ,2, pp, vv, 0 );
				} else {
					setTriangle( triangles[1], 3, 0 ,2, pp, vv, fields->dim() );
				}
			} else { // d1  min
				if (fields == 0) {
					setTriangle( triangles[1], 3, 0 ,1, pp, vv, 0 );
				} else {
					setTriangle( triangles[1], 3, 0 ,1, pp, vv, fields->dim() );
				}
			}
		} else {
			if (d3 < d2) { // d3 min
				if (fields == 0) {
					setTriangle( triangles[1], 3, 0 ,2, pp, vv, 0 );
				} else {
					setTriangle( triangles[1], 3, 0 ,2, pp, vv, fields->dim() );
				}
			} else { // d2 min
				if (fields == 0) {
					setTriangle( triangles[1], 3, 1 ,2, pp, vv, 0 );
				} else {
					setTriangle( triangles[1], 3, 1 ,2, pp, vv, fields->dim() );
				}
			}
		}
		return true;
	}
	return false;
}


