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

#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "qobject.h"
#include "qstring.h"

class ComputerPlayerInterface;
class QProcess;

class ComputerPlayer : public QObject
{
  Q_OBJECT
  
public:
  ComputerPlayer();
  ~ComputerPlayer();
  
  bool isActive() const { return isActive_; }
  void setIsActive(bool on);
  
  int allowedTime() const;
  void setAllowedTime(int time);

  QString programFileName() const;
  void setProgramFileName(const QString& name);

  void configure();

  void play(bool blue, const QString& stateFileName);

public:
  signals:
  void moveMade(QString move, int duration);
 
private slots:
  void selectProgram();
  void readFromStdout();
  
private:
  bool isActive_;
  ComputerPlayerInterface* interface_;
  QProcess* process_;
};

#endif // COMPUTER_PLAYER_H
