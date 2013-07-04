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
#ifndef FVFIELDSECTION_H
#define FVFIELDSECTION_H

#include <fvboxfield.h>

class FVBoxMgr;
class Grid;
class Field;

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVFieldSection : public FVBoxField
{
	Q_OBJECT
public:
	FVFieldSection(FVBoxMgr * _parent, int x = 0, int y = 0);
	~FVFieldSection();
	
	void slice( FVBoxField * bf );
	
	virtual Grid * getGrid();
	virtual Field * getField();

	virtual QString classType() { return QString("FVFieldSection"); }
	
	virtual void updateAttributes();

public slots:
	void slotDrawGrid();
	void slotDrawBoundingBox();
	
	virtual void message( const QList<QString> &argv );
	
protected:
	virtual void setupMenu();
	virtual void setupAttributes();
	virtual void readAttributes();
	
private:
	Grid * grid;
	double P[3];
	double N[3];
	bool bFirstSlice;
};

#endif
