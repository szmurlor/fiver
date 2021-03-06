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

#include "fvgridinterface.h"
#include <QString>
#include <QtDebug>

FVGridInterface::FVGridInterface( Grid ** gridPtr)
{
	gp = gridPtr;
	mp = 0;
        bmp = 0;
}

FVGridInterface::FVGridInterface( dolfin::Mesh * meshPtr)
{
        mp = meshPtr;
	gp = 0;
        bmp =0;
}

FVGridInterface::FVGridInterface( dolfin::BoundaryMesh * bmeshPtr)
{
        mp = 0;
        gp = 0;
        bmp = bmeshPtr;
}



FVGridInterface::~FVGridInterface()
{
}

Grid * FVGridInterface::getGrid( )
{
	return *gp;
}

dolfin::Mesh * FVGridInterface::getMesh( )
{
  return mp;
}

dolfin::BoundaryMesh * FVGridInterface::getBoundaryMesh( )
{
  return bmp;
}
