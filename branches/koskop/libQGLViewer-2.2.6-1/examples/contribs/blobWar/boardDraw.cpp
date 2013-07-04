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
#include <qgl.h>
#include <QGLViewer/vec.h>

using namespace qglviewer;

void Board::draw() const
{
  for (int i=0; i<sizeX_; ++i)
    for (int j=0; j<sizeY_; ++j)
      {
	QPoint p(i,j);
	if (stateOf(p) != HOLE)
	  {
	    Board::drawBlock(i,j);
	    if (stateOf(p) != EMPTY)
	      drawPiece(p, stateOf(p) == BLUE);
	  }
      }
}

void Board::drawShadows() const
{
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  for (int i=0; i<sizeX_; ++i)
    for (int j=0; j<sizeY_; ++j)
      {
	QPoint p(i,j);
	if ((stateOf(p) != HOLE) && (stateOf(p) != EMPTY))
	    drawShadow(p);
      }
  glDisable(GL_BLEND);
}

void Board::drawBlock(int i, int j)
{
  static const float thickness = 0.6f;
  glPushMatrix();
  glTranslatef(i, j, 0.0f);

  if ((i+j)%2)
    glColor3f(0.5, 0.5, 0.5);
  else
    glColor3f(0.7, 0.7, 0.7);

  glBegin(GL_QUADS);
  // Top
  glNormal3f(0.0, 0.0, 1.0);
  glVertex2i(0, 0);
  glVertex2i(1, 0);
  glVertex2i(1, 1);
  glVertex2i(0, 1);

  // Sides
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3f(0, 0, 0.0);
  glVertex3f(0, 0, -thickness);
  glVertex3f(1, 0, -thickness);
  glVertex3f(1, 0, 0.0);
  
  glNormal3f(1.0, 0.0, 0.0);
  glVertex3f(1, 0, 0.0);
  glVertex3f(1, 0, -thickness);
  glVertex3f(1, 1, -thickness);
  glVertex3f(1, 1, 0.0);
  
  glNormal3f(0.0, 1.0, 0.0);
  glVertex3f(1, 1, 0.0);
  glVertex3f(1, 1, -thickness);
  glVertex3f(0, 1, -thickness);
  glVertex3f(0, 1, 0.0);
  
  glNormal3f(-1.0, 0.0, 0.0);
  glVertex3f(0, 1, 0.0);
  glVertex3f(0, 1, -thickness);
  glVertex3f(0, 0, -thickness);
  glVertex3f(0, 0, 0.0);
  glEnd();

  glDisable(GL_LIGHTING);
  glLineWidth(2.0);
  glColor3f(0.3, 0.3, 0.3);
  glBegin(GL_LINE_LOOP);
  glVertex3f(0.0, 0.0, 0.004);
  glVertex3f(1.0, 0.0, 0.004);
  glVertex3f(1.0, 1.0, 0.004);
  glVertex3f(0.0, 1.0, 0.004);
  glEnd();
  glEnable(GL_LIGHTING);

  glPopMatrix();
}

void Board::drawSquare(const QPoint& p)
{
  glPushMatrix();
  glTranslatef(p.x(), p.y(), 0.001);

  glBegin(GL_QUADS);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex2i(0, 0);
  glVertex2i(1, 0);
  glVertex2i(1, 1);
  glVertex2i(0, 1);
  glEnd();

  glPopMatrix();
}

static qglviewer::Vec posOf(const QPoint& p) { return qglviewer::Vec(p.x()+0.5,p.y()+0.5,0.0); };

void Board::drawPiece(const QPoint& p, bool blue)
{
  glPushMatrix();
  Vec pos = posOf(p);
  glTranslatef(pos.x, pos.y, pos.z);
  drawPiece(blue);
  glPopMatrix();
}

void Board::drawFlippingPieces(const QPoint& p, bool flip) const
{
  for (int i=-1; i<=1; ++i)
    for (int j=-1; j<=1; ++j)
      {
	const QPoint q(p.x()+i, p.y()+j);
	if (isValid(q) && stateOf(q) == Board::blueColor(!bluePlays()))
	  drawPiece(q, flip ^ bluePlays());
      }
}

void Board::drawPiece(bool blue)
{
  static GLUquadric* quadric = gluNewQuadric();

  if (blue)
    glColor3f(0.4, 0.4, 1.0);
  else
    glColor3f(1.0, 0.4, 0.4);
  gluCylinder(quadric, 0.3, 0.2, 0.6, 12, 1);
  glTranslatef(0.0f, 0.0f, 0.6f);
  gluDisk(quadric, 0.0, 0.2, 12, 1);
}

void Board::drawShadow(const QPoint& p)
{
  static GLUquadric* quadric = gluNewQuadric();

  glPushMatrix();
  glColor4f(0.4, 0.4, 0.4, 0.5);
  const Vec pos = posOf(p);
  const Vec dir = pos.unit();
  glTranslatef(pos.x+0.1f*dir.x, pos.y+0.1f*dir.y, 0.004);
  gluDisk(quadric, 0.0, 0.37, 12, 1);
  glPopMatrix();
}

void Board::drawSelectedPiece(int piece) const
{
  const QPoint sp = pointFromInt(piece);
  
  glEnable(GL_BLEND);
  glColor4f(0.3, 0.9, 0.3, 0.8);
  
  Board::drawSquare(sp);

  glDisable(GL_BLEND);
}

void Board::drawPossibleDestinations(int piece, bool select) const
{
  glEnable(GL_BLEND);
  
  const QPoint sp = pointFromInt(piece);
  bool blue = stateOf(sp) == Board::BLUE;
  for (int i=-2; i<=2; ++i)
    for (int j=-2; j<=2; ++j)
      {
	const QPoint p(sp.x()+i,sp.y()+j);
	if (isValid(p) && stateOf(p) == Board::EMPTY)
	  {
	    if (select)
	      glPushName((intFromPoint(p)));
	    else
	      {
		bool close = ((abs(i) < 2) && (abs(j) < 2));
		if (blue)
		  if (close)
		    glColor3f(0.3, 0.3, 1.0);
		  else
		    glColor3f(0.5, 0.5, 1.0);
		else
		  if (close)
		    glColor3f(1.0, 0.3, 0.3);
		  else
		    glColor3f(1.0, 0.5, 0.5);
	      }
	    Board::drawSquare(p);
	    if (select)
	      glPopName();
	  }
      }
  glDisable(GL_BLEND);
}

void Board::drawSelectablePieces() const
{
  for (int i=0; i<sizeX_; ++i)
    for (int j=0; j<sizeY_; ++j)
      {
	QPoint p(i,j);
	if ((stateOf(p) == Board::blueColor(bluePlays())) && (pieceCanMove(p)))
	  {
	    glPushName(intFromPoint(p));
	    drawPiece(p, true);
	    drawSquare(p);
	    glPopName();
	  }
      }
}
