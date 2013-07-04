#include <math.h>
#include "colormap.h"

#define sqr(x) ((x)*(x))
#define LENGTH2D(x) (sqrt( sqr(x[0]) + sqr(x[1]) ))

ColorMap::ColorMap()
{
    norm[0] = 0;
    norm[1] = 0;
    norm[2] = 1;
	logScale= false;
	vmin = 0;
	vmax = 1;
	df = 1;
	bDoGLInit = true;
	initJet();
}

ColorMap::~ColorMap()
{
}

void ColorMap::getRGBColor(double v, double &r, double &g, double &b)
{
	double tv;
	int pos;

	if (df != 0) {
		if( logScale )
			tv= log10(v/vmin) / df;
		else
			tv = (v - vmin) / df;
		
		pos = (int) (tv * (double) vr.size());
		if (pos < 0) pos = 0;
		if (pos >= (int) vr.size()) pos = vr.size()-1;
		b = vb[pos];
		g = vg[pos];
		r = vr[pos];
	} else {
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}
}


void ColorMap::divideTriangleValue(double tv, int ind1, int ind2, int ind3, double elem[3][3], double elem1[3][3], double elem2[3][3], double v[3], double v1[3], double v2[3] )
{
  int _ind1, _ind2;
  if (ind1 < ind2) {
    _ind1 = ind1;
    _ind2 = ind2;
  } else {
    _ind1 = ind2;
    _ind2 = ind1;
  }
  double df_tmp = (v[_ind2] - v[_ind1]);

  elem1[0][0] = elem[_ind1][0] + (tv - v[_ind1]) / df_tmp * ( elem[_ind2][0] - elem[_ind1][0] );
  elem1[0][1] = elem[_ind1][1] + (tv - v[_ind1]) / df_tmp * ( elem[_ind2][1] - elem[_ind1][1] );
  elem1[0][2] = elem[_ind1][2] + (tv - v[_ind1]) / df_tmp * ( elem[_ind2][2] - elem[_ind1][2] );

  elem2[0][0] = elem1[0][0];
  elem2[0][1] = elem1[0][1];
  elem2[0][2] = elem1[0][2];

  elem1[1][0] = elem[_ind1][0];
  elem1[1][1] = elem[_ind1][1];
  elem1[1][2] = elem[_ind1][2];
  elem2[1][0] = elem[_ind2][0];
  elem2[1][1] = elem[_ind2][1];
  elem2[1][2] = elem[_ind2][2];

  elem1[2][0] = elem[ind3][0];
  elem1[2][1] = elem[ind3][1];
  elem1[2][2] = elem[ind3][2];
  elem2[2][0] = elem[ind3][0];
  elem2[2][1] = elem[ind3][1];
  elem2[2][2] = elem[ind3][2];

  v1[0] = tv;
  v1[1] = v[_ind1];
  v1[2] = v[ind3];

  v2[0] = tv;
  v2[1] = v[_ind2];
  v2[2] = v[ind3];

}

int ColorMap::findSeparators( double v1, double v2, double *sep)
{
  double df4 = df/4;
  double _v1,_v2;
  double s[3];
  double ds;
  int i;
  int sepno = 0;

  if (v1>v2) {
    _v1=v2;
    _v2=v1;
  } else {
    _v1=v1;
    _v2=v2;
  }

  ds = 0.1 * df4;

  for (i=0; i<3;i++) {
    s[i] = vmin + df4 * (i+1);
    if ((_v1 < (s[i]-ds)) && (_v2 > (s[i]+ds))) {
      sep[ sepno ] = s[i];
      sepno++;
    }
  }


  return(sepno);
}

bool ColorMap::splitTriangleColor( double elem[3][3], double elem1[3][3], double elem2[3][3], double v[3], double v1[3], double v2[3] )
{
  int sepno[3];
  double seps[3][3];
  int max_sepno;
  int max_sepind;

  sepno[0] = findSeparators(v[0], v[1], seps[0]);
  sepno[1] = findSeparators(v[0], v[2], seps[1]);
  sepno[2] = findSeparators(v[1], v[2], seps[2]);

  max_sepno = 0;
  max_sepind = 0;
  for (int i = 0; i < 3; i++) {
    if (max_sepno < sepno[i]) {
      max_sepno = sepno[i];
      max_sepind = i;
    }
  }

  if (max_sepno > 0) {

    switch (max_sepind) {
    case 0:
      divideTriangleValue(seps[0][0],0,1,2, elem, elem1, elem2, v, v1, v2);
      break;
    case 1:
      divideTriangleValue(seps[1][0],0,2,1, elem, elem1, elem2, v, v1, v2);
      break;
    case 2:
      divideTriangleValue(seps[2][0],1,2,0, elem, elem1, elem2, v, v1, v2);
      break;
    }
    return(true);
  }

  return (false);

}

void ColorMap::drawTriangleColor( double elem[3][3], double v[3] )
{
  double r,g,b;
  double elem1[3][3];
  double elem2[3][3];
  double v1[3], v2[3];
  

  if (splitTriangleColor(elem, elem1, elem2, v, v1, v2) == true) {
    drawTriangleColor(elem1,v1);
    drawTriangleColor(elem2,v2);

  } else {
	if (bDoGLInit) {
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    	glBegin(GL_TRIANGLES);
	}
    glNormal3d(norm[0], norm[1],norm[2]);
    for (int i = 0; i< 3; i++) {
      getRGBColor(v[i],r,g,b);
      glColor3f(r,g,b);
      glVertex3f( (GLdouble) elem[i][0],(GLdouble) elem[i][1], (GLdouble) elem[i][2] );
    }
	if (bDoGLInit) {
		glEnd();
	}
  }
}

void ColorMap::drawTriangleGL( double e[3][3], double v[3] )
{
    if ((vmin != 0) || (vmax != 0)) {
        drawTriangleColor( e, v );
    } else {
        //std::cerr << "Error! ColorMap::drawTriangleGL() - undefined vmin i vmax. Pleas call setMinMax(..) function first." << std::endl;
    }
}

void ColorMap::setMinMax( double _vmin, double _vmax, bool _logScale )
{
	vmin = _vmin;
	vmax = _vmax;

	if( vmin > vmax ) {
		float tmp= vmin;
		vmin= vmax;
		vmax= tmp;
	}

	logScale= _logScale;
	if( vmin <= 0 )
		logScale= false;

	if( logScale ) {
		df= log10( vmax/vmin );
	} else {
		df = vmax - vmin;
	}
}

void ColorMap::initJet( int n )
{
	double tv = 0;
	vr.resize(0);
	vg.resize(0);
	vb.resize(0);
	while (tv < 1) {
		if (tv <= 0.25) {
			vb.push_back(1);
			vg.push_back(tv * 4);
			vr.push_back(0);
		};
		
		if ((tv > 0.25) && (tv <= 0.5)) {
			vb.push_back(1 - (tv - 0.25) * 4);
			vg.push_back(1);
			vr.push_back(0);
		};
		
		if ((tv > 0.5) && (tv < 0.75)) {
			vb.push_back(0);
			vg.push_back(1);
			vr.push_back((tv - 0.5) * 4);
		};
		
		if (tv >= 0.75) {
			vb.push_back(0);
			vg.push_back(1 - (tv - 0.75) * 4);
			vr.push_back(1);
		};

		tv += 1 / (double) n;
	}
}

void ColorMap::initGray( int n )
{
	double tv = 0;
	vr.resize(0);
	vg.resize(0);
	vb.resize(0);
	while (tv < 1) {
		vb.push_back( tv );
		vg.push_back( tv );
		vr.push_back( tv );
		tv += 1 / (double) n;
	}
}

void ColorMap::initXRay( int n )
{
	double tv = 0;
	vr.resize(0);
	vg.resize(0);
	vb.resize(0);
	while (tv < 1) {
		vb.push_back(1 - tv);
		vg.push_back(1 - tv);
		vr.push_back(1 - tv);
		tv += 1 / (double) n;
	}
}

void ColorMap::init( ColorMapType type, int n )
{
	switch (type) {
	case Jet: 	initJet(n);
	            break;
	case Gray: 	initGray(n);
	            break;
	case XRay: 	initXRay(n);
	            break;
	}
}
