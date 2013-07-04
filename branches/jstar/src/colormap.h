#ifndef _COLORMAP_IS_INCLUDED
#define _COLORMAP_IS_INCLUDED

#include <qgl.h>
#include <iostream>
#include <geom.h>
#include <vector>

class ColorMap
{

protected:
	std::vector<double> vr;
	std::vector<double> vg;
	std::vector<double> vb;

	double vmin, vmax, df;
	bool logScale;
	bool bDoGLInit;
	
	void divideTriangleValue(double tv, int ind1, int ind2, int ind3, 
								double elem[3][3], double elem1[3][3], double elem2[3][3], 
								double v[3], double v1[3], double v2[3] );
								
	bool splitTriangleColor( double elem[3][3], double elem1[3][3], double elem2[3][3], double v[3], double v1[3], double v2[3] );
	
	void drawTriangleColor( double elem[3][3], double v[3] );
	
	int findSeparators( double v1, double v2, double *sep);

public:
	enum ColorMapType { Jet, Gray, XRay };

	double norm[3];
	ColorMap();
	~ColorMap();

	void initJet(int n = 100);
	void initGray(int n = 100);
	void initXRay(int n = 100);
	void init( ColorMapType type, int n = 100 );

	void drawTriangleGL( double e[3][3], double v[3] );
	void setMinMax( double _vmin, double _vmax, bool _logScale = false );
	void setDoGLInit( bool v ) { bDoGLInit = v; }

	void getRGBColor(double v, double &r, double &g, double &b);
};

#endif
