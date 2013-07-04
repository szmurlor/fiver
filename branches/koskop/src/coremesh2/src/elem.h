#ifndef __ELEM_H
#define __ELEM_H

#include <iostream>
#include <vector>

#include "meshobject.h"
#include "node.h"
#include "geom.h"
#include "edgelist.h"
#include "facelist.h"
#include "nodelist.h"
#include "elemlist.h"
#include "point.h"
#include "tri.h"

using namespace std;

class Grid;
class SimpleField;

typedef enum {T4n3D, T3n2D} ElemType;

class Elem : public MeshObject
{
	protected:
		ElemType _elemtype;
		int _isubdomain;
		double *bbox;
		bool _isExternal;
	
	/** Functional protected methods */
		void checkBBox();
	public:
		Grid *_parent;
		vector<unsigned int> _nodes;
		int nr;
		int Tag; // simple tag.
		int TagForNbr; // simple tag.
		bool toDelete;

		Elem(Grid *AParent) {_parent = AParent;};
		Elem() { _parent = 0; };
        virtual ~Elem() { };
	
		Node &operator[](unsigned int i);
        Elem &operator=(const Elem &src);

	/** Initialization methods */
		virtual void setNode(int i, int iv); // i - index, iv - node number
		void setSubdomain(int subdomain, bool auto_register = false);
  
	/** Informational methods */
		int subdomain() {return (_isubdomain); };
		virtual ElemType elemType() { return(_elemtype); };
		/** Get faces which are on hte boundary: bnd. */
		virtual void getFacesOnBnd(vector<CFace> &_faces, int bnd, bool all = false) = 0;
		virtual void getFacesOnBnd(vector<int> &_faces, int bnd, bool all = false) = 0;
		virtual bool touchesBoundary( int bnd_ind = 0 );

		/** 1 - yes, 0 - not */
		virtual int hasNodes( int N1,int N2, int N3);
		virtual double volume() = 0;
		virtual vector<double> center();
		virtual bool isPointInside( double P[3] ) = 0;
		virtual int getPointDistances( double P[3], vector<double> & dist );
		virtual int getPointEdgeDistances( double P[3], vector<double> & dist ) = 0;
		virtual double getMaxEdgeLength() = 0;
		/** face - must be initialized with proper node numbers wich belong to element.*/
		virtual int getNeighbourWithFace(CFace face) = 0;
		virtual int getNeighbourWithFaceNr(int ifNr) = 0;
		virtual double calcValPtv( double P[3], vector<double> v ) = 0;
		/** Get the face number ifNr, (local face number). Before returning the result
			check the proper order of vertices: should be counterclockwise.*/
		virtual CFace getFaceOrder(int ifNr) = 0;
		virtual int whichFace(CFace face) = 0;
		virtual CFace getFace(int ifNr) = 0;
		virtual int whichEdge(CEdge edge) = 0;
		virtual CEdge getEdge(int i) = 0;
		virtual int getFacesCount() = 0 ;
		virtual int getEdgesCount() = 0 ;
		virtual int getFaceHash(int iFace) = 0;
		/** Get the list of elements which neigbour with CEdge.
			Warning! Pointher to this element is not included in the list. */
		virtual void getNeighboursWithEdge(CEdge edge, ElemList &el) = 0;
		/**Wektor normalny sciany o numerze ifNr, skierowany na zewnatrz elementu.
		Do obliczen uzywany jest czwarty wezel elementu.*/
		virtual void getNormalOfFace(int ifNr, double n[3]) = 0;
		virtual bool isecPlane( Point p1, Point n, vector<Tri> &triangles, SimpleField* fields ) = 0;

	/** Processing methods. */
		virtual void fixNodesOrder() = 0; //ok
		virtual void updateNeighbours();
		/** Update information: is any face of element external to subdomain,
			update the neighbour elements. 
		*/
		virtual void updateInfo() = 0; 
		virtual void debug();

		virtual bool isExternal();
		virtual void setExternal( bool val );
};
#endif
