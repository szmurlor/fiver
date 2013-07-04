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

#include "board.h"
#include <qmessagebox.h>

bool Board::canBeSelected(int i)
{
  QPoint p = pointFromInt(i);
  return (isValid(p) && (stateOf(p) == Board::blueColor(bluePlays())) && (pieceCanMove(p)));
}

bool Board::isValid(const QPoint& p) const
{
  return ((p.x() >= 0) && (p.y() >= 0) && (p.x() < sizeX_) && (p.y() < sizeY_) && (stateOf(p) != HOLE));
}

void Board::play(const Move& m)
{
  if (!m.isValid(this))
    {
      QMessageBox::critical(NULL, "Invalid move", "Invalid move");
      return;
    }

  if (!m.isClose())
    setStateOf(m.start(), Board::EMPTY);

  setStateOf(m.end(), Board::blueColor(bluePlays()));
  
  for (int i=-1; i<=1; ++i)
    for (int j=-1; j<=1; ++j)
      {
	const QPoint p(m.end().x()+i, m.end().y()+j);
	if (isValid(p) && stateOf(p) == Board::blueColor(!bluePlays()))
	  setStateOf(p, Board::blueColor(bluePlays()));
      }

  undo_.addState(stateString());
  bluePlays_ = !bluePlays_;
  checkPlayerCanPlay();
}

void Board::checkPlayerCanPlay()
{
  for (int i=0; i<sizeX_; ++i)
    for (int j=0; j<sizeY_; ++j)
      {
	const QPoint p(i,j);
	if (stateOf(p) == Board::blueColor(bluePlays()) && (pieceCanMove(p)))
	  return;
      }

  // Fills empty squares with opposite color
  for (int i=0; i<sizeX_; ++i)
    for (int j=0; j<sizeY_; ++j)
      {
	const QPoint p(i,j);
	if (isValid(p) && stateOf(p) == Board::EMPTY)
	  setStateOf(p, Board::blueColor(!bluePlays()));
      }
}

bool Board::gameIsOver() const
{
  for (int i=0; i<sizeX_; ++i)
    for (int j=0; j<sizeY_; ++j)
      if (stateOf(QPoint(i,j)) == EMPTY)
	return false;
  return true;
}

#if QT_VERSION < 0x040000
QValueVector<Move> Board::possibleMoves(bool bluePlays) const
{
  QValueVector<Move> res;
#else
QVector<Move> Board::possibleMoves(bool bluePlays) const
{
  QVector<Move> res;
#endif
  for (int i=0; i<sizeX_; ++i)
    for (int j=0; j<sizeY_; ++j)
      {
	const QPoint s(i,j);
	if (stateOf(s) == Board::blueColor(bluePlays))
	  {
	    for (int ii=-2; ii<=2; ++ii)
	      for (int jj=-2; jj<=2; ++jj)
		{
		  const QPoint e(s.x()+ii,s.y()+jj);
		  if (isValid(e) && stateOf(e) == Board::EMPTY)
		    res.append(Move(s, e));
		}
	  }
      }
  return res;
}

bool Board::pieceCanMove(const QPoint& p) const
{
  for (int i=-2; i<=2; ++i)
    for (int j=-2; j<=2; ++j)
      {
	const QPoint q(p.x()+i,p.y()+j);
	if (isValid(q) && stateOf(q) == Board::EMPTY)
	  return true;
      }
  return false;
}

void Board::setStateOf(const QPoint& p, State s)
{
  if (stateOf(p) == RED) nbRed_--;
  if (stateOf(p) == BLUE) nbBlue_--;
  
  board_[p.x()][p.y()] = s;
  
  if (stateOf(p) == RED) nbRed_++;
  if (stateOf(p) == BLUE) nbBlue_++;
}

QString Board::stateString() const
{
  QString res;
  
  for (int i=0; i<sizeX_; ++i)
    for (int j=0; j<sizeY_; ++j)
      switch(board_[i][j])
	{
	case Board::EMPTY: res.append('.'); break;
	case Board::RED:   res.append('R'); break;
	case Board::BLUE:  res.append('B'); break;
	case Board::HOLE:  res.append('X'); break;
	}
  return res;
}

std::ostream& operator<<(std::ostream& out, const Board& b)
{
  out << b.sizeX_ << " " << b.sizeY_ << std::endl;

  QString s = b.stateString();

  for (int i=0; i<b.sizeX_; ++i)
#if QT_VERSION < 0x040000
    out << s.mid(i*b.sizeY_, b.sizeY_).latin1() << std::endl;
#else
    out << s.mid(i*b.sizeY_, b.sizeY_).toLatin1().constData() << std::endl;
#endif

  out << b.undo_;
  
  return out;
}

void Board::initFromStateString(const QString& s)
{
  nbBlue_ = nbRed_ = 0;
  
  for (int i=0; i<sizeX_; ++i)
    for (int j=0; j<sizeY_; ++j)
#if QT_VERSION < 0x040000
      switch (s[i*sizeY_+j].latin1())
#else
      switch (s[i*sizeY_+j].toLatin1())
#endif
	{
	case '.' : board_[i][j] = Board::EMPTY; break;
	case 'R' : board_[i][j] = Board::RED;   nbRed_++; break;
	case 'B' : board_[i][j] = Board::BLUE;  nbBlue_++; break;
	case 'X' : board_[i][j] = Board::HOLE;  break;
	}
}

std::istream& operator>>(std::istream& in, Board& b)
{
  int sx, sy;
  in >> sx >> sy;
  b.resize(sx, sy);

  char c;
  QString s;
  for (int i=0; i<sx; ++i)
    for (int j=0; j<sy; ++j)
      {
	in >> c;
	s.append(c);
      }

  b.initFromStateString(s);
  
  in >> b.undo_;
    
  if (b.undo_.isEmpty())
    b.undo_.addState(s);

  b.bluePlays_ = (b.undo_.nbMoves() % 2 == 1);

  return in;
}

void Board::resize(int sizeX, int sizeY)
{
  if (board_)
    {
      for (int i=0; i<sizeX_; ++i)
	delete[] board_[i];
      delete[] board_;
    }

  sizeX_ = sizeX;
  sizeY_ = sizeY;
  
  board_ = new int*[sizeX_];
  for (int i=0; i<sizeX_; ++i)
    board_[i] = new int[sizeY_];
}

bool Board::undo()
{
  QString state = undo_.undoState();
  if (state.isEmpty())
    return false;
  else
    initFromStateString(state);
  bluePlays_ = !bluePlays_;
  return true;
}

bool Board::redo()
{
  QString state = undo_.redoState();
  if (state.isEmpty())
    return false;
  else
    initFromStateString(state);
  bluePlays_ = !bluePlays_;
  return true;
}

Move Board::bestMoveNumberOfNewPieces() const
{
  Move res;
  int bestValue = 0;
#if QT_VERSION < 0x040000
  QValueVector<Move> moves = possibleMoves(bluePlays());
#else
  QVector<Move> moves = possibleMoves(bluePlays());
#endif
  for (int i=0; i<int(moves.size()); ++i)
    {
      int value = moves[i].numberOfNewPieces(*this);
      if (value > bestValue)
	{
	  bestValue = value;
	  res = moves[i];
	}
    }
  return res;
}

QString Board::statusMessage() const
{
  const QString score = "Blue : " + QString::number(nbBlue_) + " - Red : " + QString::number(nbRed_);
  
  if (gameIsOver())
      if (nbBlue_ == nbRed_)
	return "Deuce !\n" + score;
      else
	return QString("The ") + ((nbBlue_>nbRed_)?"Blue":"Red") + " won !\n" + score;
  else
    return score + " - " + (bluePlays_?"Blue":"Red") + " plays";
}
