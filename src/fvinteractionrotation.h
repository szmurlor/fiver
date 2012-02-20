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
#ifndef FVINTERACTIONROTATION_H
#define FVINTERACTIONROTATION_H

#include <fvinteractionmodel.h>
#include <float.h>
#include <fviewer.h>

class FVObject;

/**
@author Robert Szmurlo
*/
class FVInteractionRotation : public FVInteractionModel
{
	Q_OBJECT
public:
        FVInteractionRotation(FVObject * parent, double P[3], double size, double length);
        ~FVInteractionRotation();
        virtual void paintGL();

        virtual bool wantMouseEvent( QMouseEvent* event );
        virtual void mouseMoveEvent( QMouseEvent* e );
        virtual void mousePressEvent( QMouseEvent * event );
        virtual void mouseReleaseEvent( QMouseEvent * event );

signals:
        void signalRotate( double, double, double,
                           double, double, double, double );

protected:
        double P[3]; // Center point
        double PStart[3]; // Start point of Movement
        double PEnd[3]; // End point of Movement
        // Below we have two arrays of three vectors (double[3])
        double AxisR[3][3]; // Radius expressed as a vector.
                            // It's lenght is radius value.
        double AxisN[3][3]; // Normal vector of circle -- ortoghonal to A1R

        bool bMoving;
        int axis;
        double size;
        double length;
        int mode; // 0 - rotating, 1 - move
        void drawAxis(int );
        int selectedName(QMouseEvent * e);
};

#endif
