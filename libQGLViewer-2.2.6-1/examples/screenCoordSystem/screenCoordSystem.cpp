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

#include "screenCoordSystem.h"
#include <stdio.h>
#include <stdlib.h> // RAND_MAX

using namespace qglviewer;
using namespace std;

void Viewer::init()
{
  for (int i=0; i<nbSaucers; i++)
    {
      Vec pos;
      pos.x = rand() / static_cast<float>(RAND_MAX) - 0.5;
      pos.y = rand() / static_cast<float>(RAND_MAX) - 0.5;
      pos.z = rand() / static_cast<float>(RAND_MAX) - 0.5;

      Quaternion ori(Vec(static_cast<float>(rand()) / RAND_MAX,
			  static_cast<float>(rand()) / RAND_MAX,
			  static_cast<float>(rand()) / RAND_MAX),
		      rand() / static_cast<float>(RAND_MAX) * M_PI);

      saucerPos[i].setPosition(pos);
      saucerPos[i].setOrientation(ori);

      saucerColor[i].setRgb(int(255.0 * rand() / static_cast<float>(RAND_MAX)),
			    int(255.0 * rand() / static_cast<float>(RAND_MAX)),
			    int(255.0 * rand() / static_cast<float>(RAND_MAX)));
    }

  restoreStateFromFile();
  help();
}

QString Viewer::helpString() const
{
  QString text("<h2>S c r e e n C o o r d S y s t e m</h2>");
  text += "This example illustrates the <i>startScreenCoordinatesSystem()</i> function ";
  text += "which enables a GL drawing directly into the screen coordinate system.<br><br>";
  text += "The arrows are drawned using this method. The screen projection coordinates ";
  text += "of the objects is determined using <code>camera()->projectedCoordinatesOf()</code>, ";
  text += "thus <i>attaching</i> the 2D arrows to 3D objects.";
  return text;
}

void Viewer::drawSaucer() const
{
  static GLUquadric* quadric = gluNewQuadric();

  glTranslatef(0.0, 0.0, -0.014);
  gluCylinder(quadric, 0.015, 0.03, 0.004, 32, 1);
  glTranslatef(0.0, 0.0, 0.004);
  gluCylinder(quadric, 0.03, 0.04, 0.01, 32, 1);
  glTranslatef(0.0, 0.0, 0.01);
  gluCylinder(quadric, 0.05, 0.03, 0.02, 32, 1);
  glTranslatef(0.0, 0.0, 0.02);
  gluCylinder(quadric, 0.03, 0.0, 0.003, 32, 1);
  glTranslatef(0.0, 0.0, -0.02);
}

void Viewer::draw()
{
  static Vec proj[nbSaucers];

  int i;
  // Draw 3D flying saucers
  for (i=0; i<nbSaucers; i++)
    {
      glPushMatrix();
      glMultMatrixd(saucerPos[i].matrix());
      qglColor(saucerColor[i]);
      drawSaucer();
      glPopMatrix();
    }

  // Draw the arrows
  qglColor(foregroundColor());
  startScreenCoordinatesSystem();
  for (i=0; i<nbSaucers; i++)
    {
      glBegin(GL_POLYGON);
      proj[i] = camera()->projectedCoordinatesOf(saucerPos[i].position());
      // The small z offset makes the arrow slightly above the saucer, so that it is always visible
      glVertex3fv(proj[i] + Vec(-55, 0, -0.001));
      glVertex3fv(proj[i] + Vec(-17,-5, -0.001));
      glVertex3fv(proj[i] + Vec( -5, 0, -0.001));
      glVertex3fv(proj[i] + Vec(-17, 5, -0.001));
      glEnd();
    }
  stopScreenCoordinatesSystem();

  // Draw text id
  glDisable(GL_LIGHTING);
  for (i=0; i<nbSaucers; i++)
    drawText(int(proj[i].x)-60, int(proj[i].y)+4, QString::number(i));
  glEnable(GL_LIGHTING);
}


