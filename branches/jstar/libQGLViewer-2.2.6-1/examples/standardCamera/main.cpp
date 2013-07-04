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

#include "standardCamera.h"
#include "viewer.h"
#include "cameraViewer.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  // Instantiate the two viewers.
  StandardCamera* sc = new StandardCamera();
  Viewer viewer(sc);
  CameraViewer cviewer(sc);

  // Make sure every v camera movement updates the camera viewer
  QObject::connect(viewer.camera()->frame(), SIGNAL(manipulated()), &cviewer, SLOT(updateGL()));
  QObject::connect(viewer.camera()->frame(), SIGNAL(spun()), &cviewer, SLOT(updateGL()));
  // Also update on camera change (type or mode)
  QObject::connect(&viewer, SIGNAL(cameraChanged()), &cviewer, SLOT(updateGL()));

#if QT_VERSION < 0x040000
  application.setMainWidget(&viewer);
#else
  viewer.setWindowTitle("standardCamera");
  cviewer.setWindowTitle("Camera viewer");
#endif

  cviewer.show();
  viewer.show();

  return application.exec();
}
