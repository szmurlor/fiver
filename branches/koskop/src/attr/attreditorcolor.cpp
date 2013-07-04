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


