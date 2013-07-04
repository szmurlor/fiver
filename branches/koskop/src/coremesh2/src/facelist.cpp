//
// C++ Implementation: facelist
//
// Description: 
//
//
// Author: Robert Szmurlo <szmurlor@iem.pw.edu.pl>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "facelist.h"
#include "node.h"


FaceList::FaceList()
{
	iCount = 0;
	_last = 0;
	_first = 0;
	bAutoDelete = true;
}


FaceList::~FaceList()
{
	clear();
	_first = 0;
}


/*!
    \fn FaceList::find(int n1, int n2, int n3)
 */
CFace *FaceList::find(int n1, int n2, int n3)
{	
	CFace *tmp;
	
	tmp = _first;
	while (tmp != 0) {
		if (tmp->cmp(n1,n2,n3) == 0) {
			return tmp;
		}
		tmp = tmp->next;
	}   
	
	return 0; 
}


/*!
    \fn NodeList::addNew()
 */
CFace *FaceList::addNew()
{
	CFace *tmp;
	tmp = new CFace();
	tmp->next = 0;
	return( add( tmp ) );
}


/*!
    \fn NodeList::add( PNode pe)
 */
CFace *FaceList::add( CFace *pn)
{
	if (_last != 0)
		_last->next = pn;
	if (_first == 0)
		_first = pn;
	_last = pn;

	return( pn );

	iCount++;
}


/*!
    \fn NodeList::clear()
 */
void FaceList::clear()
{
	CFace *tmp;
	tmp = _first;
	while (tmp != 0) {		
		_first = tmp->next;
		if (bAutoDelete == true) {
			delete(tmp);
		}
		tmp = _first;
	}
	iCount = 0;
	_first = 0;
	_last = 0;
}




/*!
    \fn NodeList::CNode *addNew(Node * _n1, Node * _n2)
 */
CFace * FaceList::addNew(int n1, int n2, int n3)
{
	CFace *t;
	t = addNew();
	t->nodes[0] = n1;
	t->nodes[1] = n2;
	t->nodes[2] = n3;
	
	return(t);
}

void FaceList::debug()
{
	int i = 1;
	CFace *t = _first;
	while (t!= 0) {
		t->Tag = i;
		t->debug();		
		t = t->next;
		i++;
	}
}

int CFace::cmp(int n1, int n2, int n3)
{
	bool b1,b2,b3;
	int i;
	b1 = b2 = b3 = false;
	for (i=0;i<3;i++) if (nodes[i] == n1) b1 = true;
	for (i=0;i<3;i++) if (nodes[i] == n2) b2 = true;
	for (i=0;i<3;i++) if (nodes[i] == n3) b3 = true;
	
	if ((b1==true) && (b2==true) && (b3==true))
		return 0;
	else
		return -1;
}

void CFace::debug()
{
	printf("Face[%d]: %d, %d, %d\n",Tag,nodes[0], nodes[1], nodes[2]);
}

CFace & CFace::operator=( const CFace & src )
{
    Tag = src.Tag;
    next = src.next;
    nodes = src.nodes;
    coors = src.coors;
	return (*this);
}

CFace::CFace( int isize )
{
    nodes.resize(isize);
    Tag = 0;
    next = 0;
}

void CFace::resize( int isize )
{
    nodes.resize( isize );
}

void CFace::fill( std::vector<Node> & vecNodes )
{
    Node * n;
    coors.resize( nodes.size() * 3 );
    for (unsigned int i = 0; i < nodes.size(); i++) {
        n = &vecNodes[ nodes[i] - 1];
        coors[i*3+0] = n->x();
        coors[i*3+1] = n->y();
        coors[i*3+2] = n->z();
    }
}

void CFace::shrink( double s )
{
    double x=0,y=0,z=0;
    for (unsigned int i = 0; i < nodes.size(); i++) {
        x += coors[i*3+0];
        y += coors[i*3+1];
        z += coors[i*3+2];
    }
    x /= nodes.size();
    y /= nodes.size();
    z /= nodes.size();

    for (unsigned int i = 0; i < nodes.size(); i++) {
        coors[i*3+0] = x + s * (coors[i*3+0] - x) ;
        coors[i*3+1] = y + s * (coors[i*3+1] - y) ;
        coors[i*3+2] = z + s * (coors[i*3+2] - z) ;
    }
    
}

void CFace::shrink( std::vector<double> center, double s )
{
    for (unsigned int i = 0; i < nodes.size(); i++) {
        for (int j = 0; j < 3; j++)
        coors[i*3+j] = center[j] + s * (coors[i*3+j] - center[j]) ;
    }
}

double CFace::x(int i)
{
    return coors[ i * 3 ];
}

double CFace::y(int i)
{
    return coors[ i * 3 + 1 ];
}

double CFace::z(int i)
{
    return coors[ i * 3 + 2 ];
}

double CFace::coor( int dim, int i )
{
    return coors[ i * 3 + dim ];
}

void CFace::computeHash()
{
	hashVal = 0;
	for (unsigned int i = 0; i < nodes.size(); i++)
		hashVal +=  nodes[i];
}
