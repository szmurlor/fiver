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

#include "cullingCamera.h"

using namespace qglviewer;

float CullingCamera::distanceToFrustumPlane(int index, const Vec& pos) const
{
  return pos * Vec(planeCoefficients[index]) - planeCoefficients[index][3];
}

bool CullingCamera::sphereIsVisible(const Vec& center, float radius) const
{
  for (int i=0; i<6; ++i)
    if (distanceToFrustumPlane(i, center) > radius)
      return false;
  return true;
}

bool CullingCamera::aaBoxIsVisible(const Vec& p1, const Vec& p2, bool* entirely) const
{
  bool allInForAllPlanes = true;
  for (int i=0; i<6; ++i)
    {
      bool allOut = true;
      for (unsigned int c=0; c<8; ++c)
	{
	  const Vec pos((c&4)?p1.x:p2.x, (c&2)?p1.y:p2.y, (c&1)?p1.z:p2.z);
	  if (distanceToFrustumPlane(i, pos) > 0.0)
	    allInForAllPlanes = false;
	  else
	    allOut = false;
	}

      // The eight points are on the outside side of this plane
      if (allOut)
	return false;
    }

  if (entirely)
    // Entirely visible : the eight points are on the inside side of the 6 planes
    *entirely = allInForAllPlanes;

  // Too conservative, but tangent cases are too expensive to detect
  return true;
}
