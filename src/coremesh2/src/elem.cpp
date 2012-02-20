#include "elem.h"
#include "grid.h"

Elem & Elem::operator =( const Elem & src )
{
    _parent = src._parent;
    _elemtype = src._elemtype;
    _isubdomain = src._isubdomain;
    _nodes = src._nodes;
    nr = src.nr;
    Tag = src.Tag;
	_isExternal = false;
	return (*this);
}

Node & Elem::operator [ ]( unsigned int i )
{
    if (_parent == 0) {
        printf("Warning: ElemT4n3D::operator[] _parent = 0\n");
        return _parent->_nodes[0];
    }
    if ((_nodes[i]-1) >= _parent->_nodes.size() ){
        printf("Warning: Elem: %d, Node %d > _parent->_nodes.size() = %d\n",nr,_nodes[i]-1,_parent->_nodes.size());
        return _parent->_nodes[0];
    }
    else
        return _parent->_nodes[_nodes[i]-1];
}

void Elem::setNode( int i, int iv )
{
    _nodes[i] = iv;
}

void Elem::setSubdomain( int subdomain, bool auto_register )
{
    _isubdomain = subdomain;
    if (auto_register) {
        if (_parent !=  0) {
            (*_parent).addElemToSubdomain(nr,subdomain);
        } else {
            cerr << "Warning! Autoregister of element called when grid = 0;" << endl;
        }
    }
}

int Elem::hasNodes( int N1, int N2, int N3 )
{
    unsigned int i;
    for (i=0;i<_nodes.size();i++) {
        if ((int)_nodes[i] == N1) N1 = -1;
        if ((int)_nodes[i] == N2) N2 = -1;
        if ((int)_nodes[i] == N3) N3 = -1;
    };

    return ( ((N1==-1)&&(N2==-1)&&(N3==-1))?1:0);
}

vector< double > Elem::center( )
{
    unsigned int i,j;
    Node *nt;
    vector<double> c;

    c.resize(3);
    for (j=0; j < 3; j++)
        c[j] = 0;
	
    for (j=0; j < 3; j++) {
        for (i=0; i < _nodes.size(); i++) {
            nt = &_parent->_nodes[_nodes[i]-1];
            c[j] += nt->_Coords[j];
        }
        c[j] /= _nodes.size();
    }
    
    return c;
}

void Elem::updateNeighbours( )
{
    unsigned int j;
    Node *nt;
		printf("_nodes.size() = %d\n",_nodes.size());
    for (j=0; j<_nodes.size(); j++)
    {
				printf("in Elem::updateNeighbours() for node: %d\n",_nodes[j]);

        nt = &_parent->_nodes[_nodes[j]-1];
				printf("node = %p\n",nt);
        nt->addNeighbour( nr );
                                printf("after add neighbour\n");
    }
}

void Elem::debug( )
{
    if (_parent==0) return;

    cout << "[ Nr: " << nr << endl;
    cout << "Sub: " << _isubdomain <<  endl;
    unsigned int i = 0;
    while (i<_nodes.size()) {
        _parent->_nodes[_nodes[i]-1].debug();
        cout << endl;
        i++;
    }
    cout << "]" << endl;
}

int Elem::getPointDistances( double P[3], vector<double> & dist )
{
	Node * n;
	dist.resize( _nodes.size() );
	for (unsigned int i=0; i < _nodes.size(); i++) {
        n =	&_parent->_nodes[_nodes[i]-1];
		dist[i] = DIST_Point_Point(P, n->_Coords);
	}
	return (_nodes.size());
}

void Elem::checkBBox( )
{
    double p[3];
    Node *n;
    
    if (bbox == 0) {
        bbox = new double[6];
        bbox[0] = 1e88;
        bbox[1] = 1e88;
        bbox[2] = 1e88;
        bbox[0] = -1e88;
        bbox[1] = -1e88;
        bbox[2] = -1e88;
        
        for (int i=0; i<(int)_nodes.size();i++) {
            n =	&_parent->_nodes[_nodes[i]-1];
            SET(p, n->_Coords);
            
            if (p[0]<bbox[0]) bbox[0] = p[0];
            if (p[1]<bbox[1]) bbox[1] = p[1];
            if (p[2]<bbox[2]) bbox[2] = p[2];
            if (p[0]>bbox[3]) bbox[3] = p[0];
            if (p[1]>bbox[4]) bbox[4] = p[1];
            if (p[2]>bbox[5]) bbox[5] = p[2];
        }

    }
}

bool Elem::touchesBoundary( int bnd_ind )
{
    Node * n;
	for (unsigned int i=0; i < _nodes.size(); i++) {
        n =	&_parent->_nodes[_nodes[i]-1];
        if (n->isOnBoundary( bnd_ind ) == 1)
            return true;
	}
	return false;

}

bool Elem::isExternal()
{
    _parent->checkNbrs();
	return _isExternal;	
}
void Elem::setExternal( bool val )
{
	_isExternal = val;
}

