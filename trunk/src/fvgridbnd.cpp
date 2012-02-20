//
// C++ Implementation: fvgridbnd
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fvgridbnd.h"
#include <fivermain.h>
#include <main.h>
#include <setofint.h>
#include <QtDebug>
#include <time.h>

#include <GL/glu.h>

FVGridBnd::FVGridBnd(FVBoxMgr * manager, FVObject * parent, int x, int y)
 : FVObject(manager,x,y)
{
	grid = reqGrid.getGrid( parent );
	
	if (grid != 0) {
		minBnd = 1;
		maxBnd = grid->_bnds.size();
	} else {
		qWarning() << "FVGridBnd::constructor Unable to get grid from parent: " << parent->classType();
	}
	
	initDrawable( "GridBnd", "Draw" );
	cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_DRAW ).value<QColor>();
}


FVGridBnd::~FVGridBnd()
{
}

void FVGridBnd::setupAttributes( )
{
	
	Attr * a;
	a = am->addAttr( tr("View Style"), QString("Nodes"), "textlist" );
	QStringList lst;
	lst.append("Nodes");
	lst.append("Faces");
	lst.append("Frames");
	lst.append("Numbers");
	a->setList( lst );
	
	a = am->addAttr( tr("Visible Boundaries"), QString("%1-%2").arg(minBnd).arg(maxBnd), "text" );
	
	a = am->addAttr( tr("Size Coefficient"), QString("1.0"), "text" );

        a = am->addSection( tr("Colors") );
        a = am->addAttr( tr("Sub.Color Policy"), QString("Single Color"), "textlist" );
        lst.clear();
        lst.append("Single Color");
        lst.append("Individual Colors");
        a->setList( lst );

        QVariant v;
        QColor defColor;
        v = fvsettings.value( QString("/RSoft/FViewer/SingleColor"), QVariant(QColor(200,200,200)) );
        defColor = v.value<QColor>();
        a = am->addAttr( tr("Single Color"), defColor, "color" );

        if (grid != 0) {

            for (unsigned int i = 0; i < grid->_bnds.size(); i++) {
                v = fvsettings.value( QString("/RSoft/FViewer/Color%1").arg(i+1), QVariant(QColor(100,100,100)) );
                defColor = v.value<QColor>();
                a = am->addAttr( tr("Boundary %1").arg(i+1), defColor, "color" );
            }
        }
}

QColor FVGridBnd::getColor( int ibnd )
{
        Attr * a;
        if ( getAttrValue( tr("Sub.Color Policy") ) == "Single Color" ) {
                a = getAttr( tr("Single Color") );
                if (a != 0)
                        return a->color();
        } else {
                a = getAttr( tr("Boundary %1").arg( ibnd ) );
                if (a != 0)
                        return a->color();
        }

        return QColor(200,200,200);
}

void FVGridBnd::initSubColors( )
{
    if (grid != 0) {
        subColors.clear();
        for( int i = 1; i <= (int) grid->_bnds.size(); i++ )
            subColors[ i ] = getColor( i );
    }
}

bool FVGridBnd::isFaceOnBnd( CFace &f, int xb ) 
{
	for (int k = 0; k < f.size(); k++) {
		Node *n = &(grid->_nodes[ f.nodes[k] - 1 ]);
		bool found= false;
		for (unsigned int b = 0; b < n->_bnds.size(); b++)
			if( n->_bnds[b] == xb ) {
				found= true;
				break;
			}
		if( !found )
			return false;
	}
	return true;
}

void FVGridBnd::paintGL( )
{
	grid = reqGrid.getGrid( parentObject() );
	if (grid == 0) return;
	
	readAttributes();
	
	Node * nd;
	bool bDraw;
    
	fvlist->start();
    
	GLfloat tx,ty,tz,td = 0;
    
	if (isFirstShow()) {
		double pmin[3], pmax[3];
		grid->getBBox( pmin, pmax );
		tx = (pmax[0] - pmin[0]) / 2;
		ty = (pmax[1] - pmin[1]) / 2;
		tz = (pmax[2] - pmin[2]) / 2;
		td = sqrt(tx*tx + ty*ty + tz*tz);
                if ((td > 1e-10 ) && (td < 1e10)) {
                    getCurrentViewer()->setSceneRadius( td );
                    sphereBoundaryRadius = td;
                }
	}
 
	SetOfInt visBnd( getAttrValue("Visible Boundaries"), minBnd, maxBnd);
	QString viewStyle = getAttrValue("View Style");
	time_t t= time(NULL);

        // inicjalizacja kolorow
        initSubColors();

	if( viewStyle == "Nodes" ) { // show nodes 
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            drawNodes(sphereBoundaryRadius);
	} else if( viewStyle == "Faces" || viewStyle == "Frames" ) { // show faces

                if( viewStyle == "Faces" ) {
                        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                        glBegin( GL_TRIANGLES );
                } else {
                        glDisable(GL_LIGHTING);
                        glBegin( GL_LINES );
		}
		
		CFace f;
		Elem * e;
		Node * n;
		int ie;
		double nr[3];
		float nf[3];
		QColor cl = color;
                //glColor3f((GLfloat) cl.red()/255,(GLfloat) cl.green()/255,(GLfloat) cl.blue()/255);
		for ( int i = 0; i < (int) grid->_elems.size(); i++) {
			e = grid->_elems[ i ];
			for( int j= 0; j < e->getFacesCount(); j++ ) {
				bDraw=false;
				f = e->getFace( j );

				for( int b= minBnd; b <= maxBnd; b++ ) {
					if (visBnd.find( b )) {
						if( isFaceOnBnd( f, b ) ) {
							bDraw= true;
                                                        cl = subColors[ b ];
                                                        glColor3f((GLfloat) cl.red()/255,
                                                                           (GLfloat) cl.green()/255,
                                                                           (GLfloat) cl.blue()/255);
							break;
						}
					}
				}

				if ( bDraw && (ie = e->getNeighbourWithFaceNr(j)) != 0
					&& (grid->_elems[ ie - 1 ]->subdomain() == e->subdomain()) ) {
					bDraw = false;
				}

				if (bDraw) {
					e->getNormalOfFace( j, nr );
					SET(nf,nr);
					VERSOR(nf,nf);
					glNormal3f( nf[0], nf[1], nf[2] );
					for (int k = 0; k < 3; k++) {
						n = &(grid->_nodes[ f.nodes[k] - 1 ]);
						glVertex3f(n->x(), n->y(), n->z() );
						if (viewStyle=="Frames") { 
							if (k<2) {
								n = &(grid->_nodes[ f.nodes[k+1] - 1 ]);
							} else {
								n = &(grid->_nodes[ f.nodes[0] - 1 ]);
							}
							glVertex3f(n->x(), n->y(), n->z() );
						}
					}
				}
			}
		} 
	} else 	if( viewStyle == "Numbers" ) { // show numbers
		for (int in = 0; in < grid->getNoNodes();  in++) {
			int thisBN= 0;
			nd = &grid->_nodes[ in ];
			bDraw = false;
			for (int b = 0; b < (int) nd->_bnds.size(); b++)
				if( visBnd.find( nd->_bnds[b] ) ) {
					thisBN= nd->_bnds[b];
					bDraw = true;
					break;
				}
		
			if (bDraw) {
				getCurrentViewer()->fvRenderText(nd->_Coords[0], nd->_Coords[1], nd->_Coords[2], QString::number( thisBN ) );
			}	
		}
	}
	if( viewStyle == "Faces" || viewStyle == "Frames" ) 
		glEnd();
	
	glEnable(GL_LIGHTING);
	fvlist->end();
	t= time(NULL);
}

void FVGridBnd::drawNodes(double td )
{
    Node * nd;
    bool bDraw;
    double sizeCoef = getAttr("Size Coefficient")->toDouble();

    SetOfInt visBnd( getAttrValue("Visible Boundaries"), minBnd, maxBnd);

    GLUquadric * quad = gluNewQuadric();

    QColor cl = Qt::blue;
    for (int in = 0; in < grid->getNoNodes();  in++) {


        nd = &grid->_nodes[ in ];
        bDraw = false;
        for (int b = 0; b < (int) nd->_bnds.size(); b++) {
            if( visBnd.find( nd->_bnds[b] ) ) {
                bDraw = true;
                cl = subColors[ nd->_bnds[b] ];
                break;
            }
        }

        if (bDraw) {
            glPushMatrix();
            glTranslatef( nd->_Coords[0], nd->_Coords[1], nd->_Coords[2] );
            glColor3f((GLfloat) cl.red()/255, (GLfloat) cl.green()/255, (GLfloat) cl.blue()/255);
            gluSphere( quad, sizeCoef* td / 50, 5 , 5);
            glPopMatrix();

        }

    }
    gluDeleteQuadric( quad );

}

void FVGridBnd::slotApply( )
{
	update();
}

void FVGridBnd::readAttributes( )
{
	Attr * a;
	a = getAttr( "Color" );
	if (a != 0) 
		color = a->color();
}

void FVGridBnd::setupMenu( )
{

}
