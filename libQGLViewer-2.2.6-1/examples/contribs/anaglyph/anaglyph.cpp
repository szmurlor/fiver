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

#include "anaglyph.h"

using namespace qglviewer;
using namespace std;

void Viewer::draw()
{
  static const bool left = true;

  // Draw for left eye
  camera()->loadProjectionMatrixStereo(left);
  camera()->loadModelViewMatrixStereo(left);
  glColor3f(0.0, 0.0, 1.0);
  drawScene();

  // Draw for right eye
  camera()->loadProjectionMatrixStereo(!left);
  camera()->loadModelViewMatrixStereo(!left);
  glColor3f(1.0, 0.0, 0.0);
  drawScene();
}

// Draws a spiral, without changing color
void Viewer::drawScene()
{
  const float nbSteps = 200.0;

  glBegin(GL_QUAD_STRIP);
  for (float i=0; i<nbSteps; ++i)
    {
      float ratio = i/nbSteps;
      float angle = 21.0*ratio;
      float c = cos(angle);
      float s = sin(angle);
      float r1 = 1.0 - 0.8*ratio;
      float r2 = 0.8 - 0.8*ratio;
      float alt = ratio - 0.5;
      const float nor = .5;
      const float up = sqrt(1.0-nor*nor);
      glNormal3f(nor*c, up, nor*s);
      glVertex3f(r1*c, alt, r1*s);
      glVertex3f(r2*c, alt+0.05, r2*s);
    }
  glEnd();
}

void Viewer::init()
{
  // Wireframe display is needed to prevent occlusions
  // between left and right images.
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Restore previous viewer state.
  restoreStateFromFile();

  help();
}

QString Viewer::helpString() const
{
  QString text("<h2>A n a g l y p h</h2>");
  text += "The anaglyph stereo mode displays simultaneously two colored views of the scene.<br><br>";
  text += "You need to wear glasses with colored lenses (here red and blue) to view the stereo image";
  text += "Each eye then sees the associated view, creating the stereo illusion.<br><br>";
  text += "Stereo is best perceived when viewer is full screen (<code>Alt+Enter</code>).<br><br>";
  text += "Simply use the <i>loadModelViewMatrixStereo()</i> and";
  text += "<i>loadProjectionMatrixStereo()</i> camera functions to set appropriate";
  text += "<i>GL_MODELVIEW</i> and <i>GL_PROJECTION</i> stereo matrices.";
  return text;
}
