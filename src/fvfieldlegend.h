/***************************************************************************
 *   Copyright (C) 2008 by Robert Szmurlo   *
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
#ifndef FVFIELDLEGEND_H
#define FVFIELDLEGEND_H

#include <fvobject.h>
#include <fvrequiregrid.h>
#include <fvrequirefield.h>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVFieldLegend : public FVObject
{
public:
    double minValue;
    double maxValue;
    int x,y;

    FVFieldLegend(FVBoxMgr * manager,  int x = 0, int y = 0);

    ~FVFieldLegend();
	
    virtual QString classType() { return QString("FVFieldDraw"); }
	
    virtual void paintGL();
    virtual bool message( const QList<QString> &argv );

protected:
    FVRequireGrid reqGrid;
    FVRequireField reqField;
    bool bGotMessageMinMax;

    virtual void setupAttributes();
    virtual void readAttributes();

};

#endif
