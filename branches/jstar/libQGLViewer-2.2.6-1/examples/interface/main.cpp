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
# include "ui_viewerInterface.Qt4.h"
  class ViewerInterface : public QDialog, public Ui::Dialog
  {
  public:
    ViewerInterface() { setupUi(this); }
  };
#else
# include "interface.h"
# if QT_VERSION >= 0x030000
#  include "viewerInterface.Qt3.h"
# else
#  include "viewerInterface.Qt2.h"
# endif
#endif

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  ViewerInterface vi;

#if QT_VERSION < 0x040000
  application.setMainWidget(&vi);
#else
  vi.setWindowTitle("interface");
#endif

  vi.show();

  return application.exec();
}
