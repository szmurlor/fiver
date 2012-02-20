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

#ifndef QUARTO_H
#define QUARTO_H

#include <qvariant.h>
#include <qmainwindow.h>
#include "glview.h"
#include "piece.h"
#include <iostream>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPopupMenu;
class QFrame;
class QGroupBox;
class QLabel;
class QPushButton;
class QWidget;

class Quarto : public QMainWindow
{
  Q_OBJECT

public:
  Quarto( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
  ~Quarto();

  QFrame* GLFrameJeu, *GLFramePieces;
  // buttons
  QGroupBox* GameGroupBox;
  QLabel* TourDeJeuLabel, *NomLabel;
  QPushButton* ResetButton, *QuitButton;
  QPopupMenu *GagnantPopUp;

public slots:
  virtual void New();
  virtual void Exit();
  virtual void changeTour();
  virtual void piecePlacee();
  virtual void finDeJeu();

signals:
  void updategl();

protected:
  QVBoxLayout* QuartoLayout, *VLayout1, *VLayout2, *VLayout3;
  QHBoxLayout* MainHLayout, *HLayout1, *HLayout2, *HLayout3, *HLayout4;

  bool joueur;
  bool pieceplacee;
  int width, height;
  GLViewJeu *vuePlateau;
  GLViewPieces *vuePieces;
  SetOfPiece *setofpiece;
  virtual void init(bool);
};

#endif // QUARTO_H
