/*****************************************************************************
 *   Copyright (C) 2008 Robert Szmurlo <robert@iem.pw.edu.pl>                *
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

#include "fvboxsphere.h"
#include "main.h"
#include <QDebug>
#include <QStringList>
#include <GL/glu.h>

FVBoxSphere::FVBoxSphere(FVBoxMgr * manager,  int ix, int iy)
    : FVObject(manager,ix,iy)
{
    x= y= z= 0;
    r= 1.0;
    cartesianUsed= true;
    degreesUsed= true;

    QVariant v;
    v = fvsettings.value( QString("/RSoft/FViewer/SingleColor"), QVariant(QColor(200,200,200)) );
    cl = v.value<QColor>();

    initDrawable( "BoxSphere", "Draw" );
}

FVBoxSphere::~FVBoxSphere()
{
}

FVBoxSphere * FVBoxSphere::clone() {
    FVBoxSphere *c= manager->addBoxSphere("Sphere");
    c->x = x;
    c->y = y;
    c->z = z;
    c->r = r;
    c->cl = cl;
    c->cartesianUsed = cartesianUsed;
    c->degreesUsed = degreesUsed;
    c->changeAttributes();
    c->update();
    return c;
}

void FVBoxSphere::setupCartesian()
{
    qDebug() << "setupCartesian();";
    Attr * a;
    am->clear();
    a = am->addAttr( tr("Type"), tr("Sphere"), QString("text") );
    a->setEditable(false);

    a = am->addSection( tr("Size") );
    a = am->addAttr( tr("Radius R:"), tr("%1").arg(r), QString("text") );
    a->setEditable(true);

    a = am->addSection( tr("Coordinates") );
    a = am->addAttr( tr("Cartesian/Spherical"), QString(tr("Cartesian")), "textlist" );
    QStringList lst;
    lst.append("Cartesian");
    lst.append("Spherical");
    a->setList( lst );

    a = am->addSection( tr("Cartesian coordinates") );
    a = am->addAttr( tr("Position X:"), tr("%1").arg(x), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Position Y:"), tr("%1").arg(y), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Position Z:"), tr("%1").arg(z), QString("text") );
    a->setEditable(true);

    a = am->addSection( tr("Presentation") );

    a = am->addAttr( tr("Color:"), cl, "color" );
    a->setEditable(true);

    qDebug() << "setupCartesian(); finished";
}

void FVBoxSphere::setupSpherical() {

    Attr * a;
    am->clear();
    a = am->addAttr( tr("Type"), tr("Sphere"), QString("text") );
    a->setEditable(false);

    a = am->addSection( tr("Size") );
    a = am->addAttr( tr("Radius R:"), tr("%1").arg(r), QString("text") );
    a->setEditable(true);

    a = am->addSection( tr("Coordinates") );
    a = am->addAttr( tr("Cartesian/Spherical"), QString(tr("Spherical")), "textlist" );
    QStringList lst;
    lst.append("Cartesian");
    lst.append("Spherical");
    a->setList( lst );

    double rho = sqrt( x*x + y*y + z*z );
    double phi = atan2( y, x );
    double theta = atan2( sqrt(x*x+y*y), z );
    if( degreesUsed ) {
        phi *= 180./M_PI;
        theta *= 180./M_PI;
    }
    a = am->addSection( tr("Spherical coordinates") );
    a = am->addAttr( tr("Deg/Rad"), (degreesUsed ? QString(tr("Deg")) : QString(tr("Rad"))), "textlist" );
    QStringList list;
    list.append("Deg");
    list.append("Rad");
    a->setList( list );
    a = am->addAttr( tr("Position rho:"), tr("%1").arg(rho), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Position phi:"), tr("%1").arg(phi), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Position theta:"), tr("%1").arg(theta), QString("text") );
    a->setEditable(true);

    a = am->addSection( tr("Presentation") );

    a = am->addAttr( tr("Color:"), cl, "color" );
    a->setEditable(true);
}

void FVBoxSphere::setupAttributes()
{
    setupCartesian();
    cartesianUsed = true;
}

bool FVBoxSphere::message( const QList< QString > &  args)
{
    return false;
}

void FVBoxSphere::changeAttributes() {

    if( cartesianUsed ) {
        setupCartesian();
    } else {
        setupSpherical();
    }

    am->updateAttr( tr("Color:"), cl, "color");
}

void FVBoxSphere::readAttributes()
{
    qDebug() << "readAttributes();";
    bool degsSet = degreesUsed;
    if( cartesianUsed ) {
        x = am->getAttr(tr("Position X:"))->value().toDouble();
        y = am->getAttr(tr("Position Y:"))->value().toDouble();
        z = am->getAttr(tr("Position Z:"))->value().toDouble();
    } else {
        double rho = am->getAttr(tr("Position rho:"))->value().toDouble();
        double phi = am->getAttr(tr("Position phi:"))->value().toDouble();
        double theta = am->getAttr(tr("Position theta:"))->value().toDouble();
        Attr *a = am->getAttr(tr("Deg/Rad"));
        degsSet = a != 0 && a->value() == tr("Deg");
        if( degreesUsed ) { // tu uzywamy tego co było, bo w takich jednostkach było
            phi *= M_PI / 180.;
            theta *= M_PI / 180.;
        }
        x = rho*sin(theta)*cos(phi);
        y = rho*sin(theta)*sin(phi);
        z = rho*cos(theta);
    }
    qDebug() << "readAttributes: x=" << x << ", y=" << y << ", z=" << z <<", r=" << r;
    r = am->getAttr("Radius R:")->value().toDouble();

    Attr * a = getModel()->getAttr( tr("Color:" ) );
    if (a != 0) cl = a->color();

    degreesUsed = degsSet;

    qDebug() << "readAttributes: coordinates=" << am->getAttr(tr("Cartesian/Spherical"))->value();
    cartesianUsed = am->getAttr(tr("Cartesian/Spherical"))->value() == tr("Cartesian");
    qDebug() << "readAttributes: use Deg=" << degreesUsed;
    if( cartesianUsed ) {
        setupCartesian();
    } else {
        setupSpherical();
    }
    am->update();
}

void FVBoxSphere::paintGL()
{
    readAttributes();

    qDebug() << "Color: " << cl;

    fvlist->start();

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glColor3f((GLfloat) cl.red()/255,
                       (GLfloat) cl.green()/255,
                       (GLfloat) cl.blue()/255);

    GLUquadric * quad = gluNewQuadric();

    glTranslatef(x,y,z);


    gluQuadricDrawStyle( quad, GLU_FILL );
    gluSphere( quad, r, 10, 10 );


    gluDeleteQuadric( quad );

    fvlist->end();
}

void FVBoxSphere::setupMenu()
{
    contextMenuObj->clear();

    contextMenuObj->addAction(tr("&Clone"), this, SLOT( slotClone() ) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );

}

void FVBoxSphere::slotClone()
{
    FVBoxSphere *s= clone();
    s->update();
}

