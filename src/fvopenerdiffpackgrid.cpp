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
#include "fvopenerdiffpackgrid.h"
#include <QtDebug>
#include <QMessageBox>
#include <fvboxgrid.h>
#include <grid.h>
#include <diffreader.h>



FVOpenerDiffpackGrid::FVOpenerDiffpackGrid( )
: FVOpener(0)
{
}

FVOpenerDiffpackGrid::FVOpenerDiffpackGrid(QObject * parent)
 : FVOpener(parent)
{
}

FVOpenerDiffpackGrid::~FVOpenerDiffpackGrid()
{
}

FVObject* FVOpenerDiffpackGrid::open( FVBoxMgr * bm, QString fname, int type )
{
	QList<int> positions;
	int pos;
	type = type;
	DpGridFileInfo dgfi;
	DpReader dgr;
    FVObject *firstBox = 0;

        //qDebug() << "Reading info from file: "  << fname;
	dgfi.ReadFromFile( fname.toStdString() );
	
	if ( dgfi.count() == 0 ) {
		QMessageBox::critical(0,tr("Wrong File Dialog"), tr("Could not find any Diffpack grid in a file: ") + fname);
        return firstBox;
	}
	
	if (dgfi.count() > 0 ) {
		stGridInfo *gi;
		gi = dgfi.first();
		do  {
                        positions.append(gi->position-1);
		} while ((gi = gi->next) != 0);
	}
	
	foreach(pos, positions){
		
		Grid * g = new Grid();
		
		if (g == 0) {
			qFatal("Unable to allocate memory for new Grid()");
            return firstBox;
		}
		
		dgr.ReadFromFile( g, fname.toStdString(), pos );
		
        FVObject* box = bm->addBoxGrid( 0, g, fname, tr("Diffpack Grid"));
        if ( firstBox == 0)
            firstBox = box;
	}
	
    return firstBox;
}




