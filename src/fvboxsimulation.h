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
#ifndef FVBOXSIMULATION_H
#define FVBOXSIMULATION_H

#include <fvobject.h>
#include <fvboxmgr.h>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVBoxSimulation : public FVObject
{
Q_OBJECT

public:
	FVBoxSimulation(FVBoxMgr * manager);
	virtual ~FVBoxSimulation();

	virtual QString classType() { return QString("FVBoxSimulation"); }

public slots:

protected:
	virtual void setupAttributes();
	virtual void setupMenu();

};

#endif
