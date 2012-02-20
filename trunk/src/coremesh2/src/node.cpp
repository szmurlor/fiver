#include "node.h"
#include "grid.h"

Node::Node(Grid *aparent)
{
	// Creating Node
	_Coords[0] = 0;
	_Coords[1] = 0;
	_Coords[2] = 0;

	_parent = aparent;
}

Node::Node( )
{
	_parent=0;
	Tag = 0;
}

Node::~Node()
{

}

double Node::x()
{
	return _Coords[0];
}

double Node::y()
{
	return _Coords[1];
}

double Node::z()
{
	return _Coords[2];
}

double& Node::operator[](int i)
{
	return _Coords[i];
}

void Node::debug()
{
	cout << "Node " << nr << " (" << _Coords[0] << "," << _Coords[1] << "," << _Coords[2] << ") [ " ;
	for (unsigned int i=0; i < _bnds.size(); i++)
		cout << _bnds[i] << " ";
	cout << "] ";
}

void Node::getNormalOfBnd( int iBndNr, double norm[3] )
{
	double v[3];
	unsigned int i,j;
	vector<CFace> faces;
	Elem * e;
	int iFace;
	int iNum = 0;
	vector<int> * _nbrs;
	_nbrs = _parent->getNodeNbrs( nr );

	for (i = 0; i< 3;i++) 
		norm[i] = 0;


	for (i = 0; i < _nbrs->size(); i++){		
		e = _parent->_elems[ (*_nbrs)[i] - 1 ];
		faces.clear();
		e->getFacesOnBnd( faces, iBndNr, false );
		if (faces.size() > 1)
			printf("Warning! Number of walls on bnd = %d, in element %d > 1\n", iBndNr, e->nr);
		if (faces.size() == 1) {
			iFace = e->whichFace( faces[0] );
			e->getNormalOfFace( iFace, v );
			for (j = 0; j< 3;j++) 
				norm[j] += v[j];
			iNum++;
		}
	}
	if (iNum > 0) 
		for (i = 0; i< 3;i++) 
			norm[i] /= iNum;
}

void Node::addBoundary(int bnd, bool auto_register)
{
	unsigned int i = 0;
	while (i<_bnds.size()){
		if (_bnds[i] == bnd) 
			return;

		i++;
	}
	_bnds.resize( _bnds.size() + 1 );
	_bnds[_bnds.size() - 1] = bnd;

    if (auto_register == true) {
		if (_parent != 0)
			_parent->addNodeToBoundary(nr,bnd);
    }
}


Node &Node::operator=(Node const& src)
{
	Tag = src.Tag;
	for (int i=0;i<MAX_COORDS;i++)
		_Coords[i] = src._Coords[i];
	_parent = src._parent;
	this->_bnds = src._bnds;
	nr = src.nr;
	return(*this);
}

int Node::isOnBoundary(int bnd)
{
    if (bnd == 0)
        return (_bnds.size()>0);
    else
    	for (unsigned int i = 0;i<_bnds.size();i++)
	    	if (_bnds[i] == bnd)
		    	return 1;

	return 0;
}

void Node::removeNeighbour( int elem_nr )
{
	_parent->removeNodeNbr( nr, elem_nr );
}

bool Node::hasNeighbour( int elem_nr )
{
	return _parent->hasNodeNbr( nr, elem_nr );
}

void Node::addNeighbour( int elem_nr )
{
	if (_parent == 0) printf("_parent == 0 in Node::addNeighbour()\n");
	_parent->addNodeNbr( nr, elem_nr );
}

void Node::getNeighboursWithEdge( int n1, int n2, ElemList &el )
{
	Elem *e;
	el.clear();
	vector<int>::iterator i;
	vector<int> * _nbrs;
	_nbrs = _parent->getNodeNbrs( nr );

	for (i=_nbrs->begin(); i != _nbrs->end(); i++){
		e = _parent->_elems[ (*i) -1 ]; 		
		CEdge ed;
		ed.n1 = n1;
		ed.n2 = n2;
		if (e->whichEdge( ed ) >= 0)
			el.addNew( e->nr );
	}
}

void Node::importBoundaries( int node_nr )
{
	unsigned int i;
	Node *n;
	n = &_parent->_nodes[ node_nr - 1 ];
	for (i=0; i < n->_bnds.size(); i++)
		addBoundary( n->_bnds[ i ], false);	
}

