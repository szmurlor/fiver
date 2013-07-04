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

#include "fvopenerdiffpacksim.h"
#include <QtDebug>
#include <fvboxsimulation.h>
#include <fvboxmgr.h>
#include <diffsimu.h>
#include <field.h>
#include <grid.h>
#include <boxbox.h>

FVOpenerDiffpackSim::FVOpenerDiffpackSim( )
: FVOpener(0)
{
}

FVOpenerDiffpackSim::FVOpenerDiffpackSim(QObject * parent)
 : FVOpener(parent)
{
}


FVOpenerDiffpackSim::~FVOpenerDiffpackSim()
{
}

FVObject* FVOpenerDiffpackSim::open( FVBoxMgr * bm,  QString fname, int  )
{
	FVBoxGrid *bg;
	unsigned int i;
	
	qDebug() << "Opening Diffpack simulation: " << fname;
	
	FVBoxSimulation * bs = bm->addBoxSimulation( fname, tr( "DpSimulation" ) );
	
	DiffSimu * s = new DiffSimu;
	s->loadFromFile( fname.toStdString() );
	
	vector< Grid * > grids = s->getGrids();
	vector< Field * > fields = s->getFields();
	vector< FVBoxGrid *> boxGrids;
	
	for (i = 0; i < grids.size(); i++) {
		boxGrids.push_back( bm->addBoxGrid( bs, grids[i], fname, tr("DpGrid") ) );
	}
	
	for (i = 0; i < fields.size(); i++) {
		QString sGridRefNum = fields[i]->getAttr("gridRefNum").c_str();
		bg = boxGrids[sGridRefNum.toInt()-1];
		bm->addBoxField( (FVObject*)bg, fields[i], QString("%1").arg(fields[i]->getAttr("name").c_str()), QString("DpField") );
	}
	
	bm->autoArrangeChildren( bs );

    return bs;
}



