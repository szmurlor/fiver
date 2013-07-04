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

#include "box.h"
#include "cullingCamera.h"

using namespace std;
using namespace qglviewer;

// Static member initialization
Box* Box::Root;

void Box::draw() const
{
  glColor3f(0.3*level, 0.2, 1.0-0.3*level);
  glLineWidth(level+1);

  glBegin(GL_LINE_STRIP);
  glVertex3fv(Vec(p1.x, p1.y, p1.z));
  glVertex3fv(Vec(p1.x, p2.y, p1.z));
  glVertex3fv(Vec(p2.x, p2.y, p1.z));
  glVertex3fv(Vec(p2.x, p1.y, p1.z));
  glVertex3fv(Vec(p1.x, p1.y, p1.z));
  glVertex3fv(Vec(p1.x, p1.y, p2.z));
  glVertex3fv(Vec(p1.x, p2.y, p2.z));
  glVertex3fv(Vec(p2.x, p2.y, p2.z));
  glVertex3fv(Vec(p2.x, p1.y, p2.z));
  glVertex3fv(Vec(p1.x, p1.y, p2.z));
  glEnd();

  glBegin(GL_LINES);
  glVertex3fv(Vec(p1.x, p2.y, p1.z));
  glVertex3fv(Vec(p1.x, p2.y, p2.z));
  glVertex3fv(Vec(p2.x, p2.y, p1.z));
  glVertex3fv(Vec(p2.x, p2.y, p2.z));
  glVertex3fv(Vec(p2.x, p1.y, p1.z));
  glVertex3fv(Vec(p2.x, p1.y, p2.z));
  glEnd();
}

void Box::buildBoxHierarchy(int l)
{
  level = l;
  const Vec middle = (p1+p2) / 2.0;
  for (unsigned int i=0; i<8; ++i)
    {
      // point in one of the 8 box corners
      const Vec point((i&4)?p1.x:p2.x, (i&2)?p1.y:p2.y, (i&1)?p1.z:p2.z);
      if (level > 0)
	{
	  child[i] = new Box(point, middle);
	  child[i]->buildBoxHierarchy(level-1);
	}
      else
	child[i] = NULL;
    }
}

void Box::drawIfAllChildrenAreVisible(const CullingCamera* camera) const
{
  static bool* entirely = new bool;

  if (camera->aaBoxIsVisible(p1, p2, entirely))
    if (*entirely)
      draw();
    else
      if (child[0])
	for (int i=0; i<8; ++i)
	  child[i]->drawIfAllChildrenAreVisible(camera);
      else
	draw();
}
