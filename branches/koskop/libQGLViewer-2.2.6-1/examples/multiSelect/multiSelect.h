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

#include "QGLViewer/qglviewer.h"
#include "object.h"

#if QT_VERSION < 0x040000
# include "qptrvector.h"
#endif

class Viewer : public QGLViewer
{
public:
  Viewer();

protected :
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;

  // Selection functions
  virtual void drawWithNames();
  virtual void endSelection(const QPoint&);

  // Mouse events functions
  virtual void mousePressEvent(QMouseEvent *e);
  virtual void mouseMoveEvent(QMouseEvent *e);
  virtual void mouseReleaseEvent(QMouseEvent *e);

private :
  void startManipulation();
  void drawSelectionRectangle() const;
  void addIdToSelection(int id);
  void removeIdFromSelection(int id);

  // Current rectangular selection
  QRect rectangle_;

  // Different selection modes
  enum SelectionMode { NONE, ADD, REMOVE };
  SelectionMode selectionMode_;

#if QT_VERSION < 0x040000
  // Objects of the scene
  QPtrVector<Object> objects_;
  // ids of the selected objects
  QValueList<int> selection_;
#else
  QList<Object*> objects_;
  QList<int> selection_;
#endif

};
