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
#ifndef ATTREDITOR_H
#define ATTREDITOR_H

#include <QFrame>
#include <QLineEdit>
#include <QToolButton>
#include <QLayout>
#include <QFocusEvent>

/**
@author Robert Szmurlo
*/
class AttrEditor : public QFrame
{
	Q_OBJECT 

public:
	AttrEditor(QWidget * parent);
	virtual ~AttrEditor();

	virtual void setText( QString s );
	virtual QString text();
	
	virtual void setDefault( QString s );

public slots:
	virtual void slotCmdClicked();
	virtual void slotDefault();

protected:
	QString sDefault;
	QLineEdit * txt;
	QToolButton * cmd;
	QToolButton * cmdDefault;

	virtual void enterEvent( QEvent * event );
	virtual void focusOutEvent( QFocusEvent * event );
	virtual void focusInEvent( QFocusEvent * event );
	
	virtual void setupUI();
};

#endif
