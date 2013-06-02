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

#include "fvfieldslicexml.h"
#include <fvboxmgr.h>
#include <fvboxfield.h>
//#include <grid.h>
//#include <field.h>
//#include <simplefield.h>
//#include <geom.h>

//#include <fieldsection.h>

//#include <fvgriddraw.h>
#include <fvmeshdraw.h>
//#include <fvfielddraw.h>
#include <fvfielddrawxml.h>

#include <fvboundbox.h>
#include <fivermain.h>

#include <fvinteractiontranslation.h>
#include <fvinteractionrotation.h>
#include <Helpers.h>
#include <QMessageBox>
#include <QtDebug>
#include <QDir>

using namespace Helpers;

FVFieldSliceXml::FVFieldSliceXml(FVBoxMgr * manager, int x, int y)
	: FVObject(manager, x,y)
{
        sName = "FVFieldSliceXml";
	sType = "Slice";

        fvFieldInterface = new FVFieldInterfaceXml( field );
        fvGridInterface = new FVGridInterface( mesh );

	bFirstSlice = true;
//	field = new Field ();
//	grid = new Grid();
	fit = 0;
	
        setupMenu();
}


FVFieldSliceXml::~FVFieldSliceXml()
{
}

QString FVFieldSliceXml::getName() {
    QString name= sName;
    QString reg( "^.*" );
    reg.append(QDir::separator());
    name.remove( QRegExp(reg) );
    return name;
};

void FVFieldSliceXml::slice()
{
	
        dolfin::Function * f = reqField.getFunction(parentObject());
        dolfin::Mesh * m = reqGrid.getMesh(parentObject());

        sName = f->name().c_str();//->getAttr("name").c_str();
	
//        delete mesh;
//        mesh = new dolfin::Mesh();

//	delete field;
//	field = new dolfin::Function();
			
	
        if (bFirstSlice) {
                bFirstSlice = false;
                FVHelpers::getCenter(m,P);//g->getCenter( P );
                N[0] = 0.0;
                N[1] = 0.0;
                N[2] = 1.0;
        } else {
                readAttributes();
        }
	

//	FieldSection fs;
//	fs.setParams(P,N);

//        SimpleField * sf = new SimpleField();
//	fs.section(g,f->getSimpleField(), grid, sf );
//	field->add(sf);
	
	setupAttributes();
	setupMenu();
}

dolfin::Mesh * FVFieldSliceXml::getMesh( )
{
        return mesh;
}

dolfin::Function * FVFieldSliceXml::getFunction( )
{
	return field;
}

void FVFieldSliceXml::setupMenu( )
{
//        Field * f = getField();
	contextMenuObj->clear();

	QMenu * aMenu = contextMenuObj->addMenu( tr("&Manipulator") );
	aMenu->addAction(tr("&Rotation"), this, SLOT( slotRotateManipulator() ));
	aMenu->addAction(tr("&Translation"), this, SLOT( slotTranslateManipulator() ));
        aMenu->addSeparator();
        aMenu->addAction( tr("Re&move"), this, SLOT( slotRemoveManipulator() ));
	
//	if (f->dim() > 1)
//		contextMenuObj->addAction(tr("Draw &Vectors"), this, SLOT( slotDrawVectors() ) );
	contextMenuObj->addAction(tr("Draw &Colormap"), this, SLOT( slotDrawColormap() ) );
	
	contextMenuObj->addAction(tr("Draw &Grid"), this, SLOT( slotDrawGrid() ) );
	contextMenuObj->addAction(tr("Draw &Bounding Box"), this, SLOT( slotDrawBoundingBox() ) );
	
	contextMenuObj->addSeparator();
	contextMenuObj->addAction(tr("&Delete"),(QWidget*) manager, SLOT(slotDelete()) );

        qDebug() << "FVFieldSliceXml::setupMenu finished";
	
}

void FVFieldSliceXml::slotDrawGrid( )
{
//        qDebug() << "FVFieldSliceXml drawing grid...";
//	FVGridDraw * gd = new FVGridDraw( manager, this, childSuggestedX(), childSuggestedY() );
//	manager->addObj( gd );
//	manager->addCon(this, gd, "", "");
//	gd->update();
}

void FVFieldSliceXml::slotDrawBoundingBox( )
{
//	FVBoundBox * bb = new FVBoundBox( manager, childSuggestedX(), childSuggestedY() );
//	double p1[3],p2[3];
//	grid->getBBox(p1,p2);
//	bb->setBBox(p1,p2);
//	manager->addObj( bb );
//	manager->addCon(this, bb, "", "");
//	bb->update();
}

void FVFieldSliceXml::setupAttributes( )
{
	am->clear();
	
        qDebug() << "FVFieldSliceXml::setupAttributes - Setting up attributes for FVFieldSliceXml";
	Attr * a;
	a = am->addAttr( tr("Type"), tr("Field"), QString("text") );
	a->setEditable(false);
        a = am->addAttr( tr("Name"), tr("FVFieldSliceXml"), QString("text") );
	a->setEditable(false);
	
        qDebug() << "FVFieldSliceXml::setupAttributes - accessing field";
//	a = am->addAttr( tr("Values No."), QString("%1").arg( field->size() ) , QString("text") );
//	a->setEditable(false);
//        a = am->addAttr( tr("Elems No."), QString("%1").arg( grid->getNoElems() ) , QString("text") );
//        a->setEditable(false);
//        a = am->addAttr( tr("Min. Value"), QString("%1").arg( field->min() ), QString("text") );
//	a->setEditable(false);
//	a = am->addAttr( tr("Max. Value"), QString("%1").arg( field->max() ), QString("text") );
//	a->setEditable(false);
	
	am->addSection(tr("Slice attributes"));
	a = am->addAttr( tr("Px"), QString("%1").arg( P[0] ) , QString("text") );
	a = am->addAttr( tr("Py"), QString("%1").arg( P[1] ), QString("text") );
	a = am->addAttr( tr("Pz"), QString("%1").arg( P[2] ), QString("text") );
	a = am->addAttr( tr("Nx"), QString("%1").arg( N[0] ) , QString("text") );
	a = am->addAttr( tr("Ny"), QString("%1").arg( N[1] ), QString("text") );
	a = am->addAttr( tr("Nz"), QString("%1").arg( N[2] ), QString("text") );
	
	qDebug() << "Finished setting up attributes";

}

void FVFieldSliceXml::readAttributes( )
{
	P[0] = getAttrValue(tr("Px")).toDouble();
	P[1] = getAttrValue(tr("Py")).toDouble();
	P[2] = getAttrValue(tr("Pz")).toDouble();
	N[0] = getAttrValue(tr("Nx")).toDouble();
	N[1] = getAttrValue(tr("Ny")).toDouble();
	N[2] = getAttrValue(tr("Nz")).toDouble();
	
//	if (fit != 0) {
//		fit->setP( P );
//		fit->setN( N );
//	}
	qDebug() << "Attributes = " << P[0] << ", " << P[1] << ", " << P[2] << ": "  << N[0] << ", " << N[1] << ", " << N[2];
}

void FVFieldSliceXml::updateAttributes( )
{
	updateSlice();
	manager->sendMessage( QString("update"), this, true );
}

void FVFieldSliceXml::slotDrawVectors( )
{
//	FVFieldDraw * fd = new FVFieldDraw(manager,this);
//	addChild(fd,QString("DrawField"),QString("Draw"));
//	fd->setAttrValue( QString("View Style"), QString("vectors") );
//	fd->update();
}

void FVFieldSliceXml::slotRotateManipulator( )
{
//    if (fvinteraction != 0) {
//            delete fvinteraction;
//    }
//    double p[3];
//    getGrid()->getCenter(p);
//    double length, size;
//    double bb[3],b1[3],b2[3];
//    getGrid()->getBBox(b1,b2);
//    getGrid()->getCenter(p);
//    SUB(bb,b2,b1);
//    size = bb[0];
//    size = size > bb[1] ? size : bb[1];
//    size = size > bb[2] ? size : bb[2];

//    if(size == 0.0)
//        size = 1.0;

//    length = (size * 1.5)/2.0;
//    size = (size * 1.1)/2.0;

//    fir = new FVInteractionRotation(this, p, size, length);
//    fvinteraction = fir;
//    connect(fir, SIGNAL(signalRotate(double, double, double,
//                                     double, double, double, double)),
//            this,    SLOT(slotRotate(double, double, double,
//                                     double, double, double, double )) );
//    getCurrentViewer()->setInteractionModel(fir);
//    update();
}

void FVFieldSliceXml::slotTranslateManipulator( )
{
//    if (fvinteraction != 0) {
//            delete fvinteraction;
//    }

//    double length, size, w, h;
//    double bb[3],b1[3],b2[3];
//    double p[3], n[3], u[3], v[3];
//    getGrid()->getBBox(b1,b2);
//    getGrid()->getCenter(p);
//    SUB(bb,b2,b1);

//    ZERO(n);
//    ZERO(u);
//    ZERO(v);
//    n[2] = 1.0;
//    length = bb[2];
//    u[0] = 1.0;
//    w = bb[0];
//    v[1] = 1.0;
//    h = bb[1];

//    size = w > h ? w : h;
//    size = (size * 1.1)/2.0;

//    if(size == 0.0)
//        size = 1.0;
//    if(length == 0.0)
//        length = 1.0;
//    length = (length * 1.5)/2.0;
//    fit = new FVInteractionTranslation(this, p, n, u, v, length, size);
//    fvinteraction = fit;
//    connect(fit, SIGNAL(signalTranslate(double, double, double)), this, SLOT(slotTranslate(double, double, double)) );
//    getCurrentViewer()->setInteractionModel(fit);
//    update();
}

void FVFieldSliceXml::slotTranslate( double dx, double dy, double dz)
{
	// qDebug() << "dx = " << dx << " dy = " << dy << "dz = " << dz;
	
	readAttributes();
	P[0] += dx;
	P[1] += dy;
	P[2] += dz;
	setAttrValue( tr("Px"), QString("%1").arg(P[0]));
	setAttrValue( tr("Py"), QString("%1").arg(P[1]));
	setAttrValue( tr("Pz"), QString("%1").arg(P[2]));
	
	updateSlice();
	manager->sendMessage( QString("update"), this, true );
}

void FVFieldSliceXml::slotRotate( double px, double py, double pz,
                            double ax, double ay, double az, double angle){
//        qDebug() << "slotRotate:"
//                 << "c(" << pz << "," << py << "," << pz << "), "
//                 << "a(" << ax << ", " << ay << ", " << az << "), "
//                 << "@" << angle;
        double p[3] = {px, py, pz};
        double a[3] = {ax, ay, az};
        readAttributes();
        Helpers::rotate(N, a, angle);
        setAttrValue( tr("Nx"), QString("%1").arg(N[0]));
        setAttrValue( tr("Ny"), QString("%1").arg(N[1]));
        setAttrValue( tr("Nz"), QString("%1").arg(N[2]));

//        updateSlice();
//        manager->sendMessage( QString("update"), this, true );

//        getGrid()->rotate(p, a, angle);
//        manager->sendMessage( QString("update"), this, true );
}
void FVFieldSliceXml::slotRemoveManipulator( )
{
	if (fvinteraction != 0) {
		delete fvinteraction;
		fvinteraction = 0;
		fit = 0;
		getCurrentViewer()->setInteractionModel( getInteractionModel() );
	}
}

void FVFieldSliceXml::updateSlice( )
{
	qDebug() << "slicing...";
	slice();
}

FVInterface * FVFieldSliceXml::getInterface( QString interfaceName )
{
	if (interfaceName == QString("FVFieldInterface"))
		return fvFieldInterface;
	if (interfaceName == QString("FVGridInterface"))
		return fvGridInterface;

	return parentInterface( interfaceName );
}

void FVFieldSliceXml::slotDrawColormap( )
{
//	FVFieldDraw * fd = new FVFieldDraw( manager, this );
//	addChild(fd, QString("DrawField"),QString("Draw"));
//	fd->update();
}

bool FVFieldSliceXml::message( const QList< QString > &argv )
{
//	if (argv.size() > 0) {
//		if (argv[0] == "update") {
//			updateSlice();
//			return true;
//		}
//	}
	
	return false;
}
