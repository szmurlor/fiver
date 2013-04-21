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

#include "fvfielddraw.h"

#include <math.h>

#include <geom.h>
#include <field.h>
#include <grid.h>
#include <elem.h>
#include <elemlist.h>

#include <fvboxmgr.h>
#include <fivermain.h>
#include <fvfieldinterface.h>
#include <fvgridinterface.h>
#include <fvfieldlegend.h>
#include <main.h>
#include <fvboxmeshfunction.h>

#include <GL/glu.h>

FVFieldDraw::FVFieldDraw(FVBoxMgr * manager, FVObject * boxField, int x, int y)
 : FVObject(manager,x,y)
{
	this->boxField = boxField;

    if (reqField.getField(boxField) != 0){
        minValue= reqField.getField(boxField)->min();
        maxValue= reqField.getField(boxField)->max();
        Grid * g = reqGrid.getGrid(boxField);

        if (g != 0) {
                maxSub = g->_subdomains.size();
        } else {
                maxSub = 1;
        }
    } else if (reqField.getMeshFun(boxField) != 0 ){
        std::cout << "jest funkcja" << std::endl;
        minValue= reqField.getMeshFun(boxField)->min();
        maxValue= reqField.getMeshFun(boxField)->max();
        dolfin::Mesh*  m = reqGrid.getMesh(boxField);
        maxSub = 1;
    } else
        qDebug() << "FVFieldDraw: Unable to get mesh from " << boxField->classType() << "!" ;

        minSub = 1;

        modelBaseLength = 0;
        baseLength = 0;
	
        initDrawable( "DrawField", "Draw" );
}

void FVFieldDraw::updateAttributes( )
{
    // jstar: trochę dziwnie to tu wygląda - ale po nacisnieciu Apply
    // trzeba sprawdzic, czy nie zmienila sie lista rysowanych podobszarow
    // i w razie zmiany poprawić CustomMin i CustomMax
    // ale tylko wtedy, gdy użytkownik ich nie ruszal
    if (getAttrValue( tr("Use Custom Min/Max:") ) != "Yes") {
        visibleSub = getAttrValue( tr("Visible Subdomains") ); // pozniej dodac sprawdzenie
        visSub.init( visibleSub, minSub, maxSub );
        calculateMinMax();
        am->updateAttr( tr("Custom Min:"), tr("%1").arg(minValue), QString("text"), tr("%1").arg(minValue) );
        am->updateAttr( tr("Custom Max:"), tr("%1").arg(maxValue), QString("text"), tr("%1").arg(maxValue) );
    }
    FVObject::update();
    manager->sendMessage(QString("update"), this, true );
}

FVInterface * FVFieldDraw::getInterface( QString interfaceName )
{
    return parentInterface(interfaceName);
}

FVFieldDraw::~FVFieldDraw()
{
}

bool FVFieldDraw::showElem( int ie )
{
	Elem * el = grid->_elems[ ie ];

	if ( ! el->isExternal() )
		return false;
	if( visibleSub.length() > 0 && ! visSub.find( el->subdomain() )  )
		return false;
	
	return true;
}

void FVFieldDraw::fillValuesOfFace( int ie, CFace & face, double e[3][3], double v[3] )
{
	Node * n;
	
	// read geomtery of the face
	n = &grid->_nodes[ face.nodes[0] - 1 ];
	SET(e[0],n->_Coords);
	n = &grid->_nodes[ face.nodes[1] - 1 ];
	SET(e[1],n->_Coords);
	n = &grid->_nodes[ face.nodes[2] - 1 ];
	SET(e[2],n->_Coords);
	
	if (field->size() == grid->getNoNodes()) {
		v[0] = field->val( face.nodes[0] - 1);
		v[1] = field->val( face.nodes[1] - 1);
		v[2] = field->val( face.nodes[2] - 1);
	} else {
		double tmpv = 0;
		if (field->dim() == 1) {
			tmpv = field->val( 0, ie );
		} else {
			for (int d = 0; d < field->dim(); d++)
				tmpv += field->val( d, ie ) * field->val( d, ie );
			tmpv = sqrt(tmpv);
		}
		v[0] = tmpv;
		v[1] = tmpv;
		v[2] = tmpv;
	}
}

void FVFieldDraw::drawColormap( int ie, CFace & face )
{
	double e[3][3];
	double v[3];
	
	fillValuesOfFace(ie,face,e,v);	
		
	double vec1[3], vec2[3], norm[3];
	SUB(vec1, e[0],  e[2]);
	SUB(vec2, e[0],  e[1]);
	CROSS(norm,vec1,vec2);
	VERSOR(norm,norm);
	SET(cm.norm, norm);
	
	cm.drawTriangleGL(e,v);
}

void FVFieldDraw::drawIsolines( int ie, CFace & face )
{
	double e[3][3];
	double v[3];
	
	fillValuesOfFace(ie,face,e,v);

	iso.drawTriangleGL(e,v);
}

void FVFieldDraw::drawVector( double c[], double v[])
{

	double vn[3];
	double lv[3];
	double ang;
	double l;

        double headLgt= vectorHeadLength;
        double headThick= vectorHeadThickness;

#if 0
        // do testowania
        glDisable(GL_BLEND);
        glDisable(GL_LIGHTING);
        glLineWidth(1.0);
        glBegin( GL_LINES );

        glVertex3f( c[0], c[1], c[2] );
        glVertex3f( c[0]+(1-headLgt)*v[0], c[1]+(1-headLgt)*v[1], c[2]+(1-headLgt)*v[2] );
        glLineWidth(5.0);
        glVertex3f( c[0]+v[0], c[1]+v[1], c[2]+v[2] );
        glEnd();
#endif

	l = LENGTH(v);
	VERSOR(lv,v);
	vn[0] = -lv[1];
	vn[1] = lv[0];
	vn[2] = 0;
	
	/* ang is the angle between OZ axis and normalized lv */
	ang = acos( lv[2] ) * 180 / M_PI;

        vn[0] *= headLgt;
        vn[1] *= headLgt;
	
	glPushMatrix();
        //glTranslatef(c[0]+(1-headLgt)*v[0], c[1]+(1-headLgt)*v[1], c[2]+(1-headLgt)*v[2]);
        glTranslatef(c[0], c[1], c[2]);
	glScalef( l, l, l );
	glRotatef(ang, vn[0], vn[1], vn[2]);

	GLUquadric * quad = gluNewQuadric();

        //walec
        gluQuadricDrawStyle( quad, GLU_FILL );
        gluCylinder( quad, vectorThickness, vectorThickness, 1.0-headLgt, headFaces, 1 );

        // podstawa walca
        gluQuadricDrawStyle( quad, GLU_FILL );
        gluQuadricOrientation(quad, GLU_INSIDE);
        gluDisk(quad, 0.0, vectorThickness, headFaces, 1);

        // stozek
        gluQuadricDrawStyle( quad, GLU_FILL );
        gluQuadricOrientation(quad, GLU_OUTSIDE);
        glTranslatef(0,0,1-headLgt);
        gluCylinder( quad, headThick*headLgt, 0.0, headLgt, headFaces, 1 );

        // podstawa stozka
        gluQuadricDrawStyle( quad, GLU_FILL );
        gluQuadricOrientation(quad, GLU_INSIDE);
        gluDisk(quad, 0.0, headThick*headLgt, headFaces, 1);


        gluDeleteQuadric( quad );

	glPopMatrix();
  
}

void FVFieldDraw::drawVectorElem( int ie )
{
    std::vector<double> c;
    double fscale;
    //double scale;
    double r,g,b;
    double modv;
    Elem * el;
    double v[3];

    if ( maxValue > 0 )
        if( logScale )
            fscale = 0.05 * modelBaseLength * baseLength / log(maxValue/minValue);
    else
        fscale = 0.05 * modelBaseLength *baseLength / maxValue;
    else
        fscale = 1;

    if( sameColor ) {
        r = (GLfloat) sameColorColor.red()/255;
        g = (GLfloat) sameColorColor.green()/255;
        b = (GLfloat) sameColorColor.blue()/255;
        glColor3f( r,g,b );
    }

    int de= grid->_elems.size() < 200 ? 1 : grid->_elems.size() / 400;
    de = (int)( de / vectorsDensity);

    el = grid->_elems[ ie ];
    c = el->center();
    v[2] = 0;
    modv = 0;
    for (int i = 0; i < field->dim(); i++) {
        modv += field->val(i,ie) * field->val(i,ie);
        v[i] = field->val(i,ie );
    }
    modv = sqrt(modv);
    VERSOR( v, v );
    if( sameLength ) {
        MULT( v, v, 0.05* modelBaseLength *baseLength );
        //scale= 1.0;
    } else {
        if( logScale ) {
            MULT( v, v, log(modv/minValue) * fscale );
            //scale= log(modv/maxValue);
        } else {
            MULT( v, v, modv * fscale );
            //scale= modv/maxValue;
        }
    }

    cm.setMinMax(minValue,maxValue,logScale);

    if( (dontMissBigV && modv > 0.95 * maxValue) || de < 2 || ie % de == 0 ) { // dont miss big vectors!
        if( !sameColor ) {
            cm.getRGBColor( modv, r, g, b);
            glColor3f(r,g,b);
        }

        double c1[3];
        SET(c1,c);
        drawVector(c1,v);
    }
}

void FVFieldDraw::drawVectorNode( int in )
{
    double fscale;
    //double scale;
    double r,g,b;
    double modv;
    Node * n;
    double v[3];

    if ( maxValue > 0 )
        if( logScale )
            fscale = 0.05 * modelBaseLength * baseLength / log(maxValue/minValue);
    else
        fscale = 0.05 * modelBaseLength *baseLength / maxValue;
    else
        fscale = 1;

    if( sameColor ) {
        r= (GLfloat) sameColorColor.red()/255;
        g= (GLfloat) sameColorColor.green()/255;
        b= (GLfloat) sameColorColor.blue()/255;
        glColor3f( r,g,b );
    }

    int de= grid->_nodes.size() < 200 ? 1 : grid->_nodes.size() / 400;
    de = (int)( de / vectorsDensity);

    n = &grid->_nodes[ in ];
    v[2] = 0;
    modv = 0;
    for (int i = 0; i < field->dim(); i++) {
        modv += field->val(i,in) * field->val(i,in);
        v[i] = field->val(i,in );
    }
    modv = sqrt(modv);
    VERSOR( v, v );
    if( sameLength ) {
        MULT( v, v, 0.05* modelBaseLength *baseLength );
        //scale= 1.0;
    } else {
        if( logScale ) {
            MULT( v, v, log(modv/minValue) * fscale );
            //scale= log(modv/maxValue);
        } else {
            MULT( v, v, modv * fscale );
            //scale= modv/maxValue;
        }
    }

    cm.setMinMax(minValue,maxValue,logScale);

    if( (dontMissBigV && modv > 0.95 * maxValue) || de < 2 || in % de == 0 ) { // dont miss big vectors!
        if( !sameColor ) {
            cm.getRGBColor( modv, r, g, b);
            glColor3f(r,g,b);
        }

        drawVector(n->_Coords,v);
    }
}


void FVFieldDraw::paintGL( )
{
//	int ie, in;
//	int iFace;
//	Elem * el;
//	CFace face;
//	GLfloat tx,ty,tz;

//        qDebug("FVFieldDraw::paintGL started");
	
//	field = reqField.getField(parentObject());
//        mf = reqField.getMeshFun(parentObject());
//	grid = reqGrid.getGrid(parentObject());
//        mesh = reqGrid.getMesh(parentObject());
	
//	if (grid == 0) {
//		qWarning() << "FVFieldDraw: Grid pointer for " << sName << " is NULL. Unable to draw field.";
//		return;
//	}
//        if (mesh == 0) {
//                qWarning() << "FVFieldDraw: Mesh pointer for " << sName << " is NULL. Unable to draw field.";
//                return;
//        }

//	readAttributes();

//	if (isFirstShow()) {
//		double pmin[3], pmax[3];
//		grid->getBBox( pmin, pmax );
//		tx = (pmax[0] - pmin[0]) ;
//		ty = (pmax[1] - pmin[1]) ;
//		tz = (pmax[2] - pmin[2]) ;
		
//        double td = sqrt(tx*tx + ty*ty + tz*tz);
//        if ((td > 1e-10 ) && (td < 1e10)) {
//            getCurrentViewer()->setSceneRadius( td );
//            modelBaseLength = td;
//        }
//	}
    
    // Poinformuj wszystkie dzieci o aktualnej skali,
    // która zostanie przekazana  do ColorMapy
//    QList<QString> args;
//    args.append(QString("min_max_field"));
//    args.append(QString::number(minValue));
//    args.append(QString::number(maxValue));
//    manager->sendMessageToChildrenOf( args, this );
//    ///////////////////////////////////////////////

//    fvlist->start();
//    if (drawMode == "colormap" ) {
//		cm.setDoGLInit(false);
//		cm.setMinMax(minValue,maxValue,logScale);
//		cm.init( colormapType );
		
//		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//		glBegin(GL_TRIANGLES);
//	}
//	if (drawMode == "isolines") {
//		cm.setDoGLInit(true);
//		iso.isolines_no = isolines_no;
//		iso.setWidth(isolinesWidth);
//		iso.setMinMax(minValue,maxValue,logScale);
//		glDisable(GL_LIGHTING);
//	}
	
//	if ( (drawMode == "colormap") || (drawMode == "isolines") ) {
//		for (ie = 0; ie < (int) grid->_elems.size();  ie++) {
//			if (showElem( ie )) {
//				el = grid->_elems[ ie ];
//				for (iFace = 0; iFace < el->getFacesCount(); iFace++) {
//					int inbr = el->getNeighbourWithFaceNr(iFace);
//					if (inbr != 0) {
//						if (grid->_elems[ inbr - 1 ]->subdomain() != el->subdomain())
//							inbr = 0;
//					}
//					if (inbr == 0) {
//						face = el->getFaceOrder( iFace );
//						if (drawMode == "colormap")
//							drawColormap( ie, face );
//						if (drawMode == "isolines")
//							drawIsolines( ie, face );
//					}
//				}
//			}
//		}
//	}
	
//	if ( drawMode == "vectors" ) {
//            vectorThickness= fvsettings.value( QString("/RSoft/FViewer/vector.thickness"), QVariant("0.04") ).toDouble();
//            dontMissBigV= fvsettings.value( QString("/RSoft/FViewer/vector.dont-miss-big-vectors"), QVariant("true") ).toBool();
//            vectorHeadLength= fvsettings.value( QString("/RSoft/FViewer/vector.head.length"), QVariant("0.5") ).toDouble();
//            vectorHeadThickness= fvsettings.value( QString("/RSoft/FViewer/vector.head.thickness"), QVariant("0.2") ).toDouble();
//            headFaces= fvsettings.value( QString("/RSoft/FViewer/vector.head.faces"), QVariant("4") ).toInt();
//            if( ! fvsettings.value( QString("/RSoft/FViewer/vector.lighting"), QVariant("false") ).toBool() )
//                glDisable(GL_LIGHTING);
//            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
//            cm.init( colormapType );
//            if ( field->size() == grid->getNoElems()) {
//                for (ie = 0; ie < (int) grid->_elems.size();  ie++) {
//                    if (showElem(ie)) {
//                        drawVectorElem(ie);
//                    }
//                }
//            } else {
//                for (in = 0; in < (int) grid->_nodes.size();  in++) {
//                    if (showNode(in+1)) {
//                        drawVectorNode(in);
//                    }
//                }
//            }
//	}
	
//	if (drawMode == "colormap" ) {
//		glEnd();
//	}
//	glEnable(GL_LIGHTING);
//	fvlist->end();
}

void FVFieldDraw::setupAttributes( )
{
	Attr * a;
	a = am->addAttr( tr("Type"), tr("Field"), QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("Name"), tr(""), QString("text") );
	a->setEditable(false);
        	
	a = am->addAttr( tr("Use Custom Min/Max:"), QString("No"), QString("boolean") );
	a = am->addAttr( tr("Custom Min:"), tr("%1").arg(minValue), QString("text"), tr("%1").arg(minValue) );
	a = am->addAttr( tr("Custom Max:"), tr("%1").arg(maxValue), QString("text"), tr("%1").arg(maxValue) );

	a = am->addAttr( tr("View Style"), tr("colormap"), QString("textlist") );
	a->addListItem( tr("colormap") );
	a->addListItem( tr("isolines") );
	a->addListItem( tr("vectors") );

	a = am->addAttr( tr("Log. Scale"), tr("Linear"), QString("textlist") );
	a->addListItem( tr("Linear") );
	a->addListItem( tr("Log10") );

	a = am->addAttr( tr("Colormap Type"), tr("Jet"), QString("textlist") );
	a->addListItem( tr("Jet") );
	a->addListItem( tr("Gray") );
	a->addListItem( tr("XRay") );

	a = am->addAttr( tr("Visible Subdomains"), tr("%1-%2").arg(minSub).arg(maxSub), QString("text") );
	a = am->addAttr( tr("No. of Isolines"), tr("15"), QString("text") );
	a = am->addAttr( tr("Isolines Width"), tr("1.0"), QString("text") );
	
	am->addSection( tr("Vectors") );
	a = am->addAttr( tr("+ Length"), tr("1.0"), QString("text") );
	a = am->addAttr( tr("+ Same Color"), tr("No"), QString("boolean") );
	a = am->addAttr( tr("+ Same Color Value"), tr("(125,125,125)"), QString("color") );
        sameLength = fvsettings.value( QString("/RSoft/FViewer/vector.same.length"), QVariant("true") ).toBool();
        a = am->addAttr( tr("+ Same Length"), (sameLength ? tr("Yes") : tr("No")), QString("boolean") );
	a = am->addAttr( tr("+ Density"), tr("1.0"), QString("text") );
	
}

void FVFieldDraw::readAttributes( )
{
    QString cmt = getAttrValue( tr("Colormap Type") );
    if (cmt == "Jet") colormapType = ColorMap::Jet;
    if (cmt == "Gray") colormapType = ColorMap::Gray;
    if (cmt == "XRay") colormapType = ColorMap::XRay;

    visibleSub = getAttrValue( tr("Visible Subdomains") );
    visSub.init( visibleSub, minSub, maxSub );
    isolines_no = getAttrValue( tr("No. of Isolines") ).toInt();
    isolinesWidth = getAttrValue( tr("Isolines Width") ).toDouble();
    baseLength = getAttrValue( tr("+ Length") ).toDouble();
    sameColor = (getAttrValue( tr("+ Same Color") ) == "Yes");
    vectorsDensity = getAttrValue( tr("+ Density") ).toDouble();
    sameLength = (getAttrValue( tr("+ Same Length") ) == "Yes");
    Attr * a = getModel()->getAttr( tr("+ Same Color Value" ) );
    if (a != 0) sameColorColor = a->color();

    if (getAttrValue( tr("Use Custom Min/Max:") ) == "Yes") {
        minValue = getAttrValue( tr("Custom Min:") ).toDouble();
        maxValue = getAttrValue( tr("Custom Max:") ).toDouble();
    } // w przeciwnym wypadku (Use Custom.. == No) min i max ustawione w updateAtrri

    drawMode = getAttrValue( tr("View Style") );

    if ( (getAttrValue( tr("Log. Scale") ) == "Log10") && (minValue > 0) )
        logScale = true;
    else
        logScale = false;
}

void FVFieldDraw::calculateMinMax( void ) {
    field = reqField.getField(parentObject());
    grid = reqGrid.getGrid(parentObject());

    if( field != 0 ) {
        if( visibleSub.length() == 0 ) {
            minValue= field->min( );
            maxValue= field->max( );
        } else {
            minValue= 1e78;
            maxValue= -1e78;
            qDebug() << "FVFieldDraw::calculateMinMax: iterating over visible subdomains";
            for( std::set<int>::iterator i= visSub.begin(); i != visSub.end(); i++ ) {
                double minI= field->min(grid,*i);
                double maxI= field->max(grid,*i);
                qDebug() << "FVFieldDraw::readAttributes: subDomain no. " << *i << ": min=" << minI << " max=" << maxI;
                minValue = minValue < minI ? minValue : minI;
                maxValue = maxValue > maxI ? maxValue : maxI;
            }
        }
    }
}

bool FVFieldDraw::showNode( int in )
{

	Node * n = &grid->_nodes[ in ];
        if( n == 0 )
            return false;

        if( visibleSub.length() == 0 )
            return true;

        vector<int> *elems= grid->getNodeNbrs( in);

        for( vector<int>::iterator i= elems->begin(); i != elems->end(); i++ ) {
            Elem * el = grid->_elems[ *i - 1 ];

            if( visSub.find( el->subdomain() )  ) // wiadomo, że visibleSub.length() > 0
                return true;
        }

        return false;
}

void FVFieldDraw::setupMenu()
{
    contextMenuObj->clear();

    contextMenuObj->addAction(tr("&Draw Field Legend"), this, SLOT( slotDrawFieldLegend() ) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );

}

void FVFieldDraw::slotDrawFieldLegend()
{
    FVFieldLegend * fl = new FVFieldLegend( manager );
    addChild(fl);
    fl->update();
}
