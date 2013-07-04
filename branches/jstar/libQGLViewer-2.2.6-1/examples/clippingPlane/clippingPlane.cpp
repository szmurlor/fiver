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

#include "clippingPlane.h"
#include "QGLViewer/manipulatedFrame.h"

using namespace std;
using namespace qglviewer;

// Draws a spiral
void Viewer::draw()
{
  const float nbSteps = 200.0;

  glBegin(GL_QUAD_STRIP);
  for (int i=0; i<nbSteps; ++i)
    {
      const float ratio = i/nbSteps;
      const float angle = 21.0f*ratio;
      const float c = cos(angle);
      const float s = sin(angle);
      const float r1 = 1.0f - 0.8f*ratio;
      const float r2 = 0.8f - 0.8f*ratio;
      const float alt = ratio - 0.5f;
      const float nor = 0.5f;
      const float up = sqrt(1.0f-nor*nor);
      glColor3f(1.0f-ratio, 0.2f , ratio);
      glNormal3f(nor*c, up, nor*s);
      glVertex3f(r1*c, alt, r1*s);
      glVertex3f(r2*c, alt+0.05f, r2*s);
    }
  glEnd();


  glPushMatrix();
  glMultMatrixd(manipulatedFrame()->matrix());
  // Since the Clipping Plane equation is multiplied by the current modelView, we can define a 
  // constant equation (plane normal along Z and passing by the origin) since we are here in the
  // manipulatedFrame coordinates system (we glMultMatrixd() with the manipulatedFrame matrix()).
  static const GLdouble equation[] = { 0.0, 0.0, 1.0, 0.0 };
  glClipPlane(GL_CLIP_PLANE0, equation);
  
  // Draw a plane representation: Its normal...
  glColor3f(0.8, 0.8, 0.8);
  drawArrow(0.4, 0.015);
  // ...and a quad (with a slightly shifted z so that it is not clipped).
  glBegin(GL_QUADS);
  glVertex3f(-1.0, -1.0, 0.001);
  glVertex3f(-1.0,  1.0, 0.001);
  glVertex3f( 1.0,  1.0, 0.001);
  glVertex3f( 1.0, -1.0, 0.001);
  glEnd();
  glPopMatrix();
}

void Viewer::init()
{
  // Restore previous viewer state.
  restoreStateFromFile();
  
  // Opens help window
  help();
  
  // The ManipulatedFrame will be used to position the clipping plane
  setManipulatedFrame(new ManipulatedFrame());
  
  // Enable plane clipping
  glEnable(GL_CLIP_PLANE0);
}

QString Viewer::helpString() const
{
  QString text("<h2>C l i p p i n g P l a n e</h2>");
  text += "The standard OpenGL <i>GL_CLIP_PLANE</i> feature is used to add an additionnal clipping ";
  text += "plane in the scene, which position and orientation are set by a <b>ManipulatedFrame</b>.<br><br>";
  text += "Hold the <b>Control</b> key pressed down while using the mouse to modify the plane orientation (left button) ";
  text += "and position (right button) and to interactively see the clipped result.<br><br>";
  text += "Since the plane equation is defined with respect to the current modelView matrix, a constant equation (normal";
  text += "along the Z axis) can be used since we transformed the coordinates system using the <b>matrix()</b> method.";

  return text;
}
