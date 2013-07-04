//
// C++ Implementation: fviewer
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "fviewer.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QtDebug>
#include <QPainter>
#include <main.h>

using namespace qglviewer;

FViewer::FViewer()
        :QGLViewer()
{
    init();
}

FViewer::FViewer(QWidget * parent)
:QGLViewer(parent)
{
    init();
}

void FViewer::init()
{
    glfont = 0;
    fvinteraction = 0;
    bFirstInit = true;
    setAutoFillBackground(false);

    float diffuseMaterial[] =
      { 0.1f, 0.1f, 0.1f, 1.0f };
    float mat_specular[] =
    { 0.0f, 0.0f, 0.0f, 1.0f };


    //glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialf(GL_FRONT, GL_SHININESS, 5.0f);

    //glPolygonOffset(1.0, 2);
    //glEnable( GL_POLYGON_OFFSET_FILL );
}

FViewer::FViewer( const FViewer & fv )        
{
    this->lists = fv.lists;
    this->glfont = fv.glfont;
    this->fvinteraction = fv.fvinteraction;
    this->bFirstInit = fv.bFirstInit;
}

FViewer::~FViewer()
{
}

QString FViewer::helpString() const
{
	QString help("<h2>FIVER 2.0</h2>");
	help += "<p>Finite element graphical postprocessor.</p>";
	help += "<em> Authors: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005, Jacek Starzynski <jstar@iem.pw.edu.pl></em>";

	return help;
}


void FViewer::minit()
{
	
	if (bFirstInit) {
		bFirstInit = false;
		if (!restoreStateFromFile())
			showEntireScene();
	}
	
}

void FViewer::drawWithNames( )
{
	if (fvinteraction != 0) {
		if (fvinteraction->isDrawable()) {
			fvinteraction->paintGL();
		}
	}
}

/*!
    \fn FViewer::draw()
 */
void FViewer::draw()
{
	
    QColor c = QColor::fromRgb(200,200,200,0);
    if ( fvsettings.contains(QString("/RSoft/FViewer/ViewerBackground")) ) {
        QVariant cVar = fvsettings.value( QString("/RSoft/FViewer/ViewerBackground") , QString("(255,255,255)" ) );
        c = cVar.value<QColor>();
    }
    QGLViewer::setBackgroundColor(c);

    std::list < FVList * >::iterator it;
    for (it = lists.begin(); it != lists.end(); it++) {
            if ((*it)->bVisible) {
                    glPushMatrix();
                    (*it)->call();
                    glPopMatrix();
            }
    }

    fvRenderText(10,10, "FIVER 2.0 (ZETiIS, Warsaw Univ. of Techn.)");
    fvDrawLabels();
    fvDrawAxis();

    if (fvinteraction != 0) {
            if (fvinteraction->isDrawable()) {
                    fvinteraction->paintGL();
            }
    }
}

void FViewer::add(FVList * fl)
{
    lists.push_back(fl);
}

void FViewer::remove(QString & name)
{
    std::list < FVList * >::iterator it;

    for (it = lists.begin(); it != lists.end(); it++) {
	if ((*it)->name == name)
	    lists.erase(it);
	return;
    }
}

FVList *FViewer::find(QString & name)
{
	std::list < FVList * >::iterator it;

	for (it = lists.begin(); it != lists.end(); it++)
		if ((*it)->name == name)
			return *it;

	return 0;
}

void FViewer::remove(FVList * what)
{
	lists.remove(what);
}

void FViewer::fvRenderText( int x, int y, QString str )
{
    glDisable(GL_LIGHTING);
    glColor4f(0.0,0.0,0.0,1.0);
    renderText(x,y, str);
    glEnable(GL_LIGHTING);
}

void FViewer::fvRenderText( float x, float y, float z, QString str )
{
        glDisable(GL_LIGHTING);
	glColor4f(0.0,0.0,0.0,1.0);
        renderText(x,y,z, str);
	glEnable(GL_LIGHTING);
}

void FViewer::setInteractionModel( FVInteractionModel * i )
{
	fvinteraction = i;
}

void FViewer::mousePressEvent( QMouseEvent * e )
{
	if ( fvinteraction != 0 )
		if ( fvinteraction->wantMouseEvent( e ) ) {
			fvinteraction->mousePressEvent( e );
		}
	QGLViewer::mousePressEvent( e );
}

void FViewer::mouseMoveEvent( QMouseEvent * e )
{
	if ( fvinteraction != 0 )
		if ( fvinteraction->wantMouseEvent( e ) ) {
			fvinteraction->mouseMoveEvent( e );
			return;
		}
		
	QGLViewer::mouseMoveEvent( e );
}

void FViewer::mouseReleaseEvent( QMouseEvent * e )
{
	if ( fvinteraction != 0 )
		if ( fvinteraction->wantMouseEvent( e ) ) {
			fvinteraction->mouseReleaseEvent( e );
			return;
		}
		
	QGLViewer::mouseReleaseEvent( e );
}

void FViewer::keyPressEvent( QKeyEvent * e )
{
/*
	if ( fvinteraction != 0 )
		if ( fvinteraction->wantPressEvent( e ) ) {
			fvinteraction->keyPressEvent( e );
			return;
		}
	if (e->key() != Qt::Key_Escape)
*/
	QGLViewer::keyPressEvent( e );
}

void FViewer::fvDrawAxis( )
{
	qglviewer::Vec p, p0, px,py,pz, dpx, dpy, dpz,  dp;
	float lmax = 0, fscale;

        glColor4f(0.0f,0.0f,0.0f,1.0f);
        glLineWidth(1.0);

	startScreenCoordinatesSystem();

	// get the 0,0,0 in screen coords, for reference
	p0 = camera()->projectedCoordinatesOf(Vec(0,0,0));
	dp = p0 - Vec(40, height()-40,0);
	dp[2] = -0.1;

	// calculate the new translated coordinates.
	p0 = p0 - dp;
	px = camera()->projectedCoordinatesOf(Vec(0.001,0,0)) - dp;
	py = camera()->projectedCoordinatesOf(Vec(0,0.001,0)) - dp;
	pz = camera()->projectedCoordinatesOf(Vec(0,0,0.001)) - dp;
	
	// determine the scale to make the axis constant in size.
	dpx = px - p0;
	if (dpx.norm() > lmax) 
		lmax = dpx.norm();

	dpy = py - p0;
	if (dpy.norm() > lmax) 
		lmax = dpy.norm();

	dpz = pz - p0;
	if (dpz.norm() > lmax) 
		lmax = dpz.norm();

	fscale = 35 / lmax;
	px = p0 + dpx * fscale;
	py = p0 + dpy * fscale;
	pz = p0 + dpz * fscale;

	// Make visible for OpenGL default viewport (z in (0,-1))
	px[2] = -0.1;
	py[2] = -0.1;
	pz[2] = -0.1;

	// draw axes.
	glBegin(GL_LINES);
	glVertex3fv(p0);
	glVertex3fv(px);
	glVertex3fv(p0);
	glVertex3fv(py);
	glVertex3fv(p0);
	glVertex3fv(pz);
	glEnd();

        fvRenderText((int) px[0], (int) px[1], QString("X"));
        fvRenderText((int) py[0], (int) py[1], QString("Y"));
        fvRenderText((int) pz[0], (int) pz[1], QString("Z"));
	
	stopScreenCoordinatesSystem();
}

void FViewer::repaint()
{
    makeCurrent();
    paintGL();
    updateGL();
}

void FViewer::rotx( double ang )
{
    qglviewer::Camera *c = camera();
    qglviewer::Vec vec = c->rightVector();
    qglviewer::Vec pos = c->position();

    qglviewer::Vec oy = c->upVector();
    qglviewer::Vec ox = c->rightVector();
    qglviewer::Vec oz = c->viewDirection();

    pos = rotate (pos.x, pos.y, pos.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);

//  ox = rotate (ox.x, ox.y, ox.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);
    oy = rotate (oy.x, oy.y, oy.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);
    oz = rotate (oz.x, oz.y, oz.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);

    c->setUpVector(oy);
    c->setViewDirection(oz);

    c->setPosition(pos);

    update();
}

void FViewer::roty( double ang )
{
    qglviewer::Camera *c = camera();
    qglviewer::Vec vec = c->upVector();
    qglviewer::Vec pos = c->position();

    qglviewer::Vec oy = c->upVector();
    qglviewer::Vec ox = c->rightVector();
    qglviewer::Vec oz = c->viewDirection();

    pos = rotate (pos.x, pos.y, pos.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);

    ox = rotate (ox.x, ox.y, ox.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);
//  oy = rotate (oy.x, oy.y, oy.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);
    oz = rotate (oz.x, oz.y, oz.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);

//  c->setUpVector(oy);
    c->setViewDirection(oz);

    c->setPosition(pos);

    update();
}

void FViewer::rotz( double ang )
{
    qglviewer::Camera *c = camera();
    qglviewer::Vec vec = c->viewDirection();
    qglviewer::Vec pos = c->position();

    qglviewer::Vec oy = c->upVector();
    qglviewer::Vec ox = c->rightVector();
    qglviewer::Vec oz = c->viewDirection();

    pos = rotate (pos.x, pos.y, pos.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);

    ox = rotate (ox.x, ox.y, ox.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);
    oy = rotate (oy.x, oy.y, oy.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);
//  oz = rotate (oz.x, oz.y, oz.z, vec.x, vec.y, vec.z, - ang / 180 * M_PI);

    c->setUpVector(oy);
//  c->setViewDirection(oz);

    c->setPosition(pos);

    update();
}

qglviewer::Vec FViewer::rotate( float x, float y, float z, float u, float v, float w, float a)
{
        float ux,uy,uz,vx,vy,vz,sa,ca,wx,wy,wz;

        ux=u*x;
        uy=u*y;
        uz=u*z;
        vx=v*x;
        vy=v*y;
        vz=v*z;
        wx=w*x;
        wy=w*y;
        wz=w*z;

        sa=sin(a);
        ca=cos(a);

        qglviewer::Vec res;

        res.x = u*(ux+vy+wz)+(x*(v*v+w*w)-u*(vy+wz))*ca+(-wy+vz)*sa;
        res.y = v*(ux+vy+wz)+(y*(u*u+w*w)-v*(ux+wz))*ca+(wx-uz)*sa;
        res.z = w*(ux+vy+wz)+(z*(u*u+v*v)-w*(ux+vy))*ca+(-vx+uy)*sa;

        return res;
}

void FViewer::snapshot(QString filename)
{
    saveSnapshot(filename);
}

void FViewer::fvDrawLabels()
{
    std::list < Label * >::iterator it;

    for (it = labels.begin(); it != labels.end(); it++) {
        Label *l = (*it);
        fvRenderText(l->x, l->y, l->text);
    }
}

void FViewer::addLabel(int x, int y, QString text)
{
    Label *l = new Label;
    l->x = x;
    l->y = y;
    l->text = text;
    labels.push_back(l);
}

// Moves camera over given vector [x, y, z]
void FViewer::trans(double x, double y, double z)
{
    qglviewer::Camera *c = camera();
    qglviewer::Vec pos = c->position();

    pos.x = pos.x + x;
    pos.y = pos.y + y;
    pos.z = pos.z + z;

    c->setPosition(pos);

    update();
}

