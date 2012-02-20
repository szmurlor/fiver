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
#include "fvinteractiontranslation.h"
#include <fvobject.h>
#include <GL/gl.h>
#include <geom.h>
#include <QtDebug>
#include <fviewer.h>
#include <fivermain.h>
#include <Helpers.h>

using namespace Helpers; // Tutaj znajduja sie przydatne funkcje
                         // jak np. rzutowanie punktu 2d na prosta w 3d

FVInteractionTranslation::FVInteractionTranslation(
        FVObject * parent, double P[3], double V[3], double Vh[3], double Vw[3],
        double length, double size)
 : FVInteractionModel(parent)
{
        SET(this->P,P);
        SET(this->V,V);
        SET(this->Vh,Vh);
        SET(this->Vw,Vw);
        this->length = length;
        this->size = size;
        this->moveMode = -1;
        setDrawable(true); // Chcemy rysowac manipulator
	bMoving = false;

        SET(PStart, P);
        SET(PEnd, P);
        ZERO(totalMovement);
        qDebug() << "#P(" << P[0] << "," << P[1] << "," << P[2] << ")";
}

FVInteractionTranslation::~FVInteractionTranslation()
{}

void FVInteractionTranslation::paintGL( )
{
    double X[3];
    double Y[3];
    double Z[3];

    SET(X,Vw); // Pobranie wektorow ...
    SET(Y,Vh); // .. plaszczyzny
    SET(Z,V); // .. i wektora osi

    // Jako os, musimy narysowac bardzo niewysoki, waski ale dlugi prostopadloscian
    MULT(X, X, length/1000);
    MULT(Y, Y, length/1000);
    MULT(Z, Z, length);

    qDebug() << "Blending...";
    glEnable( GL_BLEND );
    glEnable( GL_ALPHA_TEST );
    //glDisable(GL_DEPTH_TEST);
    glAlphaFunc( GL_LESS, 1.f );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.9,0.9,0.9,0.5);
    glPushName(1);
    drawCube(P, X, Y, Z);
    glPopName();

    SET(X,Vw); // Analogicznie, do tego co wyzej
    SET(Y,Vh);
    SET(Z,V);

    // Chcemy zeby plaszczyzna byla wysoka, szeroka ale bardzo cienka
    MULT(X, X, size);
    MULT(Y, Y, size);
    MULT(Z, Z, 0.001);

    glColor4f(0.0,1.0,0.0,0.4);
    glPushName(2);
    drawCube(P, X, Y, Z);
    glPopName();

    //glEnable(GL_DEPTH_TEST);
    glDisable( GL_ALPHA_TEST );
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

bool FVInteractionTranslation::wantMouseEvent( QMouseEvent * event )
{	
	int sel = selectedName(event);
        return ((sel == 1) || (sel == 2) || (bMoving)); // Jesli kliknieto os, plaszyzne, lub ruszamy juz myszka
}

void FVInteractionTranslation::mousePressEvent( QMouseEvent * event )
{
    int sel = selectedName(event);
    qDebug() << "sel = " << sel;
    double PP[3]; //Drugi z punktow na osi, rowny P + V
    double point[3]; //Punkt 3d po rzutowaniu klikniecia uzytkownika, na prosta osi lub plaszyzne
    double click[2]; //Punkt 2d, klikniety przez uzytkownika na ekranie
    ADD(PP, P, V);
    click[0] = event->x();
    click[1] = event->y();

    if ((sel == 1)) { // przesuwanie wzdluz osi
        unprojectPointOnLine(point, P, PP, click); // Rzutujemy punkt click, na os P --> PP
        SET( PStart, point); // Plik point od tej pory jest naszym punktem poczatkowym
        moveMode = 0; // Przesuwamy po osi
        bMoving = true; // Chcemy przechwytywac zdarzenia myszki, bo nie skonczylismy przesuwania
        qDebug() << "Set PStart(" << PStart[0] << "," << PStart[1] << "," << PStart[2] << ")";
    } else  if(sel == 2) { // przesuwanie po plaszczyznie
        unprojectPointOnPlane(point, click, P, V); // Rzutujemy klikniecie uzytkownika na plaszczyzne przesuwania
        SET( PStart, point );
        qDebug() << "Set PStart(" << PStart[0] << "," << PStart[1] << "," << PStart[2] << ")";
        moveMode = 1; // Przesuwamy po plaszczyznie
        bMoving = true; // to samo co wyzej
    } else {
        bMoving = false;
    }
}

void FVInteractionTranslation::mouseMoveEvent( QMouseEvent * e )
{
    if (bMoving) {
        double point[3];
        double click[2];
        double delta[3];
        double PP[3];
        ADD(PP, P, V);
        click[0] = e->x();
        click[1] = e->y();

        if(moveMode == 0) {
            unprojectPointOnLine(point, P, PP, click);
            SET(PEnd, point);
            SUB(delta, PEnd, PStart);
            SET(PStart, PEnd);
            ADD(totalMovement,totalMovement,delta);
            emit signalTranslate(delta[0], delta[1], delta[2] );

        } else if (moveMode == 1) {
            unprojectPointOnPlane(point, click, P, V);
            SET(PEnd, point);
            SUB(delta, PEnd, PStart);
            SET(PStart, PEnd);
            ADD(totalMovement,totalMovement,delta);
            emit signalTranslate(delta[0], delta[1], delta[2] );
        }
    }
}

void FVInteractionTranslation::mouseReleaseEvent( QMouseEvent * )
{
    ADD(P,P,totalMovement);
    ZERO(totalMovement); // Zerujemy calkowite przesuniecie, zaczynamy nowe
    moveMode = -1;
    bMoving = false;
    getCurrentViewer()->updateGL();
}

int FVInteractionTranslation::selectedName( QMouseEvent * e )
{
	FViewer * fv = getCurrentViewer();
	fv->select(e);
	return (fv->selectedName());
}
