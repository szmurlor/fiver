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
#ifndef FVFIELDDRAW_H
#define FVFIELDDRAW_H

#include <fvobject.h>
#include <fvboxfield.h>
#include <colormap.h>
#include <isolines.h>
#include <setofint.h>
#include <fvrequirefield.h>
#include <fvrequiregrid.h>
#include <QtDebug>
#include <QString>

class FVBoxMgr;
class Field;

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVFieldDraw : public FVObject
{

	Q_OBJECT
	
public:
	FVFieldDraw(FVBoxMgr * manager, FVObject * boxField, int x = 0, int y = 0);
	~FVFieldDraw();

	virtual QString classType() { return QString("FVFieldDraw"); }
	
	virtual void paintGL();

    virtual FVInterface * getInterface( QString interfaceName );
    virtual void updateAttributes();

public slots:
    void slotDrawFieldLegend();

protected:
	FVObject * boxField;

	QString drawMode;
	
	int isolines_no;
	double isolinesWidth;
	
	double minValue;
	double maxValue;
	bool logScale;

	QString visibleSub;
	int minSub, maxSub;
	ColorMap::ColorMapType colormapType;
	
	/** Used for vector length. */
	double modelBaseLength;
	double baseLength;
	bool sameLength;
	bool sameColor;
	QColor sameColorColor;
	double vectorsDensity;

    virtual void setupAttributes();
	virtual void readAttributes();
    virtual void setupMenu();
	
	bool showElem( int ie );
	bool showNode( int in );
	
	void drawColormap( int ie, CFace & face );
	void drawIsolines( int ie, CFace & face );
	void drawVectorElem( int ie );
	void drawVectorNode( int in );
	
        void drawVector( double c[3], double v[3] );
	
private:
	Grid * grid;
	Field * field;
	ColorMap cm;
	IsoLines iso;
	SetOfInt visSub;

	void fillValuesOfFace(int ie,CFace &face, double e[3][3], double v[3]);

        void calculateMinMax( void );

	FVRequireField reqField;
	FVRequireGrid reqGrid;

        double vectorHeadLength;
        double vectorHeadThickness;
        double vectorThickness;
        int headFaces;
        bool dontMissBigV;
};

#endif
