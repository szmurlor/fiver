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

