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
#include <QColor>
#include <QFont>
#include "attrmodel.h"

#include <QDebug>

AttrModel::AttrModel(QObject * parent)
 : QAbstractTableModel(parent)
{
}


AttrModel::~AttrModel()
{
	clear();
}

void AttrModel::clear( )
{
	QList< Attr * >::iterator it;
	
	for (it = _items.begin(); it != _items.end(); it++) {
		delete (*it);
	}
	_items.clear();
        reset();
}

void AttrModel::update()
{
    QModelIndex topLeft;
    QModelIndex bottomRight;
    topLeft = index(0,0);
    bottomRight = index( _items.size() - 1, 1);
    emit dataChanged ( topLeft, bottomRight );
}


int AttrModel::rowCount( const QModelIndex &  ) const
{
	return _items.size();
}


int AttrModel::columnCount( const QModelIndex &  ) const
{
	return 1;
}


QVariant AttrModel::data( const QModelIndex & index, int role ) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::BackgroundRole) {
		if (_items[ index.row() ]->type() == "color") {
			QVariant v = QColor( _items[ index.row() ]->color() );
			return v;
		}		
		if ( ! _items[ index.row() ]->editable()) {
			QVariant v = QColor(230,230,230);
			return v;
		} else {
			return QVariant();
		}
	}
	
	if (role == Qt::FontRole) {
		if (_items[ index.row() ]->type() == "section") {
			QFont f;
			f.setBold(true);
			return f;
		} else {
			return QVariant();
		}
	}
	
	if (role == Qt::DisplayRole)
		return _items[ index.row() ]->value();

	if (role == Qt::EditRole)
		return _items[ index.row() ]->value();
	
	if (role == (Qt::UserRole+1)) 
		return _items[ index.row() ]->type();

	if (role == (Qt::UserRole+2))
		return _items[ index.row()]->list();

	if (role == (Qt::UserRole+3))
		return _items[ index.row()]->getDefault();
		
		return QVariant();
}


QVariant AttrModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if (role == Qt::FontRole) {
		if (_items[ section ]->type() == "section") {
			QFont f;
			f.setBold(true);
			return f;
		} else {
			return QVariant();
		}
	}
	
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			if (section == 0)
				return QString("Value");
		}
		if (orientation == Qt::Vertical) {
			return _items[ section ]->name();
		}
	}

	return QVariant();	
}


Qt::ItemFlags AttrModel::flags( const QModelIndex & index ) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	if (_items[ index.row() ]->type() == "section")
		return QAbstractItemModel::flags(index);
	
	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


bool AttrModel::setData( const QModelIndex & index, const QVariant & value, int role )
{
	if (index.isValid() && role == Qt::EditRole) {

		if (_items[ index.row() ]->editable()) {
			_items[ index.row() ]->setValue( value.toString() );
			emit dataChanged(index, index);
		}
		return true;
	}
	return false;
}


void AttrModel::addAttr( Attr * a )
{
	_items.append(a);
        QModelIndex bottomRight;
        bottomRight = index( _items.size() - 1, 1);
        emit dataChanged ( bottomRight, bottomRight );
}


Attr * AttrModel::addAttr( QString name, QString value, QString type, QString sDefault )
{
	Attr * a = new Attr();
	a->setName( name );
	a->setValue( value );
	a->setType( type );
	if (sDefault == QString::null)
		a->setDefault( value );
	else
		a->setDefault( sDefault );
	_items.append( a );
	
	if (type == "boolean") {
		QStringList lst;
		lst.append(tr("Yes"));
		lst.append(tr("No"));
		a->setList(lst);
	}
        QModelIndex bottomRight;
        bottomRight = index( _items.size() - 1, 1);
        emit dataChanged ( bottomRight, bottomRight );

	return a;
}

Attr * AttrModel::getAttr( unsigned int id )
{
    if( _items.size() > id )
        return _items[id];

    return NULL;
}

Attr * AttrModel::updateAttr( QString name, QString value, QString type, QString sDefault )
{
    Attr * a = getAttr(name);
    if( a == 0 ) {
        a = new Attr();
        _items.append( a );
    }

    a->setName( name );
    a->setValue( value );
    a->setType( type );
    if (sDefault == QString::null)
        a->setDefault( value );
    else
        a->setDefault( sDefault );

    if (type == "boolean") {
        QStringList lst;
        lst.append(tr("Yes"));
        lst.append(tr("No"));
        a->setList(lst);
    }

    QModelIndex bottomRight;
    bottomRight = index( _items.indexOf(a), 1);
    emit dataChanged ( bottomRight, bottomRight );

    return a;
}

Attr * AttrModel::getAttr( QString name )
{
    Attr * a;
    foreach( a, _items ) {
        if (a->name() == name)
            return a;
    }

    return 0;
}

Attr * AttrModel::addAttr( QString name, QColor value, QString type, QColor cDefault )
{
    Attr * a = new Attr();
    a->setName( name );
    a->setValue( value );
    a->setType( type );
    if (! cDefault.isValid())
        a->setDefault( a->value() );
    else
        a->setDefault( tr("(%1,%2,%3)").arg(cDefault.red()).arg(cDefault.green()).arg(cDefault.blue()) );
    emit
    _items.append( a );

    QModelIndex bottomRight;
    bottomRight = index( _items.size() - 1, 1);
    emit dataChanged ( bottomRight, bottomRight );

    return a;
}

Attr * AttrModel::updateAttr( QString name, QColor value, QString type, QColor cDefault )
{
    Attr * a = getAttr(name);
    if( a == 0 ) {
        a = new Attr();
        _items.append( a );
    }

    a->setName( name );
    a->setValue( value );
    a->setType( type );
    if (! cDefault.isValid())
        a->setDefault( a->value() );
    else
        a->setDefault( tr("(%1,%2,%3)").arg(cDefault.red()).arg(cDefault.green()).arg(cDefault.blue()) );

    QModelIndex bottomRight;
    bottomRight = index( _items.indexOf(a), 1);
    emit dataChanged ( bottomRight, bottomRight );

    return a;
}

Attr * AttrModel::addSection( QString name )
{
	Attr * a = addAttr(  name, QString(""), QString("section") );
	a->setEditable( false );

	return a;
}



