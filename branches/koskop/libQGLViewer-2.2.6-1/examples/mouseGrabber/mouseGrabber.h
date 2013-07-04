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

class CameraPathPlayer : public qglviewer::MouseGrabber
{
public:
  CameraPathPlayer(int nb) : pathNb(nb) {};
  void checkIfGrabsMouse(int x, int y, const qglviewer::Camera* const camera);
  int yPos() { return 25*pathNb; };

protected:
  void mousePressEvent(QMouseEvent* const, qglviewer::Camera* const camera) { camera->playPath(pathNb); };

private:
  int pathNb;
};


class Spiral
{
public:
  void draw() const;
  void setPosition(const qglviewer::Vec& pos) { mf_.setPosition(pos); };

private:
  qglviewer::ManipulatedFrame mf_;
};


class Viewer : public QGLViewer
{
protected :
  virtual void init();
  virtual void draw();
  virtual QString helpString() const;

  void displayPlayers();
  void updatePlayers();

private:
  CameraPathPlayer** player_;
  int nbPlayers_;
#if QT_VERSION < 0x040000
  QValueVector<Spiral> spiral_;
#else
  QList<Spiral> spiral_;
#endif
};
