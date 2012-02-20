//
// C++ Interface: fvboundbox
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVBOUNDBOX_H
#define FVBOUNDBOX_H

#include <fvobject.h>
#include <geom.h>
#include <fvrequiregrid.h>

/**
Implements drawing of bounding box of specified object.

@author Robert Szmurlo
*/
class FVBoundBox : public FVObject
{
Q_OBJECT
		
public:
	FVBoundBox(FVBoxMgr * manager, int x = 0, int y = 0);
	~FVBoundBox();
	
	virtual void paintGL();
	virtual void update();
	

	void setBBox( double _p1[3], double _p2[3] );
	void DrawCube(float xPos, float yPos, float zPos);
	void DrawScale(int n, 
	               float vp, float vk,
	               float x1, float y1, float z1, 
	               float x2, float y2, float z2);
	void drawGrid();
	void drawLines( int n, double p1x, double p1y, double p1z, double p2x, double p2y, double p2z, double p3x, double p3y, double p3z );
		
public slots:
	void slotApply();
	
protected:
	double p1[3], p2[3], c[3];
	double dx,dy,dz;
	double lineThickness;
	double boxSize;
	bool showScale;
	bool showGrid;

	void readAttribs();
	virtual void setupAttributes();
};

#endif
