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

#include "fvfieldsection.h"
#include <fvboxmgr.h>
#include <fvboxfield.h>
#include <grid.h>
#include <field.h>
#include <fieldsection.h>
#include <fvgriddraw.h>
#include <fvboundbox.h>

#include <QtDebug>


FVFieldSection::FVFieldSection(FVBoxMgr * _parent, int x, int y)
	: FVBoxField(_parent,new Field(), x,y)
{
	bFirstSlice = true;
	grid = new Grid();
	
	setupMenu();
}


FVFieldSection::~FVFieldSection()
{
}

void FVFieldSection::slice( FVBoxField * bf )
{
	Field * f = bf->getField();
	Grid * g = bf->getGrid();
	
	delete grid;
	grid = new Grid();
	delete field;
	field = new Field();
			
	
	if (bFirstSlice) {
		bFirstSlice = false;
		g->getCenter( P );
		qDebug() << "Center P = " << P[0] << "," << P[1] << "," << P[2];
		N[0] = 0.0;
		N[1] = 0.0;
		N[2] = 1.0;
	} else {
		readAttributes();
	}
	
	FieldSection fs;
	qDebug() << "Setting P = " << P[0] << "," << P[1] << "," << P[2];
	fs.setParams(P,N);
	qDebug() << "Before section...";
	fs.section(g,f, grid, field );
	qDebug() << "After section...";
	qDebug() << "New grid: nodes = " << grid->getNoNodes() << ", elems = " << grid->getNoElems();
	qDebug() << "New field dim = " << field->dim() << ", values = " << field->size();
	
	setupAttributes();
}

Grid * FVFieldSection::getGrid( )
{
	return grid;
}

Field * FVFieldSection::getField( )
{
	return field;
}

void FVFieldSection::setupMenu( )
{
	Field * f = getField();
	contextMenuObj->clear();

	if (f->dim() > 1)
		contextMenuObj->addAction(tr("&Draw Vectors"), this, SLOT( slotDrawVectors() ) );
	contextMenuObj->addAction(tr("&Draw Colormap"), this, SLOT( slotDrawColormap() ) );
	
	contextMenuObj->addAction(tr("&Draw Grid"), this, SLOT( slotDrawGrid() ) );
	contextMenuObj->addAction(tr("&Draw Bounding Box"), this, SLOT( slotDrawBoundingBox() ) );
	
	contextMenuObj->addSeparator();
	contextMenuObj->addAction(tr("De&lete"),(QWidget*)parent, SLOT(slotDelete()) );
}

void FVFieldSection::slotDrawGrid( )
{
	qDebug() << "FVFieldSection drawing grid...";
	FVGridDraw * gd = new FVGridDraw( parent, grid, childSuggestedX(), childSuggestedY() );
	parent->addObj( gd );
	parent->addCon(this, gd, "", "");
	gd->update();	
}

void FVFieldSection::slotDrawBoundingBox( )
{
	FVBoundBox * bb = new FVBoundBox( parent, childSuggestedX(), childSuggestedY() );
	double p1[3],p2[3];
	grid->getBBox(p1,p2);
	bb->setBBox(p1,p2);
	parent->addObj( bb );
	parent->addCon(this, bb, "", "");
	bb->update();
}

void FVFieldSection::setupAttributes( )
{
	am->clear();
	
	qDebug() << "Setting up attributes for FVFieldSection";
	Attr * a;
	a = am->addAttr( tr("Type"), tr("FieldSlice"), QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("Name"), tr(""), QString("text") );
	a->setEditable(false);
	
	a = am->addAttr( tr("Values No."), QString("%1").arg( field->size() ) , QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("Min. Value"), QString("%1").arg( field->min() ), QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("Max. Value"), QString("%1").arg( field->max() ), QString("text") );
	a->setEditable(false);
	
	am->addSection(tr("Slice attributes"));
	a = am->addAttr( tr("Px"), QString("%1").arg( P[0] ) , QString("text") );
	a = am->addAttr( tr("Py"), QString("%1").arg( P[1] ), QString("text") );
	a = am->addAttr( tr("Pz"), QString("%1").arg( P[2] ), QString("text") );
	a = am->addAttr( tr("Nx"), QString("%1").arg( N[0] ) , QString("text") );
	a = am->addAttr( tr("Ny"), QString("%1").arg( N[1] ), QString("text") );
	a = am->addAttr( tr("Nz"), QString("%1").arg( N[2] ), QString("text") );
	
	qDebug() << "Finished setting up attributes";

}

void FVFieldSection::readAttributes( )
{
	P[0] = getAttrValue(tr("Px")).toDouble();
	P[1] = getAttrValue(tr("Py")).toDouble();
	P[2] = getAttrValue(tr("Pz")).toDouble();
	N[0] = getAttrValue(tr("Nx")).toDouble();
	N[1] = getAttrValue(tr("Ny")).toDouble();
	N[2] = getAttrValue(tr("Nz")).toDouble();
	
	qDebug() << "Attributes = " << P[0] << ", " << P[1] << ", " << P[2] << ": "  << N[0] << ", " << N[1] << ", " << N[2];
}

void FVFieldSection::message( const QList< QString > & argv )
{
	if (argv[0] == QString("update")) {
		FVBoxField * bf = (FVBoxField*) parent->getParentOf( this );
		if (bf == 0) {
		} else {
			slice(bf);
		}
	}
}

void FVFieldSection::updateAttributes( )
{
	parent->sendMessage( QString("update"), this, true );
}



