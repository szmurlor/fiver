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

#include "fvopenercblock.h"
#include <QtDebug>
#include <fvboxcblock.h>
#include <QMessageBox>

FVOpenerCBlock::FVOpenerCBlock( )
: FVOpener(0)
{
}

FVOpenerCBlock::FVOpenerCBlock(QObject * parent)
 : FVOpener(parent)
{
}


FVOpenerCBlock::~FVOpenerCBlock()
{
}

FVObject* FVOpenerCBlock::open( FVBoxMgr * bm, QString fname, int type )
{
	bm = bm;
	fname = fname;
	type = type;
	
	qDebug() << "Opening CBlock: " << fname;

        FVBoxCBlock *cblock = bm->addBoxCBlock( fname, "CBlock" );
        if (cblock->loadFromFile( fname ) == false) {
            printf("At least one of Cblocks has length = 0. It is possible that the file has wrong format:\n");
            QMessageBox::warning(bm,tr("Cblock opening warning."), tr("No cblocks or at least one has length = 0. It is possible that the file has wrong format: %1.").arg(fname));
        }
	

    return cblock;
}

