//
// C++ Interface: facelist
//
// Description: 
//
//
// Author: Robert Szmurlo <szmurlor@iem.pw.edu.pl>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FACELIST_H
#define FACELIST_H

#include <stdio.h>
#include <vector>

class Node;

class CFace {
    protected:
        std::vector<double> coors;
		int hashVal;
    public:
        int Tag;
        std::vector<int> nodes;
        CFace *next;
        
		CFace() {nodes.resize(4); Tag = 0; hashVal = 0;};
        CFace( int isize );
        ~CFace() {};

        // fill coordinates from nodes vector
        void fill( std::vector<Node> & vecNodes );
        // shrink face (only face is shrinked)
        void shrink( double s );
        void shrink( std::vector<double> center, double s );
        // get coordinates (the first vertex is 0)
        double coor( int dim, int i );
        double x( int i );
        double y( int i );
        double z( int i );
     
        int size() { return nodes.size(); };
        void resize( int isize ); 
        
        int cmp(int n1, int n2, int n3);
        CFace & operator=( const CFace & src );
        
        void debug();
		
		void computeHash();
		double hash() { return hashVal; };
        
};

/**
@author Robert Szmurlo
*/
class FaceList{
		
protected:
	CFace * _first;
	CFace * _last;
	int iCount;	
	
public:
	bool bAutoDelete;
	
  FaceList();

  ~FaceList();
	
	CFace * getFirst() { return(_first); };
	
	CFace * find(int n1, int n2, int n3);
	CFace * addNew();
	CFace * addNew(int n1, int n2, int n3);
	CFace * add(CFace * pn);
	void clear();
	
	void debug();
};

#endif
