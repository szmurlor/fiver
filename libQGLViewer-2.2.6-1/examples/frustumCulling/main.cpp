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
#include <qapplication.h>

using namespace qglviewer;

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  // Create octree AABBox hierarchy
  const qglviewer::Vec p(1.0, 0.7, 1.3);
  Box::Root = new Box(-p, p);
  Box::Root->buildBoxHierarchy(4);

  // Instantiate the two viewers.
  Viewer viewer, observer;

  // Give v a cullingCamera;
  Camera* c = viewer.camera();
  CullingCamera* cc = new CullingCamera();
  viewer.setCamera(cc);
  delete c;
  
  // Both viewers share the culling camera
  viewer.setCullingCamera(cc);
  observer.setCullingCamera(cc);

  // Place observer 
  observer.setSceneRadius(10.0);
  observer.camera()->setViewDirection(qglviewer::Vec(0.0, -1.0, 0.0));
  observer.showEntireScene();

  // Make sure every culling Camera movement updates the outer viewer
  QObject::connect(viewer.camera()->frame(), SIGNAL(manipulated()), &observer, SLOT(updateGL()));
  QObject::connect(viewer.camera()->frame(), SIGNAL(spun()), &observer, SLOT(updateGL()));


#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&viewer);
#else
  viewer.setWindowTitle("frustumCulling");
  observer.setWindowTitle("scene observer");
#endif

  // Show the viewers' windows.
  viewer.show();
  observer.show();

  return application.exec();
}
