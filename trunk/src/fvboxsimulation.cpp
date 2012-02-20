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


