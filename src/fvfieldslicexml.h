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
#ifndef FVFIELDSLICEXML_H
#define FVFIELDSLICEXML_H

#include <fvobject.h>
#include <fvboxmgr.h>
#include <fvrequirefield.h>
#include <fvrequiregrid.h>
#include <fvinterface.h>
#include <fvgridinterface.h>
#include <fvfieldinterfacexml.h>

#include <dolfin/mesh/Mesh.h>
#include <dolfin/function/Function.h>

class FVBoxMgr;
//class Grid;
//class Field;
class FVInteractionTranslation;
class FVInteractionRotation;

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVFieldSliceXml : public FVObject
{
	Q_OBJECT
public:
        FVFieldSliceXml(FVBoxMgr * manager, int x = 0, int y = 0);
        ~FVFieldSliceXml();
	
	void slice();
	
        virtual dolfin::Mesh * getMesh();
        virtual dolfin::Function * getFunction();

        virtual QString classType() { return QString("FVFieldSliceXml"); }
	
	virtual void updateAttributes();
	virtual FVInterface * getInterface( QString interfaceName );
        virtual QString getName();

public slots:
	void slotDrawColormap();
	void slotDrawVectors();
	void slotDrawGrid();
	void slotDrawBoundingBox();
	void slotRotateManipulator();
	void slotTranslateManipulator();
	void slotRemoveManipulator();
	
	void slotTranslate( double , double, double );
        void slotRotate( double, double, double, double, double, double, double);
protected:
	virtual void setupMenu();
	virtual void setupAttributes();
	virtual void readAttributes();
	
private:
//	Grid * grid;
        dolfin::Mesh* mesh;
//	Field * field;
        dolfin::Function* field;
	double P[3];
        double N[3];
	bool bFirstSlice;

	FVRequireGrid reqGrid;
	FVRequireField reqField;

        FVInteractionTranslation * fit;
        FVInteractionRotation * fir;

        FVFieldInterfaceXml * fvFieldInterface;
	FVGridInterface * fvGridInterface;

	void updateSlice();
	
	virtual bool message( const QList<QString> &argv );
};

#endif
