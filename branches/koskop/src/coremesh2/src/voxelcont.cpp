//
// C++ Implementation: voxelcont
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "voxelcont.h"

VoxelCont::VoxelCont(int _size, double _bbmin[3], double _bbmax[3])
{
	size = _size;
	SET(bbmin,_bbmin);
	SET(bbmax,_bbmax);
	_voxelsPtv.resize(size * size * size);
	bbstep[0] = (bbmax[0] - bbmin[0]) / (double) size;
	bbstep[1] = (bbmax[1] - bbmin[1]) / (double) size;
	bbstep[2] = (bbmax[2] - bbmin[2]) / (double) size;	
}


VoxelCont::~VoxelCont()
{
	
}

void VoxelCont::getVoxCrd( int v[3], double p[3] )
{
	for (int i = 0; i < 3; i++)
		v[i] = (int) ((p[i]-(p[i]/1e10) - bbmin[i]) / bbstep[i]);
}

void VoxelCont::addVoxInt( int v[3], int ival )
{
	_voxelsPtv[ v[0] + v[1] * size + v[2] * size * size ].add( ival );
	
}
	
VolVoxelPtv * VoxelCont::getVox( int v[3] )
{
	return getVox( v[0], v[1], v[2] );	
}

VolVoxelPtv * VoxelCont::getVox( double p[3] )
{
	int v[3];
	getVoxCrd(v,p);

	return getVox(v);
}

VolVoxelPtv * VoxelCont::getVox( int ix, int iy, int iz )
{
	return &_voxelsPtv[ ix + iy * size + iz * size * size ];
}


int VolVoxelPtv::first()
{
	tmp = head;
	if (tmp != 0) {
		return tmp->ind;
	};

	return -1;
}

void VolVoxelPtv::add( int ie )
{
	VVPtvElem * n = new VVPtvElem;
	n->next = 0;
	n->ind = ie;
	
	if ( head == 0) {
		head = n;
		last = n;
	} else {
		last->next = n;
	}
	last = n;
}

int VolVoxelPtv::next()
{
	if (tmp != 0) {
		if (tmp->next != 0) {
			tmp = tmp->next;
			return(tmp->ind);
		}
	}
	return (-1);
}

void VolVoxelPtv::clear()
{
	//int ic = 0;
	VVPtvElem *t = head;
	while (t!=0) {
		head = t->next;
		delete t;
		t=head;
		//ic++;
	}
	//printf("Cleared %d elements.\n", ic);
}
