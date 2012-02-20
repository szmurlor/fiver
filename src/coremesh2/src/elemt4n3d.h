//
// C++ Interface: elemt4n3d
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ELEMT4N3D_H
#define ELEMT4N3D_H

#include "elem.h"

class SimpleField;

/**
@author Robert Szmurlo
*/
class ElemT4n3D : public Elem
{
    public:

        ElemT4n3D(Grid *AParent);
        ElemT4n3D() { _parent = 0; };
        ~ElemT4n3D();
	
        virtual void fixNodesOrder(); //ok
        virtual void getFacesOnBnd(vector<CFace> &vfaces, int bnd, bool all = false);
        virtual void getFacesOnBnd(vector<int> &vfaces, int bnd, bool all = false);
        virtual double volume();
        virtual bool isPointInside( double P[3] );
        virtual CEdge getEdge(int i);
        virtual int getNeighbourWithFace(CFace face);
        virtual int getNeighbourWithFaceNr(int ifNr);
        virtual void getNeighboursWithEdge( CEdge edge, ElemList &el);
        virtual double calcValPtv( double P[3], vector<double> v );
        virtual CFace getFace(int ifNr);
        virtual CFace getFaceOrder(int ifNr);
        virtual int whichFace(CFace face);
        virtual void getNormalOfFace(int ifNr, double n[3]);
        virtual int whichEdge(CEdge edge);
        virtual void debug();
        virtual int getFacesCount() { return 4; };
        virtual int getEdgesCount() { return 6; };
		virtual int getFaceHash(int iFace);
		virtual double getMaxEdgeLength();
		virtual int getPointEdgeDistances( double P[3], vector<double> & dist );
        virtual void updateInfo();
  
    /** T4n3D specific methods. */
        int get4thofFace(int ifNr);
        CEdge getAlternateEdge( int ieNr );
        void setNodes( int n1, int n2, int n3, int n4 );

	virtual bool isecPlane( Point p1, Point n, vector<Tri> &triangles, SimpleField* fields );
};

#endif
