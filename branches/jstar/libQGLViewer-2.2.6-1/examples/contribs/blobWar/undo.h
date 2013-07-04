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

#ifndef UNDO_H
#define UNDO_H

// To get QT_VERSION
#include "qglobal.h"
#include <iostream>

#if QT_VERSION < 0x040000
# include <qvaluevector.h>
#else
# include <qstringlist.h> 
#endif

class Undo
{
public:
  Undo() { clear(); }

  void clear();
  void addState(const QString& s);
  
  QString undoState();
  QString redoState();

  bool isEmpty() const { return state_.isEmpty(); };

  int nbMoves() const { return index_; };
  
  friend std::ostream& operator<<(std::ostream& out, const Undo& u);
  friend std::istream& operator>>(std::istream& in, Undo& u);

private:
#if QT_VERSION < 0x040000
  QValueVector<QString> state_;
#else
  QStringList state_;
#endif
  int index_, maxIndex_;
};

#endif // UNDO_H
