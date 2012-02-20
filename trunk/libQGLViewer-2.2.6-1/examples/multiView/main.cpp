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

#include "multiView.h"
#include <qapplication.h>
#include <qsplitter.h>

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  // Create Splitters
  QSplitter *hSplit  = new QSplitter(Qt::Vertical);
  QSplitter *vSplit1 = new QSplitter(hSplit);
  QSplitter *vSplit2 = new QSplitter(hSplit);

  // Create the scene
  Scene* s = new Scene();

  // Instantiate the viewers.
  Viewer side  (s,0,vSplit1);
  Viewer top   (s,1,vSplit1, &side);
  Viewer front (s,2,vSplit2, &side);
  Viewer persp (s,3,vSplit2, &side);

  
#if QT_VERSION < 0x040000
  application.setMainWidget(hSplit);
#else
  hSplit->setWindowTitle("multiView");
#endif

  // Set main QSplitter as the main widget.
  hSplit->show();

  return application.exec();
}
