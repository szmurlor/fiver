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
#include "attrtable.h"
#include "attrtabledelegate.h"
#include <QHeaderView>
#include <QtDebug>
#include <QKeyEvent>

AttrTable::AttrTable(QWidget * _parent)
 : QTableView(_parent)
{
	AttrTableDelegate *ad = new AttrTableDelegate( this );
	setItemDelegate(ad);

        connect(ad, SIGNAL(closeEditor(QWidget*)), this, SLOT(customCloseEditor(QWidget*)) );
	
	verticalHeader()->setResizeMode( QHeaderView::Fixed );
	verticalHeader()->setDefaultSectionSize( fontInfo().pixelSize() + 10 );
	
        horizontalHeader()->setStretchLastSection( false );

	setEditTriggers( CurrentChanged | SelectedClicked);
}


AttrTable::~AttrTable()
{
}

void AttrTable::keyPressEvent( QKeyEvent * event )
{
	if (event->key() == Qt::Key_Return) {
            //emit signalApply();
	} else {
		QTableView::keyPressEvent(event);
	}
}

void AttrTable::resizeEvent( QResizeEvent *  )
{
}


void AttrTable::customCloseEditor(QWidget* editor)
{
    emit signalApply();
}

