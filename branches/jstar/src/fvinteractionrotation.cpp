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

#include "fvinteractionrotation.h"
#include <fvobject.h>
#include <geom.h>
#include <math.h>
#include <QtDebug>
#include <fviewer.h>
#include <fivermain.h>
#include <Helpers.h>

using namespace Helpers;

FVInteractionRotation::FVInteractionRotation(FVObject * parent, double P[3], double size, double length)
    : FVInteractionModel(parent)
{
    SET(this->P, P);
    ZERO(AxisN[0]);
    ZERO(AxisN[1]);
    ZERO(AxisN[2]);
    ZERO(AxisR[0]);
    ZERO(AxisR[1]);
    ZERO(AxisR[2]);

    AxisN[0][0] = 1.0;
    AxisN[1][1] = 1.0;
    AxisN[2][2] = 1.0;

    AxisR[0][1] = 1.0;
    AxisR[1][2] = 1.0;
    AxisR[2][0] = 1.0;

    axis = -1;
    this->size = size;
    this->length = length;
    mode = -1;
    bMoving = false;
    setDrawable(true);
}

FVInteractionRotation::~FVInteractionRotation()
{
}


bool FVInteractionRotation::wantMouseEvent( QMouseEvent * event )
{
    int sel = selectedName(event);
    // We want to handle mouse event if user selected
    // one of our objects or it was done before and
    // mouse button is still pressed (bMoving)
    return ((sel == 1) || (sel == 2) || (sel == 3) ||
            (sel == 4) || (sel == 5) || (sel == 6) ||
            (bMoving));
}

void FVInteractionRotation::mousePressEvent( QMouseEvent * event )
{
    int sel = selectedName(event);
    qDebug() << "sel = " << sel;

    if ((sel == 1) || (sel == 2) || (sel == 3)) {
        double point[3];
        double click[2];
        click[0] = event->x();
        click[1] = event->y();
        axis = sel - 1;
        mode = 0;

        unprojectPointOnPlane(point, click, P, AxisN[axis]);
        SET( PStart, point );
        bMoving = true;
    } else if ((sel == 4) || (sel == 5) || (sel == 6)){
        double PP[3];
        double point[3];
        double click[2];
        click[0] = event->x();
        click[1] = event->y();
        axis = sel - 4;
        ADD(PP, P, AxisN[axis]);
        mode = 1;
        unprojectPointOnLine(point, P, PP, click);
        SET( PStart, point);
        bMoving = true;
    } else {
        bMoving = false;
    }
}

void FVInteractionRotation::mouseMoveEvent( QMouseEvent * e )
{
    double point[3];
    double click[2];
    click[0] = e->x();
    click[1] = e->y();

    if (bMoving) {
        if(mode == 0) {
            unprojectPointOnPlane(point, click, P, AxisN[axis]);
            SET(PEnd, point);
            getCurrentViewer()->updateGL();

            double angle = getAngle(PStart, PEnd, P, AxisN[axis]);
            //        qDebug() << "event:"
            //                 << "c(" << P[0] << "," << P[1] << "," << P[2] << "), "
            //                 << "a(" << AxisN[axis][0] << ", " << AxisN[axis][1] << ", " << AxisN[axis][2] << "), "
            //                 << "@" << angle << " -- " << axis;
            SET(PStart, point);
            emit signalRotate(P[0], P[1], P[2], AxisN[axis][0], AxisN[axis][1], AxisN[axis][2], angle);
        } else if (mode == 1) {
            double PP[3];
            ADD(PP, P, AxisN[axis]);
            unprojectPointOnLine(point, P, PP, click);
            SET(PEnd, point);
            qDebug() << "Set PEnd(" << PEnd[0] << "," << PEnd[1] << "," << PEnd[2] << ")";

            double delta[3];
            SUB(delta, PEnd, PStart);
            qDebug() << "PStart(" << PStart[0] << "," << PStart[1] << "," << PStart[2] << ")";
            qDebug() << "PEnd(" << PEnd[0] << "," << PEnd[1] << "," << PEnd[2] << ")";
            SET(PStart, PEnd);
            qDebug() << "Move(" << delta[0] << "," << delta[1] << "," << delta[2] << ")";
            ADD(P, P, delta);
            getCurrentViewer()->updateGL();
        }
    }
}


void FVInteractionRotation::mouseReleaseEvent( QMouseEvent * )
{
    axis = -1;
    mode = -1;
    bMoving = false;
    getCurrentViewer()->updateGL();
}

void FVInteractionRotation::drawAxis(int axis){
    double X[3];
    double Y[3];
    double Z[3];
    double U[3];
    double V[3];
    double W[3];

    SET(X, AxisN[0]);
    SET(Y, AxisN[1]);
    SET(Z, AxisN[2]);
    SET(U, AxisN[0]);
    SET(V, AxisN[1]);
    SET(W, AxisN[2]);
    MULT(X, X, size);
    MULT(Y, Y, size);
    MULT(Z, Z, size);
    MULT(U, U, 0.002);
    MULT(V, V, 0.002);
    MULT(W, W, 0.002);
    if(axis == 0) {
        glColor4f(0.9,0.0,0.0,0.4);
        MULT(X, X, 0.001);
        MULT(U, U, 500*length);
    } else if (axis == 1) {
        glColor4f(0.0,0.9,0.0,0.4);
        MULT(Y, Y, 0.001);
        MULT(V, V, 500*length);
    } else if (axis == 2) {
        glColor4f(0.0,0.0,0.9,0.4);
        MULT(Z, Z, 0.001);
        MULT(W, W, 500*length);
    }

    //qDebug() << "Blending...";
    glEnable( GL_BLEND );
    glEnable( GL_ALPHA_TEST );
    //glDisable(GL_DEPTH_TEST);
    glAlphaFunc( GL_LESS, 1.f );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushName(axis+1);
    drawCube(P,X, Y, Z);
    glPopName();
    glPushName(axis+4);
    drawCube(P,U, V, W);
    glPopName();

    //glEnable(GL_DEPTH_TEST);
    glDisable( GL_ALPHA_TEST );
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void FVInteractionRotation::paintGL()
{
    if(axis == -1 || axis == 0)
        drawAxis(0);
    if(axis == -1 || axis == 1)
        drawAxis(1);
    if(axis == -1 || axis == 2)
        drawAxis(2);
}

int FVInteractionRotation::selectedName( QMouseEvent * e )
{
    FViewer * fv = getCurrentViewer();
    fv->select(e);
    return (fv->selectedName());
}
