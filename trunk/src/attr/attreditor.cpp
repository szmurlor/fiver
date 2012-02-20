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



