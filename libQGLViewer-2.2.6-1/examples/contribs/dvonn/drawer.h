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

#ifndef DVONNDRAWER_H
#define DVONNDRAWER_H

#include "game.h"

#include <QGLViewer/vec.h>
#include <qstring.h>
#include <qgl.h>
#include <map>

class QGLViewer;

namespace dvonn
{
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Interface of Drawer
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  class Drawer
  {
  public:
    Drawer();
    virtual ~Drawer();
    void init();
    void toggleTexture(bool);
    void drawPieces(const Board::ConstStackHandle&) const;
    void draw(const Board::ConstStackHandle&) const;
    void drawStatus(const Board::ConstStackHandle&,QGLViewer* v) const;
    void drawComplement(bool showLabels) const;
    void drawWhitePiecePools(const Board&,bool lastTransparent) const;
    void drawBlackPiecePools(const Board&,bool lastTransparent) const;

    void drawTransparentPiece(Color p,const Board::ConstStackHandle& c,
			      float a=0.5f) const;
    // Here we use a trick: we want to define the function for
    // Stack::const_iterator only but we cannot forward declare it and we
    // do not want to include dvonnBoard.h. So we declare a template and in
    // the .cpp, we force the instantiation for Case::piece_const_iterator.
    // Rq: no invocation with other type O will link!
    template <class O>
    void drawTransparentPieces(O first,O last,const Board::Coord& c,
			       float h=0.0f,float a=0.5f) const;

    void highlight(const Board::ConstStackHandle&) const;
    void highlightPieces(const Board::ConstStackHandle&) const;

    void drawMove(const Board& b,const Game::Move m,float t) const;
    float estimateDrawMoveLength(const Board& b,const Game::Move m) const;

    qglviewer::Vec boardCenter() const;
    qglviewer::Vec boardUpVector() const;
    float boardRadius() const;
    qglviewer::Vec defaultEyePosition() const;
  private:
    void startTexture(const QString& t) const;
    void startTexture(const QString& t,GLuint) const;
    void startTexture() const;
    void endTexture() const;

    std::map<QString,GLuint> textures_;
    std::vector<GLuint>      hLabels_;
    std::vector<GLuint>      vLabels_;
    bool                     showTextures_;
  };
}
#endif // DVONNDRAWER_H
