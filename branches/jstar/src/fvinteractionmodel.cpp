/*****************************************************************************
 *   Copyright (C) 2006 Robert Szmurlo <robert@iem.pw.edu.pl>                *
 *                                                                           *
 *   Licensed under the Apache License, Version 2.0 (the "License");         *
 *   you may not use this file except in compliance with the License.        *
 *   You may obtain a copy of the License at                                 *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *   Unless required by applicable law or agreed to in writing, software     *
 *   distributed under the License is distributed on an "AS IS" BASIS,       *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *   See the License for the specific language governing permissions and     *
 *   limitations under the License.                                          *
 *****************************************************************************/

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


