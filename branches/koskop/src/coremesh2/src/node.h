#ifndef __NODE_H
#define __NODE_H

#include "meshobject.h"
#include "globalmesh.h"
#include "elemlist.h"

#include <iostream>
#include <vector> 

using namespace std;

class Grid;

class Node : MeshObject
{
	public:
		double _Coords[MAX_COORDS];
		// Taki sobie integer uzywany jako znacznik
		int Tag;
		
		Grid *_parent;
		vector<int> _bnds;
		int nr;

		Node(Grid *aparent);
		Node();
		~Node();

		double x();
		double y();
		double z();

		double& operator[](int i);
		Node &operator=(Node const & src);

		void addBoundary(int bnd, bool auto_register = false);
		/* 1 - is on boundary, 0 - is NOT boundary */
		int isOnBoundary(int bnd = 0);
		void importBoundaries( int node_nr );
		
		// Those functions are reimplemented in Grid::...
		// They are left here for compatibility reasons.
		void removeNeighbour( int elem_nr );
		bool hasNeighbour( int elem_nr );
		void addNeighbour( int elem_nr );
		void getNeighboursWithEdge( int n1, int n2, ElemList &el );
		
		/** Get normal vector to the surface created from the faces
			on the boundary iBndNr and having the node.
		*/
		void getNormalOfBnd( int iBndNr, double norm[3] );

		void setCoords(double c[3]) { 
		  _Coords[0] = c[0];
		  _Coords[1] = c[1];
		  _Coords[2] = c[2];
		}

		void debug();
};

#endif
