#ifndef FVBOXCOIL_H
#define FVBOXCOIL_H


/***************************************************************************
 *   Copyright (C) 2011 by jstar   *
 *   jstar@iem.pw.edu.pl   *
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
        @author jstar <jstar@iem.pw.edu.pl>
*/
class FVBoxCoil : public FVObject
{
    Q_OBJECT
public:
    FVBoxCoil(FVBoxMgr * manager,  int x = 0, int y = 0);

    ~FVBoxCoil();

    FVBoxCoil * clone();
    bool loadFromFile(QString fname);
    virtual QString classType() { return QString("FVFieldDraw"); }

    virtual void paintGL();
    virtual bool message( const QList<QString> &argv );

public slots:
    void slotClone();
    void slotSave();

protected:
    double x,y,z;
    double ax,ay,az;
    double r1,r2,l;
    bool cartesianUsed;
    bool degreesUsed;
    QColor cl;
    bool wireframe;

    virtual void setupAttributes();
    virtual void readAttributes();

    virtual void changeAttributes();

    virtual void setupMenu();

private:
    void setupSize();
    void setupCartesian();
    void setupSpherical();
    void setupAppearance();

};
#endif // FVBOXCOIL_H
