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

#include "fastDraw.h"

using namespace std;
using namespace qglviewer;

void Viewer::init()
{
  // Increase the material shininess, so that the difference between
  // the two versions of the spiral is more visible.
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
  GLfloat specular_color[4] = { 0.8, 0.8, 0.8, 1.0 };
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular_color);

  restoreStateFromFile();
  help();
}

static void drawSpiral(bool simplified = false)
{
  int nbSteps = 600;
  int nbSub = 50;
  if (simplified)
    {
      nbSteps = 60;
      nbSub = 2;
    }

  for (float n=0; n<nbSub; ++n)
    {
      glBegin(GL_QUAD_STRIP);
      for (float i=0.0; i<nbSteps; ++i)
	{
	  float ratio = i/nbSteps;
	  float angle = 21.0*ratio;
	  float radius = 1.0 - 0.5*ratio;

	  Vec center(radius*cos(angle), ratio-0.5, radius*sin(angle));

	  for (unsigned short j=0; j<2; ++j)
	    {
	      float delta = 3.0*(n+j)/nbSub;
	      Vec shift(cos(angle)*cos(delta), sin(delta), sin(angle)*cos(delta));

	      glColor3f(1-ratio, (n+j)/nbSub , ratio);
	      glNormal3fv(shift);
	      glVertex3fv(center+0.2*shift);
	    }
	}
      glEnd();
    }
}

void Viewer::draw()
{
  drawSpiral();
}

void Viewer::fastDraw()
{
  drawSpiral(true);
}

QString Viewer::helpString() const
{
  QString text("<h2>F a s t D r a w</h2>");
  text += "The <code>fastDraw()</code> function is called instead of <code>draw()</code> when the camera ";
  text += "is manipulated. Providing such a simplified version of <code>draw()</code> allows for interactive ";
  text += "frame rates when the camera is moved, even for very complex scenes.";
  return text;
}
