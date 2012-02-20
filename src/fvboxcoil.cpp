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
#include "fvboxcoil.h"
#include "main.h"
#include <fstream>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <GL/glu.h>

FVBoxCoil::FVBoxCoil(FVBoxMgr * manager,  int ix, int iy)
    : FVObject(manager,ix,iy)
{
    x= y= z= 0;
    ax= 0; ay= 0; az= 1;
    r1= 1.0; r2= 2.0;
    l= 1.0;
    cartesianUsed= true;
    degreesUsed= true;

    QVariant v;
    v = fvsettings.value( QString("/RSoft/FViewer/SingleColor"), QVariant(QColor(200,200,200)) );
    cl = v.value<QColor>();
    wireframe = false;

    initDrawable( "BoxCoil", "Draw" );
}

FVBoxCoil::~FVBoxCoil()
{
}

FVBoxCoil * FVBoxCoil::clone() {
    FVBoxCoil *c= manager->addBoxCoil("Coil");
    c->x = x;
    c->y = y;
    c->z = z;
    c->ax = ax;
    c->ay = ay;
    c->az = az;
    c->r1 = r1;
    c->r2 = r2;
    c->l = l;
    c->cl = cl;
    c->cartesianUsed = cartesianUsed;
    c->degreesUsed = degreesUsed;
    c->changeAttributes();
    c->update();
    return c;
}

bool FVBoxCoil::loadFromFile(QString fname)
{
        std::ifstream file( fname.toAscii().data() );

        qDebug() << "Reading " << fname;

        file >> x >> y >> z;
        file.ignore(256,'\n');
        file >> ax >> ay >> az;
        file.ignore(256,'\n');
        file >> r1;
        file.ignore(256,'\n');
        file >> r2;
        file.ignore(256,'\n');
        file >> l;

        qDebug() << "R1=" << r1 << ", R2=" << r2 << ", L=" << l;

        file.close();
        changeAttributes();
        update();
        return true;
}

void FVBoxCoil::setupSize() {
    Attr * a;
    am->clear();
    a = am->addAttr( tr("Type"), tr("Coil"), QString("text") );
    a->setEditable(false);

    a = am->addSection( tr("Size") );
    a = am->addAttr( tr("Inner Radius R1:"), tr("%1").arg(r1), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Outer Radius R2:"), tr("%1").arg(r2), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Length L:"), tr("%1").arg(l), QString("text") );
    a->setEditable(true);

    a = am->addSection( tr("Orientation (Axis)") );
    a = am->addAttr( tr("Axis X:"), tr("%1").arg(ax), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Axis Y:"), tr("%1").arg(ay), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Axis Z:"), tr("%1").arg(az), QString("text") );
    a->setEditable(true);

}

void FVBoxCoil::setupAppearance() {
    Attr * a;
    a = am->addSection( tr("Presentation") );

    a = am->addAttr( tr("Color:"), cl, "color" );
    a->setEditable(true);

    qDebug() << "Adding F/W";
    a = am->addAttr( tr("Full/Wireframe"), (wireframe ? QString(tr("Wireframe")) : QString(tr("Full"))), "textlist" );
    QStringList lst;
    lst.append("Full");
    lst.append("Wireframe");
    a->setList( lst );
    qDebug() << "F/W added";
}

void FVBoxCoil::setupCartesian()
{
    qDebug() << "setupCartesian();";
    setupSize();

    Attr * a;
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

    setupAppearance();

    qDebug() << "setupCartesian(); finished";
}

void FVBoxCoil::setupSpherical()
{
    setupSize();

    Attr * a;

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

    setupAppearance();
}

void FVBoxCoil::setupAttributes()
{
    setupCartesian();
    cartesianUsed = true;
}

bool FVBoxCoil::message( const QList< QString > &  args)
{
    return false;
}

void FVBoxCoil::changeAttributes() {

    if( cartesianUsed ) {
        setupCartesian();
    } else {
        setupSpherical();
    }

    setupAppearance();
}

void FVBoxCoil::readAttributes()
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
    qDebug() << "readAttributes: x=" << x << ", y=" << y << ", z=" << z <<", r1=" << r1 <<", r2=" << r2<<", l=" << l;
    r1 = am->getAttr("Inner Radius R1:")->value().toDouble();
    r2 = am->getAttr("Outer Radius R2:")->value().toDouble();
    l = am->getAttr("Length L:")->value().toDouble();
    ax = am->getAttr("Axis X:")->value().toDouble();
    ay = am->getAttr("Axis Y:")->value().toDouble();
    az = am->getAttr("Axis Z:")->value().toDouble();

    Attr * a = getModel()->getAttr( tr("Color:" ) );
    if (a != 0) cl = a->color();
    a = am->getAttr(tr("Full/Wireframe"));
    wireframe = a->value() == tr("Wireframe");

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

void FVBoxCoil::paintGL()
{
    readAttributes();

    qDebug() << "Color: " << cl;

    fvlist->start();

    if( wireframe )
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


    glColor3f((GLfloat) cl.red()/255,
                       (GLfloat) cl.green()/255,
                       (GLfloat) cl.blue()/255);

    GLUquadric * quad = gluNewQuadric();

    glTranslatef(x,y,z);

    double xy= sqrt(ax*ax+ay*ay);
    if( xy > 1e-9 ) {
        qDebug() << "xy=" << xy << " fi=" << atan2(ay,ax)/M_PI*180 << " teta="<<atan2(xy,az)/M_PI*180;
        glRotatef(atan2(ay,ax)/M_PI*180,0,0,1);
        glRotatef(atan2(xy,az)/M_PI*180,0,1,0);
    }

    gluQuadricDrawStyle( quad, GLU_FILL );
    gluCylinder( quad, r2, r2, l, 32, 1 );
    gluCylinder( quad, r1, r1, l, 32, 1 );
    glTranslatef(0,0,l);
    gluDisk( quad, r1, r2, 32, 1 );
    glRotatef(180,0,1,0);
    glTranslatef(0,0,l);
    gluDisk( quad, r1, r2, 32, 1 );


    gluDeleteQuadric( quad );

    fvlist->end();
}

void FVBoxCoil::setupMenu()
{
    contextMenuObj->clear();

    contextMenuObj->addAction(tr("&Clone"), this, SLOT( slotClone() ) );

    contextMenuObj->addSeparator();

    contextMenuObj->addAction(tr("&Save"), this, SLOT(slotSave()));

    contextMenuObj->addSeparator();

    contextMenuObj->addAction(tr("&Delete"),(QWidget*) manager, SLOT(slotDelete()) );

}

void FVBoxCoil::slotClone()
{
    FVBoxCoil *s= clone();
    s->update();
}

void FVBoxCoil::slotSave()
{
    qDebug() << "FVBoxCoil::slotSave";

    QString startDir (fvsettings.value("/RSoft/FViewer/RecentDir","~").toString());
    QFileDialog sd(manager);
    sd.setDirectory( startDir );
    sd.setAcceptMode(QFileDialog::AcceptSave);
    QString fname= "";
    if( sd.exec() ) {
        fname = sd.selectedFiles().at(0);
    }
    // sprawdzic rozszerzenie i dokleic .cl
    if ( ! fname.contains(".") || fname.section(".", -1) != "cl" ) {
        fname.append(".cl");
    }
    if (fname.length() > 0) {
        std::ofstream file( fname.toAscii().data() );

        qDebug() << "Writing to " << fname;

        file << x << ' ' << y << ' ' << z << " # position" <<endl;
        file << ax << ' ' << ay << ' ' << az << " # axis" <<endl;
        file << r1 << " # r1" << endl;
        file << r2 << " # r2" << endl;
        file << l << " # l" << endl;

        qDebug() << "Saved R1=" << r1 << ", R2=" << r2 << ", L=" << l;

        file.close();

        qDebug() << "Saving: " << fname.section('/',0,-2);
        fvsettings.setValue("/RSoft/FViewer/RecentDir",fname.section('/',0,-2));
        fvsettings.sync();
    } else {
        QMessageBox::warning(manager,tr("Coil save warning."), tr("Coil NOT saved."));
    }
}

