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


