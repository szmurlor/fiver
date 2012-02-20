
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
#include "fvrequirefield.h"
#include <QtDebug>

FVRequireField::FVRequireField()
{
}


FVRequireField::~FVRequireField()
{
}

Field * FVRequireField::getField( FVObject * fvo )
{
	FVFieldInterface * fvi = 0;
	if (fvo != 0) 
		fvi = (FVFieldInterface*) fvo->getInterface( QString("FVFieldInterface") );

	if (fvi != 0)
		return fvi->getField();
	else {
		qDebug() << "FVRequireField: Unable to get field from " << fvo->classType() << "!";
		return 0;
	}
}


