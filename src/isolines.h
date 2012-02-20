//
// C++ Interface: isolines
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ISOLINES_H
#define ISOLINES_H

#include <qgl.h>
#include <iostream>
#include <geom.h>
#include <vector>

/**
@author Robert Szmurlo
*/
class IsoLines{
protected:
	double vmin, vmax, df;
	std::vector<double> isolines;

	double isoLineWidth;

	bool logScale;
	
	bool findIsoline( double e[3][3], double v[3], double tv, double P[2][3]);
	bool cutLine(double v[3],double tv, double e[3][3], double P[2][3], int i, int ind1, int ind2);
	void getRGBColor(double v, double &r, double &g, double &b);
public:
	int isolines_no;
	IsoLines();

	~IsoLines();
	
	void drawTriangleGL( double e[3][3], double v[3] );
	void setMinMax( double _vmin, double _vmax, bool logScale );
	void setWidth( double width );
	

};

#endif
