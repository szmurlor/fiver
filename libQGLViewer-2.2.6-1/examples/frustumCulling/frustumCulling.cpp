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

#include "frustumCulling.h"
#include "cullingCamera.h"
#include "box.h"

using namespace qglviewer;

void Viewer::draw()
{
  Box::Root->drawIfAllChildrenAreVisible(cullingCamera);

  if (cullingCamera == camera())
    // Main viewer computes its plane equation
    cullingCamera->computeFrustumPlanesEquations();
  else
    {
      // Observer viewer draws cullingCamera
      glLineWidth(4.0);
      glColor4f(1.0, 1.0, 1.0, 0.5);
      cullingCamera->draw();
    }
}

void Viewer::init()
{
  // Restore previous viewer state.
  restoreStateFromFile();

  if (cullingCamera != camera())
    {
      // Observer viewer configuration
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      help();
    }
  
  glDisable(GL_LIGHTING);
}

QString Viewer::helpString() const
{
  QString text("<h2>F r u s t u m C u l l i n g</h2>");

  text += "A hierarchical octree structure is clipped against the camera's frustum clipping planes, obtained ";
  text += "using <code>getFrustumPlanesCoefficients</code>. A second viewer uses <code>drawCamera()</code> to ";
  text += "display an external view of the first viewer's camera.<br><br>";

  text += "This frustum culling implementation is quite naive. Many optimisation techniques are available in ";
  text += "the litterature.";

  return text;
}
