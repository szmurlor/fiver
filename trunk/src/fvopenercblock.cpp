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

