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

#include "callback.h"
#include <QGLViewer/qglviewer.h>
#include <qapplication.h>
#include <qmessagebox.h>

using namespace std;

void help()
{
  QString text("<h2>C a l l b a c k</h2>");
  text += "This example is conceptually the same as simpleViewer.<br>";
  text += "The difference is that it uses the Qt signal/slot mechanism ";
  text += "instead of deriving the QGLViewer class. ";
  text += "The QGLViewer::drawNeeded() signal is connected to the Scene::draw() method. ";
  text += "The two classes are otherwise completely independant.";

  QMessageBox::information(NULL, "Callback exemple", text);
}

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  // Instantiate the viewer.
  QGLViewer viewer;

  // Restore the previous viewer state.
  viewer.restoreStateFromFile();

  // Create a scene, giving a pointer to the associated viewer.
  Scene s(&viewer);

#if QT_VERSION < 0x040000
  application.setMainWidget(&viewer);
#else
  viewer.setWindowTitle("callback");
#endif

  help();

  viewer.show();

  return application.exec();
}
