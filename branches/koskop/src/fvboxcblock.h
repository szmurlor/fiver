/***************************************************************************
 *   Copyright (C) 2008 by Robert Szmurlo   *
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
#ifndef FVBOXCBLOCK_H
#define FVBOXCBLOCK_H

#include <fvboxgrid.h>
#include <fvboxmgr.h>
#include <grid.h>
#include <simplefield.h>
#include <field.h>
#include "fvfieldinterface.h"

#include <QString>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVBoxCBlock : public FVBoxGrid
{
    Q_OBJECT 

protected:
	QString currentFName;
	bool bSelfUpdate;
public:
	FVBoxCBlock(FVBoxMgr * manager, int x = 0, int y = 0);
	~FVBoxCBlock();

	virtual QString classType() { return QString("FVBoxCBlock"); }

    bool loadFromFile( QString fname );

	virtual FVInterface * getInterface( QString interfaceName );

public slots:
        void slotDrawVectors();
        void slotAddSlice();
        void slotReload();


protected:
	Field * field;
	FVFieldInterface * fvFieldInterface;

	virtual void setupAttributes();
	virtual void update();
	virtual void setupMenu();

     void newField();
};

#endif
