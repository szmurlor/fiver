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
#include "attrtabledelegate.h"
#include <QComboBox>
#include <QVariant>
#include <QModelIndex>
#include <attreditor.h>
#include <attreditorcolor.h>

AttrTableDelegate::AttrTableDelegate(QObject *parent)
 : QItemDelegate(parent)
{
}


AttrTableDelegate::~AttrTableDelegate()
{
}

QWidget * AttrTableDelegate::createEditor( QWidget * parent, const QStyleOptionViewItem & , const QModelIndex & index ) const
{
	QVariant t = index.model()->data( index, Qt::UserRole + 1 );
	if (t.toString() == "text") {
		AttrEditor * txt = new AttrEditor( parent );
		return txt;
	}

	if (t.toString() == "textlist") {
		QComboBox * cbo = new QComboBox( parent );
		return cbo;
	}
	if (t.toString() == "boolean") {
		QComboBox * cbo = new QComboBox( parent );
		return cbo;
	}
	if (t.toString() == "color") {
		AttrEditorColor * col = new AttrEditorColor( parent );
		return col;
	}

	return 0;
}

void AttrTableDelegate::setEditorData( QWidget * editor, const QModelIndex & index ) const
{
	QVariant v = index.model()->data( index, Qt::DisplayRole );
	QVariant t = index.model()->data( index, Qt::UserRole + 1 );
	QVariant def = index.model()->data( index, Qt::UserRole + 3 );

	if (t.toString() == "text") {
		AttrEditor * txt = static_cast<AttrEditor*>(editor);
		txt->setDefault( def.toString() );
		txt->setText( v.toString() );
	}
	if (t.toString() == "textlist") {
		QVariant lst = index.model()->data( index, Qt::UserRole + 2 );
			
		QComboBox *cbo = static_cast<QComboBox*>(editor);
		cbo->addItems( lst.toStringList() );
		int pos = cbo->findText( v.toString() );
		if (pos >= 0)
			cbo->setCurrentIndex( pos );
	}
	if (t.toString() == "boolean") {
		QVariant lst = index.model()->data( index, Qt::UserRole + 2 );
			
		QComboBox *cbo = static_cast<QComboBox*>(editor);
		cbo->addItems( lst.toStringList() );
		int pos = cbo->findText( v.toString() );
		if (pos >= 0)
			cbo->setCurrentIndex( pos );
	}
	if (t.toString() == "color") {
		AttrEditorColor * col = static_cast<AttrEditorColor*>(editor);
		col->setText( v.toString() );
		col->setDefault( def.toString() );
	}
}

void AttrTableDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
	QVariant t = index.model()->data( index, Qt::UserRole + 1 );

	QVariant value;
	if (t.toString() == "text") {
		AttrEditor *txt = static_cast<AttrEditor*>(editor);
		value = txt->text();
	}
	if (t.toString() == "textlist") {
		QComboBox *cbo = static_cast<QComboBox*>(editor);
		value = cbo->currentText();
	}
	if (t.toString() == "boolean") {
		QComboBox *cbo = static_cast<QComboBox*>(editor);
		value = cbo->currentText();
	}
	if (t.toString() == "color") {
		AttrEditorColor * col = static_cast<AttrEditorColor*>(editor);
		value = col->text();
	}
	model->setData(index, value);
}

void AttrTableDelegate::updateEditorGeometry( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex &  ) const
{
	editor->setGeometry( option.rect );
}


