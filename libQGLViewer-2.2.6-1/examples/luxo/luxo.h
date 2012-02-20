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

#include <QGLViewer/qglviewer.h>

class Luxo
{
public :
  Luxo();

  void draw(bool names=false);

  qglviewer::ManipulatedFrame* frame(unsigned short i) { return frame_[i]; }
  void setSelectedFrameNumber(unsigned short nb) { selected = nb; }

private :
  // The four articulations of the lamp
  qglviewer::ManipulatedFrame* frame_[4];
  // Used to draw the selected element in a different color
  unsigned short selected;

  void drawCone(float zMin, float zMax, float r1, float r2, int nbSub);
  void drawBase();
  void drawArm();
  void drawCylinder();
  void setColor(unsigned short nb);
  void drawHead();
};


class Viewer : public QGLViewer
{
protected :
  virtual void draw();
  virtual void init();
  virtual void drawWithNames();
  virtual void postSelection(const QPoint& point);
  virtual QString helpString() const;

  void initSpotLight();

private :
  Luxo luxo;
};

