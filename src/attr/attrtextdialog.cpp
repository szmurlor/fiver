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
#include "attrtextdialog.h"
#include <QTextEdit>
#include <QLayout>

AttrTextDialog::AttrTextDialog(QWidget * parent)
 : QDialog(parent)
{
	QGridLayout * layout = new QGridLayout();
	txt = new QTextEdit(this);
	layout->addWidget( txt, 0, 0, 1, 2 );

	cmdOK = new QPushButton(this);
	cmdOK->setText( tr("OK") );
	layout->addWidget(cmdOK,1,0);
	connect( cmdOK, SIGNAL(clicked()), this, SLOT(slotOK()));

	cmdCancel = new QPushButton(this);
	cmdCancel->setText( tr("Cancel") );
	layout->addWidget(cmdCancel,1,1);
	connect( cmdCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));

	result = false;
	setLayout(layout);
}


AttrTextDialog::~AttrTextDialog()
{
}

void AttrTextDialog::slotCancel( )
{
	result = false;
	close();
}

void AttrTextDialog::slotOK( )
{
	result = true;
	close();
}

void AttrTextDialog::setText( QString s )
{
	txt->setPlainText( s );
}

QString AttrTextDialog::text( )
{
	return txt->toPlainText();
}


