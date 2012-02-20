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
#ifndef ATTR_H
#define ATTR_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QColor>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class Attr : public QObject
{
public:
	Attr();
	virtual ~Attr();
	
	QString value();
	void setValue( QString str );
	void setValue( QColor v );
	
	QString name();
	void setName( QString str );

	QString type();
	void setType(QString str);

	QStringList list();
	void setList( QStringList list );
	void addListItem( QString item );
	
	bool editable();
	void setEditable( bool b );
	
	bool readOnly();
	void setReadOnly( bool b );
	
	QColor color();
	double toDouble();
	
	QString getDefault();
	void setDefault( QString str );
	
protected:
	QString _name;
	QString _value;
	QString _type;
	QString _default;
	bool _editable;
	QStringList _list;
};

#endif
