/*
 This file is part of the VRender library.
 Copyright (C) 2005 Cyril Soler (Cyril.Soler@imag.fr)
 Version 1.0.0, released on June 27, 2005.

 http://artis.imag.fr/Members/Cyril.Soler/VRender

 VRender is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 VRender is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with VRender; if not, write to the Free Software Foundation, Inc.,
 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

/****************************************************************************

 Copyright (C) 2002-2006 Gilles Debunne (Gilles.Debunne@imag.fr)

This file is part of the QGLViewer library.
 Version 2.2.6-1, released on July 4, 2007.

 http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer

 libQGLViewer is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 libQGLViewer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with libQGLViewer; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

#include "Vector2.h"
#include "Vector3.h"
#include <math.h>

#ifdef WIN32
# include <windows.h>
#endif

using namespace vrender;
using namespace std;

const Vector2 Vector2::inf(FLT_MAX, FLT_MAX);

//! Default constructor
Vector2::Vector2 ()
{
  _xyz[0] = 0.0;
  _xyz[1] = 0.0;
}

// -----------------------------------------------------------------------------
//! Default destructor
Vector2::~Vector2 ()
{
}

// -----------------------------------------------------------------------------
//! Copy constructor
Vector2::Vector2 (const Vector2& u)
{
  setXY(u[0],u[1]);
}

// -----------------------------------------------------------------------------
//! Create a vector from real values
Vector2::Vector2 (double x,double y)
{
  setXY(x,y);
}

Vector2::Vector2 (const Vector3& u)
{
  _xyz[0] = u[0];
  _xyz[1] = u[1];
}

// -----------------------------------------------------------------------------
//! Inverse
Vector2 vrender::operator- (const Vector2& u)
{
  return Vector2(-u[0], -u[1]) ;
}


// -----------------------------------------------------------------------------
//! Left multiplication by a real value
Vector2 operator* (double r,const Vector2& u)
{
  return Vector2(r*u[0], r*u[1]) ;
}


// -----------------------------------------------------------------------------
//! Norm
double Vector2::norm () const
{
  return sqrt( _xyz[0]*_xyz[0] + _xyz[1]*_xyz[1] );
}

// -----------------------------------------------------------------------------
//! Square norm (self dot product)
double Vector2::squareNorm () const
{
  return _xyz[0]*_xyz[0] + _xyz[1]*_xyz[1] ;
}

// -----------------------------------------------------------------------------
//! Infinite norm
double Vector2::infNorm() const
{
  return max(fabs(_xyz[0]),fabs(_xyz[1])) ;
}


// -----------------------------------------------------------------------------
//! Out stream override: prints the 3 vector components
std::ostream& operator<< (std::ostream& out,const Vector2& u)
{
  out << u[0] << " " << u[1] ;
  return ( out );
}

Vector2 Vector2::mini(const Vector2& v1,const Vector2& v2)
{
  return Vector2(min(v1[0],v2[0]),min(v1[1],v2[1])) ;
}

Vector2 Vector2::maxi(const Vector2& v1,const Vector2& v2)
{
  return Vector2(max(v1[0],v2[0]),max(v1[1],v2[1])) ;
}

