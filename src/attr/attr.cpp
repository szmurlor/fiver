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
#include "attr.h"
#include <QMessageBox>

Attr::Attr()
 : QObject()
{
	_editable = true;
	_default = QString::null;
}

Attr::~Attr()
{
}

QString Attr::value( )
{
	return _value;
}

void Attr::setValue( QString str )
{
	_value = str;
}

QString Attr::name( )
{
	return _name;
}

void Attr::setName( QString str )
{
	_name = str;
}

QString Attr::type( )
{
	return _type;
}

void Attr::setType( QString str )
{
	_type = str;
}

QStringList Attr::list( )
{
	return _list;
}

void Attr::setList( QStringList list )
{
	_list = list;
}

void Attr::addListItem( QString item )
{
	_list.append( item );
}

bool Attr::editable( )
{
	return _editable;
}

void Attr::setEditable( bool b )
{
	_editable = b;
}

bool Attr::readOnly( )
{
	return !_editable;
}

void Attr::setReadOnly( bool b )
{
	_editable = !b;
}

QColor Attr::color( )
{
	bool ok;
	QString r,g,b;
	int ir, ig, ib;

	QRegExp sep("[,\\(\\)\\s]+");
	r = value().section( sep, 1,1);
	g = value().section( sep, 2,2);
	b = value().section( sep, 3,3);

	ir = r.toInt(&ok);
	if (!ok) {
		//QMessageBox::critical(0, tr("Attr Color Dialog"), tr("Incorrect red color component: %1").arg(r) );
		return QColor(200,200,200);
	}

	ig = g.toInt(&ok);
	if (!ok) {
		//QMessageBox::critical(0, tr("Attr Color Dialog"), tr("Incorrect green color component: %1").arg(g) );
		return QColor(200,200,200);
	}

	ib = b.toInt(&ok);
	if (!ok) {
		//QMessageBox::critical(0, tr("Attr Color Dialog"), tr("Incorrect blue color component: %1").arg(b) );
		return QColor(200,200,200);
	}

	return QColor(ir,ig,ib);
}

double Attr::toDouble( )
{
	return _value.toDouble();
}

void Attr::setValue( QColor v )
{
	_value = QString("(%1,%2,%3)").arg(v.red()).arg(v.green()).arg(v.blue());
}

QString Attr::getDefault( )
{
	return _default;
}

void Attr::setDefault( QString str )
{
	_default = str;
}

