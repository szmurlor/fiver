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
#ifndef FVINTERACTIONTRANSLATION_H
#define FVINTERACTIONTRANSLATION_H

#include <fvinteractionmodel.h>
#include <QGLViewer/qglviewer.h>

class FVObject;

/**
@author Robert Szmurlo
*/
class FVInteractionTranslation : public FVInteractionModel
{
	Q_OBJECT
	
public:
        // Tworzy manipulator.
        // P - to punkt centralny manipulatora, jego pozycja
        // N - to wektor osi, wzdluz ktorej mozna przesuwac obiekt
        // Vh i Vw to wektory prostopadle do N. Potrzebne do rysowania plaszczyzny
        // do przesuwania obiektu
        // length to dlugosc osi, size to dl. boku plaszczyzny
        FVInteractionTranslation(FVObject * parent,
             double P[3], double N[3], double Vh[3], double Vw[3],
             double length, double size);
	~FVInteractionTranslation();

	virtual void paintGL();
	
        virtual bool wantMouseEvent( QMouseEvent* event ); // Czy obsluzyc zdarzenie myszki?
	virtual void mouseMoveEvent( QMouseEvent* e );
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void mouseReleaseEvent( QMouseEvent * event );
	
signals:
        /** dx,dy,dz */
	void signalTranslate( double, double, double );
	
protected:
        double P[3]; // polozenie
        double V[3]; // wektor
        double Vh[3]; // wektor prostopadle
        double Vw[3]; // wektory prostopadle

        double PStart[3]; // Poczatek przesuniecia
        double PEnd[3]; // Koniec przesuniecia

        double length; // dlugosc osi
        double size; // bok plaszczyzny

        double totalMovement[3]; // wektor o ktory przesunieto obiekt od czasu
                                // nacisniecia przycisku myszy, do puszczenia go
        int moveMode; // -1 - niewiadomo, 0 - wzdluz osi, 1 - po plaszczyznie
        // This automatically initialized by wantMouseEvent() method
        bool bMoving; // Czy przesuwamy kursor myszki?

        int selectedName(QMouseEvent * e); // Jaki element kliknieto?
	
};

#endif
