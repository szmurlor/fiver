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

#ifndef DVONNWINDOWIMPL_H
#define DVONNWINDOWIMPL_H

#include <qglobal.h>

#if QT_VERSION >= 0x040000
# include "ui_dvonnwindow.Qt4.h"
  class DvonnWindow : public QMainWindow, public Ui::DvonnWindow
  {
  public:
    DvonnWindow() { setupUi(this); }
  };
#else
# if QT_VERSION >= 0x030000
#  include "dvonnwindow.Qt3.h"
# else
#  error "No designer .ui file available for Qt 2"
# endif
#endif
#include "game.h"

class QTextBrowser;
class QLabel;
class QTimer;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of DvonnWindowImpl
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DvonnWindowImpl : public DvonnWindow
{
  Q_OBJECT
public:
  DvonnWindowImpl(dvonn::Game* g);
public slots:
  void load();
  void save();
  void saveAs();
  void print();

  void newGame();
  void undo();
  void redo();

  void help();
  void about();

  void randomlyFinishPlacement();
  void randomlyMoveStack();
  void play(dvonn::Game::Placement);
  void play(dvonn::Game::Move);
protected:
  QString labelFor(dvonn::Player);
  QString labelFor(dvonn::Phase);
  void updateStatusBar();
protected slots:
  void animateScore();
private:
  dvonn::Game*  game_;
  QTextBrowser* rulesBrowser_;
  QLabel*       currentPlayerLB_;
  QLabel*       currentPhaseLB_;
  QTimer*       startAnimScoreTimer_;

};

#endif // DVONNWINDOWIMPL_H
