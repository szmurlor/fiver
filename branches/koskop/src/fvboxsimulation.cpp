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

#include "fvboxsimulation.h"
#include <main.h>

FVBoxSimulation::FVBoxSimulation(FVBoxMgr * manager)
 : FVObject(manager,false)
{
    rRect.setRect(10,10,150,20);
	cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_SIMULATION ).value<QColor>();
	
	setupAttributes();
	setupMenu();

    rRect.setWidth( 120 );
}


FVBoxSimulation::~FVBoxSimulation()
{
}

void FVBoxSimulation::setupAttributes( )
{
	Attr * a;
	a = am->addAttr( tr("Type"), tr("Field"), QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("Filename"), tr(""), QString("text") );
	a->setEditable(false);

}

void FVBoxSimulation::setupMenu( )
{
}


