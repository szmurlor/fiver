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
#include "attreditorcolor.h"
#include <QColorDialog>
#include <QtCore>
#include <QMessageBox>

AttrEditorColor::AttrEditorColor(QWidget * parent)
 : AttrEditor(parent)
{
}


AttrEditorColor::~AttrEditorColor()
{
}

void AttrEditorColor::slotCmdClicked()
{
	bool ok;
	QString r,g,b;
	int ir, ig, ib;

	QRegExp sep("[,\\(\\)\\s]+");
	r = txt->text().section( sep, 1,1);
	g = txt->text().section( sep, 2,2);
	b = txt->text().section( sep, 3,3);

	ir = r.toInt(&ok);
	if (!ok) {
		QMessageBox::critical(this, tr("Color Dialog"), tr("Incorrect red color component: %1").arg(r) );
		return;
	}

	ig = g.toInt(&ok);
	if (!ok) {
		QMessageBox::critical(this, tr("Color Dialog"), tr("Incorrect green color component: %1").arg(g) );
		return;
	}

	ib = b.toInt(&ok);
	if (!ok) {
		QMessageBox::critical(this, tr("Color Dialog"), tr("Incorrect blue color component: %1").arg(b) );
		return;
	}

	QColor c = QColorDialog::getColor(QColor(ir,ig,ib));
	if (c.isValid())
		txt->setText( QString("(%1,%2,%3)").arg(c.red()).arg(c.green()).arg(c.blue()) );
}


