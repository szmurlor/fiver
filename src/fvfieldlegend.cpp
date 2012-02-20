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
#include "fvfieldlegend.h"
#include <fivermain.h>
#include <QDebug>
#include <colormap.h>


FVFieldLegend::FVFieldLegend(FVBoxMgr * manager,  int x, int y)
    : FVObject(manager,x,y)
{
    initDrawable( "DrawFieldLegend", "Draw" );

    bGotMessageMinMax = false;
    minValue = 0;
    maxValue = 0;
}

FVFieldLegend::~FVFieldLegend()
{
}

void FVFieldLegend::setupAttributes()
{
	Attr * a;
	a = am->addAttr( tr("Type"), tr("Field"), QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("Name"), tr(""), QString("text") );
	a->setEditable(false);
    a = am->addAttr( tr("Position X:"), tr("10"), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Position Y:"), tr("30"), QString("text") );
    a->setEditable(true);
}

bool FVFieldLegend::message( const QList< QString > &  args)
{
    if (args.size() > 0) {
        if (args[0] == "min_max_field") {
            minValue = args[1].toDouble();
            maxValue = args[2].toDouble();
            bGotMessageMinMax = true;

            return true;
        }
    }

    return false;
}

void FVFieldLegend::readAttributes()
{
    x = am->getAttr("Position X:")->value().toInt();
    y = am->getAttr("Position Y:")->value().toInt();

    FVObject * parent = parentObject();
    if (parent != 0) {
        if (parent->getAttrValue( tr("Use Custom Min/Max:") ) == "Yes") {
            minValue = parent->getAttrValue( tr("Custom Min:") ).toDouble();
            maxValue = parent->getAttrValue( tr("Custom Max:") ).toDouble();
            // qDebug() << "Custom min/max = " << minValue << ", " << maxValue;
        } else {
            if (bGotMessageMinMax == false) {
                Field * f = reqField.getField( parentObject() );
                if (f != 0) {
                    minValue = f->min();
                    maxValue = f->max();
                }
                // qDebug() << "Field min/max = " << minValue << ", " << maxValue;
            } // else
                // qDebug() << "Using min/max from message: " << minValue << " , " << maxValue;
        }
    } else
        qDebug() << "Nie mogę odnaleźć obiektu rodzica dla " + sName;
}

void FVFieldLegend::paintGL()
{
	readAttributes();
	
	if (isFirstShow()) {
	}

    QString fieldName = "";
    Field * f = reqField.getField( parentObject() );
    if (f != 0)
        fieldName = f->getAttr("name").c_str();

	fvlist->start();
	getCurrentViewer()->startScreenCoordinatesSystem();
    getCurrentViewer()->fvRenderText(x, y, QString("Legend: " + fieldName));
    getCurrentViewer()->fvRenderText(x + 25, y + 15, QString::number(minValue));
    getCurrentViewer()->fvRenderText(x + 25, y + 45, QString::number(maxValue));

    ColorMap cm;
    cm.setDoGLInit(true);
    cm.setMinMax(minValue,maxValue,false);
    cm.init( ColorMap::Jet);

    double e[3][3];
    double v[3];
    v[0] = minValue;
    v[1] = minValue;
    v[2] = maxValue;
    e[0][0] = x;
    e[0][1] = y+10;
    e[0][2] = 0;
    e[1][0] = x+20;
    e[1][1] = y+10;
    e[1][2] = 0;
    e[2][0] = x;
    e[2][1] = y+40;
    e[2][2] = 0;
    cm.drawTriangleGL(e,v);

    v[0] = minValue;
    v[1] = maxValue;
    v[2] = maxValue;
    e[0][0] = x+20;
    e[0][1] = y+10;
    e[0][2] = 0;
    e[1][0] = x+20;
    e[1][1] = y+40;
    e[1][2] = 0;
    e[2][0] = x;
    e[2][1] = y+40;
    e[2][2] = 0;
    cm.drawTriangleGL(e,v);

    getCurrentViewer()->stopScreenCoordinatesSystem();
		
	fvlist->end();
}


