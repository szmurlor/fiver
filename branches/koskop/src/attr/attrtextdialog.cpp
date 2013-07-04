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


