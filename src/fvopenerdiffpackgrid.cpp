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




