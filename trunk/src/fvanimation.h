/***************************************************************************
 *   Copyright (C) 2006 by Robert Szmurlo   *
 *   robert@iem.pw.edu.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef FVANIMATION_H
#define FVANIMATION_H

#include <fvobject.h>
#include <fvboxmgr.h>
#include <fvrequirefield.h>
#include <QTimer>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVAnimation : public FVObject
{
	Q_OBJECT
public:
	FVAnimation(FVBoxMgr * manager, int x = 0, int y = 0);
	~FVAnimation();
	virtual bool removeChildren() { return false; }
	
	virtual QString classType() { return QString("FVAnimation"); };
	virtual void update();
	virtual void initAttributes();
	virtual void updateAttributes();

public slots:
	void slotUpdate();
	void slotTimer();

    void slotPlay();
    void slotPause();
    void slotStop();

protected:
	FVRequireField reqField;
	QTimer * tmr;

	virtual void setupAttributes();
	virtual void setupMenu();

private:
    QString lastPlaySpeed;
    QAction * acPlay;
    QAction * acPause;
    QAction * acStop;
};

#endif
