
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


