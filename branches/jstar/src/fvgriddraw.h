//
// C++ Interface: fvgrid
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVGRID_H
#define FVGRID_H

#include "fvobject.h"
#include <qgl.h>
#include <QColor>

#include <grid.h>
#include <elem.h>
#include <geom.h>
#include <math.h>
#include <field.h>
#include <fvrequiregrid.h>

class FViewer;
/**
@author Robert Szmurlo
*/
class FVGridDraw : public FVObject
{
    Q_OBJECT

public:
	FVGridDraw(FVBoxMgr * manager, FVObject * parent, int x = 0, int y = 0);
	virtual ~FVGridDraw();
	virtual QString classType() { return QString("FVGridDraw"); }

	virtual void paintGL();
    virtual void updateAttributes();

public slots:
	/*
	virtual void slotMousePress( QMouseEvent * );
	void slotConnect();
	void slotDisconnect(); 
	*/
	

protected:
	FVObject *parent;
	Grid * grid;
	QMap< int, QColor > subColors;
	QMap< int, bool > subVisible;
	
	FVRequireGrid reqGrid;

	QColor getColor(int isub);
	int getVertexSize();
	float getLineWidth();
	void initSubColors();
	void initSubVisibility();
	virtual void setupAttributes();
	virtual void setupMenu( );

	void paintElemsNums();
	void paintVertsNums();
	void drawNormal(QString & paintMode, double dShrink);
	void drawVertices();
	void drawSubdomainWireframe();
	
	
};

#endif
