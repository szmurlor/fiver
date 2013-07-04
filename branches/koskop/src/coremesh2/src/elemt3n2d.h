//
// C++ Interface: elemt3n2d
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ELEMT3N2D_H
#define ELEMT3N2D_H

#include "elem.h"

class Grid;
class SimpleField;

/**
@author Robert Szmurlo
*/
class ElemT3n2D : public Elem
{
    protected:
        double _shpFun[3][3]; // shpFun[0..3] - Ni, Nj, Nk; shpFun[0][0..3] - ai,bi,ci
        double _bshpFun;
        
        void init();
        
    public:
        ElemT3n2D(Grid *AParent);
        ElemT3n2D();
        ~ElemT3n2D();
    
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
		virtual int getFaceHash(int iFace);
        virtual void getNormalOfFace(int ifNr, double n[3]);
        virtual int whichEdge(CEdge edge);
        virtual void debug();
        virtual int getFacesCount() { return 1; };
        virtual int getEdgesCount() { return 3; };
		virtual double getMaxEdgeLength() { return 0; }
		virtual int getPointEdgeDistances( double P[3], vector<double> & dist );
		virtual bool isecPlane( Point p1, Point n, vector<Tri> &triangles, SimpleField* fields );
        virtual void updateInfo();
        
        void setNodes( int n1, int n2, int n3 );
		bool isExternal();
};

#endif
