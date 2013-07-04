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

#include <qapplication.h>

#if QT_VERSION >= 0x040000
# include "ui_blobWarWindow.Qt4.h"
  class BlobWarWindow : public QMainWindow, public Ui::BlobWarWindow
  {
  public:
    BlobWarWindow() { setupUi(this); }
  };
#else
# if QT_VERSION >= 0x030000
#  include "blobWarWindow.Qt3.h"
# else
#  error "No .ui file available for Qt 2"
# endif
#endif

int main(int argc, char * argv[])
{
  QApplication application(argc,argv);

  BlobWarWindow bww;
  
#if QT_VERSION < 0x040000
  application.setMainWidget(&bww);
#else
  bww.setWindowTitle("blobWar");
#endif

  bww.show();
  
  return application.exec();
}

