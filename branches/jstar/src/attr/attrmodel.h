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
#ifndef ATTRMODEL_H
#define ATTRMODEL_H

#include <QAbstractTableModel>

#include <attr.h>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class AttrModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	AttrModel(QObject * parent = 0);
	virtual ~AttrModel();


	// Qt's Model/View/Delegate architecture related
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;

	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	

	// R.Szmurlo specific:
	void addAttr( Attr * a );
	Attr * addAttr( QString name, QString value, QString type = QString("text"), QString sDefault = QString::null );
	Attr * addAttr( QString name, QColor value, QString type = QString("color"), QColor cDefault = QColor() );

        //by Furry
        Attr * getAttr( unsigned int id );

        // jstar
        Attr * updateAttr( QString name, QString value, QString type = QString("text"), QString sDefault = QString::null );
        Attr * updateAttr( QString name, QColor value, QString type = QString("color"), QColor cDefault = QColor() );
	
	Attr * getAttr( QString name );
	QString getAttrValue();
	
	Attr * addSection( QString name );
	
        void update();
	void clear();
	
	 
private:
	QList< Attr* > _items;

};

#endif
