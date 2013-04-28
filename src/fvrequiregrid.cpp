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

#include "fvrequiregrid.h"
#include <QtDebug>

FVRequireGrid::FVRequireGrid()
{
}


FVRequireGrid::~FVRequireGrid()
{
}

Grid * FVRequireGrid::getGrid( FVObject * fvo, FVObject * alternateObject )
{
	FVGridInterface * fvi = 0;
	if (fvo != 0) 
		fvi = (FVGridInterface*) fvo->getInterface( QString("FVGridInterface") );
	else {
		if (alternateObject != 0)
			fvi = (FVGridInterface*) alternateObject->getInterface( QString("FVGridInterface") );
	}

	if (fvi != 0)
		return fvi->getGrid();
	else {
		qDebug() << "FVRequireGrid: Unable to get grid from " << fvo->classType() << "!";
		return 0;
	}
}

dolfin::Mesh * FVRequireGrid::getMesh( FVObject * fvo, FVObject * alternateObject )
{
        FVGridInterface * fvi = 0;
        if (fvo != 0)
                fvi = (FVGridInterface*) fvo->getInterface( QString("FVGridInterface") );
        else {
                if (alternateObject != 0)
                        fvi = (FVGridInterface*) alternateObject->getInterface( QString("FVGridInterface") );
        }

        if (fvi != 0)
                return fvi->getMesh();
        else {
                qDebug() << "FVRequireGrid: Unable to get grid from " << fvo->classType() << "!";
                return 0;
        }
}
