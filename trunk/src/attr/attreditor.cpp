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

#include "attreditor.h"
#include <attrtextdialog.h>

AttrEditor::AttrEditor(QWidget * parent)
 : QFrame(parent)
{
	sDefault = QString::null;
	
	setupUI();
}

void AttrEditor::setupUI( )
{
	setFrameStyle(QFrame::NoFrame);

	// Initialize the layout
	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	setLayout( layout );

	// Create the LineEdit
	txt = new QLineEdit(this);
	QSizePolicy sp;
	sp  = txt->sizePolicy();
	sp.setHorizontalPolicy( QSizePolicy::Ignored );
	sp.setVerticalPolicy( QSizePolicy::Ignored );
	txt->setSizePolicy( sp );
	layout->addWidget( txt );

	// Create the button
	cmdDefault = new QToolButton(this);
	sp.setHorizontalPolicy( QSizePolicy::Maximum );
	cmdDefault->setSizePolicy( sp );
	cmdDefault->setText( tr("<") );
	QObject::connect(cmdDefault, SIGNAL(clicked()), this, SLOT(slotDefault()));
	layout->addWidget( cmdDefault );
	cmdDefault->setVisible( false );
	
	// Create the button
	cmd = new QToolButton(this);
	sp.setHorizontalPolicy( QSizePolicy::Maximum );
	cmd->setSizePolicy( sp );
	cmd->setText( tr("...") );
	QObject::connect(cmd, SIGNAL(clicked()), this, SLOT(slotCmdClicked()));
	layout->addWidget( cmd );
}


AttrEditor::~AttrEditor()
{
}

void AttrEditor::setDefault( QString s )
{
	if (s != QString::null) {
		cmdDefault->setVisible( true );
	} else {
		cmdDefault->setVisible( false );
	}
	sDefault = s;
}

void AttrEditor::setText( QString s )
{
	txt->setText( s );
}

QString AttrEditor::text( )
{
	return txt->text();
}

void AttrEditor::slotCmdClicked( )
{
	//qDebug("Trying to open dialog...");
	AttrTextDialog * atd = new AttrTextDialog( this );
	atd->setText( txt->text() );
	atd->setModal(true);
	atd->exec();
	if (atd->result == true)
		txt->setText( atd->text() );
	delete atd;
}

void AttrEditor::enterEvent( QEvent *  )
{
	txt->setFocus();
}

void AttrEditor::slotDefault( )
{
	txt->setText( sDefault );
}

void AttrEditor::focusOutEvent( QFocusEvent *  )
{
	parentWidget()->setFocus();
}

void AttrEditor::focusInEvent( QFocusEvent *  )
{
	txt->setFocus();
}



