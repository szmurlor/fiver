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

#ifndef MOVE_H
#define MOVE_H

#include <qpoint.h>
#include <iostream>

class Board;

class Move
{  
public:
  Move() {};
  Move(const QPoint& s, const QPoint& e);
  Move(const Board* const b, int s, int e);
  Move(const QString text);

  bool isValid(const Board* const b) const;  
  bool isClose() const;

  int numberOfNewPieces(const Board& b) const;

  friend std::ostream& operator<<(std::ostream& out, const Move& m);

  const QPoint& start() const { return start_; }
  const QPoint& end() const { return end_; }

private:
  QPoint start_, end_;
};

#endif // MOVE_H
