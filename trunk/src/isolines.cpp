//
// C++ Implementation: isolines
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "isolines.h"

IsoLines::IsoLines()
{
	isolines_no = 50;
    isoLineWidth = 1.0;
	df = 0;
	vmin= vmax= 0;
	logScale= false;
}


IsoLines::~IsoLines()
{
}

void IsoLines::drawTriangleGL( double e[3][3], double v[3] )
{
	double Piso[2][3];
	double r,g,b;
	for (int iso = 0;  iso <= isolines_no; iso++) {
		if ( findIsoline(e,v, isolines[iso], Piso) ) {
                        glLineWidth(isoLineWidth);
			glBegin(GL_LINES);
			glDisable( GL_BLEND );
			getRGBColor(isolines[iso],r,g,b);
			glColor3f(r,g,b);	
			glVertex3f( Piso[0][0], Piso[0][1], Piso[0][2] );
			glColor3f(r,g,b);
			glVertex3f( Piso[1][0], Piso[1][1], Piso[1][2] );
			glEnd();
			glLineWidth( 1 ); // return to default linewidth
		}
	}
}

void IsoLines:: setWidth( double width )
{
	isoLineWidth= width;
}

void IsoLines::setMinMax( double _vmin, double _vmax, bool _logScale )
{
	vmin = _vmin;
	vmax = _vmax;

	if( vmin > vmax ) {
		float tmp= vmin;
		vmin= vmax;
		vmax= tmp;
	}
	
	isolines.resize(0);

	logScale= _logScale;
	if( vmin <= 0 )
		logScale= false;

	if( logScale ) {
		df= log10( vmax/vmin );
		for( int i= 0; i <= isolines_no; i++ ) {
			isolines.push_back( vmin * pow( 10, df/isolines_no *i ) );
		}
	} else {
		df = vmax - vmin;
		for (int i = 0;  i <= isolines_no; i++) {
			isolines.push_back( vmin + df/isolines_no * i );
		}
	}
}

bool IsoLines::cutLine(double v[3],double tv, double e[3][3], double P[2][3], int i, int ind1, int ind2)
{
	double resv;

	if (v[ind1]>v[ind2]) {
		if ((tv >= v[ind2]) && (tv < v[ind1])) {
			resv = (tv - v[ind2]) / (v[ind1] - v[ind2]);
			P[i][0] = e[ind2][0] + (e[ind1][0] - e[ind2][0]) * resv;
			P[i][1] = e[ind2][1] + (e[ind1][1] - e[ind2][1]) * resv;
			P[i][2] = e[ind2][2] + (e[ind1][2] - e[ind2][2]) * resv;
			return(true);
		}
	} else {
		if ((tv >= v[ind1]) && (tv < v[ind2])) {
			resv = (tv - v[ind1]) / (v[ind2] - v[ind1]);
			P[i][0] = e[ind1][0] + (e[ind2][0] - e[ind1][0]) * resv;
			P[i][1] = e[ind1][1] + (e[ind2][1] - e[ind1][1]) * resv;
			P[i][2] = e[ind1][2] + (e[ind2][2] - e[ind1][2]) * resv;
			return(true);
		}
	}
	return(false);
}

bool IsoLines::findIsoline( double e[3][3], double v[3], double tv, double P[2][3])
{
	int i = 0;
  // checking 0,1
	if (cutLine(v,tv,e,P,i,0,1))
		i++;
	if (cutLine(v,tv,e,P,i,0,2))
		i++;
	if (cutLine(v,tv,e,P,i,1,2))
		i++;

	if (i>0)
		return (true);
	else
		return (false);

}

void IsoLines::getRGBColor(double v, double &r, double &g, double &b)
{
	double tv;

	if (df != 0) {
		if( logScale )
			tv= log10(v/vmin) / df;
		else
			tv = (v - vmin) / df;

		if (tv <= 0.25) {
			b = 1;
			g = tv * 4;
			r = 0;
		};

		if ((tv > 0.25) && (tv <= 0.5)) {
			b = 1 - (tv - 0.25) * 4;
			g = 1;
			r = 0;
		};

		if ((tv > 0.5) && (tv < 0.75)) {
			b = 0;
			g = 1;
			r = (tv - 0.5) * 4;
		};

		if (tv >= 0.75) {
			b = 0;
			g = 1 - (tv - 0.75) * 4;
			r = 1;
		};
	} else {
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}
}
