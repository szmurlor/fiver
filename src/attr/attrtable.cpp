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

