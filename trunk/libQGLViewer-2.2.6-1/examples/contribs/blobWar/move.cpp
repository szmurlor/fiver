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

#include "move.h"
#include "board.h"
#include "qstringlist.h"
#include "qregexp.h"

#include "QGLViewer/keyFrameInterpolator.h"
using namespace qglviewer;

Move::Move(const QPoint& s, const QPoint& e)
  : start_(s), end_(e)
{}

Move::Move(const Board* const b, int s, int e)
{
  start_ = b->pointFromInt(s);
  end_   = b->pointFromInt(e);
}

Move::Move(const QString text)
{
#if QT_VERSION < 0x040000
  QStringList list = QStringList::split(QRegExp("\\D"), text);
#else
  QStringList list = text.split(QRegExp("\\D"), QString::SkipEmptyParts);
#endif

  start_ = QPoint(list[0].toInt(), list[1].toInt());
  end_ = QPoint(list[2].toInt(), list[3].toInt());
}

bool Move::isValid(const Board* const b) const
{
  return (b->isValid(start()) &&
	  b->isValid(end()) &&
	  abs(start().x()-end().x()) <= 2 &&
	  abs(start().y()-end().y()) <= 2 &&
	  start() != end() &&
	  b->stateOf(start())==Board::blueColor(b->bluePlays()) &&
	  b->stateOf(end())==Board::EMPTY);
}

bool Move::isClose() const
{
  QPoint delta = start() - end();
  return (abs(delta.x()) < 2) && (abs(delta.y())<2);
}

int Move::numberOfNewPieces(const Board& b) const
{
  int res = 0;

  for (int i=-1; i<=1; ++i)
    for (int j=-1; j<=1; ++j)
      {
	const QPoint p(end().x()+i, end().y()+j);
	if (b.isValid(p) && b.stateOf(p) == Board::blueColor(!b.bluePlays()))
	  res++;
      }

  if (isClose())
    res++;

  return res;
}

std::ostream& operator<<(std::ostream& out, const Move& m)
{
  out << "(" << m.start().x() << "," << m.start().y() << ") -> (" << m.end().x() << ","<< m.end().y() << ")" << std::endl;
  return out;
}

