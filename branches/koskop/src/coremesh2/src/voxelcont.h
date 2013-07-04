//
// C++ Interface: voxelcont
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VOXELCONT_H
#define VOXELCONT_H

/**
@author Robert Szmurlo
*/

#include "geom.h"
#include <vector>

using namespace std;

struct VVPtvElem {
	int ind;
	VVPtvElem * next;
};

class VolVoxelPtv {
	VVPtvElem * head;
	VVPtvElem * tmp;
	VVPtvElem * last;

  public:
  	VolVoxelPtv() { head = 0; tmp = 0; };
	~VolVoxelPtv() { clear(); };
	void add( int ie );
	int first();
	int next();
	void clear();
};


class VoxelCont{

protected:
	vector<VolVoxelPtv> _voxelsPtv;
	double bbmin[3], bbmax[3], bbstep[3];
	int size;
	
	
public:
	VoxelCont(int _size, double _bbmin[3], double _bbmax[3]);
	~VoxelCont();
	
	void getVoxCrd( int v[3], double p[3] );
	void addVoxInt( int v[3], int ival );
	
	VolVoxelPtv * getVox( int v[3] );
	VolVoxelPtv * getVox( double p[3] );
	VolVoxelPtv * getVox( int ix, int iy, int iz );
	
};

#endif
