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
#ifndef GUIATTRIBUTES_H
#define GUIATTRIBUTES_H

#include <QDialog>
#include <QListWidget>
#include <QVector>
#include <attrtable.h>
#include <attrmodel.h>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class GUIAttributes : public QDialog
{
	Q_OBJECT
public:
	GUIAttributes(QWidget * parent);
	~GUIAttributes();
	
public slots:
	void slotOK();
	void slotCancel();
	
	void slotCurrentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
	
protected:
	AttrTable * at;
	QListWidget * lw;
	
	QVector< AttrModel * > models;
	QVector< QColor > defColors;
	
	void setUI();
	void buildSections();
	void saveSettings();
	void readSettings();
};

#endif
