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
#ifndef FVINTERACTIONSIGNALSLOT_H
#define FVINTERACTIONSIGNALSLOT_H

#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>

class FVObject;
class FVList;

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVInteractionModel : public QObject
{
	Q_OBJECT
	
public:
	FVInteractionModel(QObject * parent);
	virtual ~FVInteractionModel();

	virtual void paintGL();
	bool isDrawable();
	
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void keyPressEvent ( QKeyEvent * event );	
	virtual void mouseMoveEvent( QMouseEvent * e );
	virtual void mouseReleaseEvent( QMouseEvent * e );
	
	virtual bool wantMouseEvent( QMouseEvent * event );
	virtual bool wantPressEvent( QKeyEvent * event );

protected:
	FVObject * _parent;
	FVList		*fvlist;

	void setDrawable(bool isDrawable);
	
signals:
	void signalMousePress( QMouseEvent * event );
	void signalKeyPress ( QKeyEvent * event );
	void signalMouseRelease( QMouseEvent * event );
	void signalMouseMove( QMouseEvent * event );
};

#endif
