#ifndef FVBOXSPHERE_H
#define FVBOXSPHERE_H

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
#include <fvboxgrid.h>
#include <fvboxmgr.h>
#include <grid.h>
#include <simplefield.h>
#include <field.h>
#include "fvfieldinterface.h"

#include <QString>

/**
        @author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVBoxSphere : public FVObject
{
    Q_OBJECT
public:
    FVBoxSphere(FVBoxMgr * manager,  int x = 0, int y = 0);

    ~FVBoxSphere();

    FVBoxSphere * clone();

    virtual QString classType() { return QString("FVFieldDraw"); }

    virtual void paintGL();
    virtual bool message( const QList<QString> &argv );

public slots:
    void slotClone();

protected:
    double x,y,z;
    bool cartesianUsed;
    bool degreesUsed;
    double r;
    QColor cl;

    virtual void setupAttributes();
    virtual void readAttributes();

    virtual void changeAttributes();

    virtual void setupMenu();

private:
    void setupCartesian();
    void setupSpherical();

};

#endif // FVBOXSPHERE_H
