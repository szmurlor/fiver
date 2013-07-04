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

#include "x3dViewer.h"
#include <qfiledialog.h>

using namespace X3DTK;

void Viewer::init()
{
#ifdef GL_RESCALE_NORMAL
  glEnable(GL_RESCALE_NORMAL);
#endif

  setKeyDescription(Key_L, "Loads a new x3d file");

  loadFile();
  help();
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
  switch (e->key())
  {
    case Qt::Key_L : loadFile(); break;
    default:         QGLViewer::keyPressEvent(e);
  }
}

void Viewer::loadFile()
{
  const QString name = QFileDialog::getOpenFileName("", "X3D files (*.x3d *.X3D);;All files (*)", this);

  // In case of Cancel
  if (name.isEmpty())
    return;

  // Loads the file
  scene.load(name);

  // QGLViewer settings
  qglviewer::Vec bbMin(scene.getBBoxMin().f_data());
  qglviewer::Vec bbMax(scene.getBBoxMax().f_data());
  setSceneBoundingBox(bbMin, bbMax);

  showEntireScene();
}

void Viewer::draw()
{
  scene.draw();
}

QString Viewer::helpString() const
{
  QString text("<h2>x 3 d V i e w e r</h2>");
  text += "This example uses the libX3DToolkit library to load an x3d object file.<br><br>";
  text += "Press <b>L</b>(oad) to open an x3d file.";
  return text;
}
