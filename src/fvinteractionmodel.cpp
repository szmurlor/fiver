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
#include "fvinteractionmodel.h"
#include <fvobject.h>
#include <fivermain.h>

FVInteractionModel::FVInteractionModel(QObject * parent)
: QObject(parent)
{
}


FVInteractionModel::~FVInteractionModel()
{
}

void FVInteractionModel::mousePressEvent( QMouseEvent * event )
{
	emit signalMousePress( event );
}

void FVInteractionModel::keyPressEvent( QKeyEvent * event )
{
	emit signalKeyPress( event );
}

void FVInteractionModel::mouseMoveEvent( QMouseEvent * event )
{
	emit signalMouseMove( event );
}

void FVInteractionModel::mouseReleaseEvent( QMouseEvent * event )
{
	emit signalMouseRelease( event );
}

void FVInteractionModel::setDrawable( bool isDrawable )
{
	if (isDrawable) {
		if (fvlist == 0) {
			FViewer * fv = getCurrentViewer();
			fvlist = new FVList( fv );
			if (fv != 0) {
				fv->add( fvlist );
			}
		}
	} else {
		if (fvlist != 0) {
			delete fvlist;
		}
		fvlist = 0;
	}

}

bool FVInteractionModel::isDrawable( )
{
	return (fvlist != 0);
}

void FVInteractionModel::paintGL( )
{
}

bool FVInteractionModel::wantMouseEvent( QMouseEvent *  )
{
	return true;
}

bool FVInteractionModel::wantPressEvent( QKeyEvent *  )
{
	return true;
}


