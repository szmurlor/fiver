//
// C++ Interface: fvgridbnd
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVGRIDBND_H
#define FVGRIDBND_H

#include <fvobject.h>
#include <grid.h>
#include <geom.h>
#include <fvrequiregrid.h>

/**
@author Robert Szmurlo
*/
class FVGridBnd : public FVObject
{
	Q_OBJECT

public:
	FVGridBnd(FVBoxMgr * manager, FVObject * parent, int x = 0, int y = 0 );
	~FVGridBnd();

	virtual QString classType() { return QString("FVGridBnd"); }
	
	virtual void paintGL();
		
public slots:
	void slotApply();	

protected:
	Grid * grid;
	QColor color;
        QMap< int, QColor > subColors;
	int minBnd, maxBnd;
        double sphereBoundaryRadius;
	
	FVRequireGrid reqGrid;

	bool isFaceOnBnd( CFace &f, int xb );
	void drawNodes(double td);
	void readAttributes();
	virtual void setupAttributes();
	virtual void setupMenu();
        QColor getColor(int ibnd);
        void initSubColors();
};

#endif
