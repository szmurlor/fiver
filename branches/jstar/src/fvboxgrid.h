//
// C++ Interface: fvboxgrid
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVBOXGRID_H
#define FVBOXGRID_H

#include <QObject>

#include <fvobject.h>
#include <boxbox.h>
#include <fvboxmgr.h>
#include <boxcon.h>
#include <grid.h>
#include <attrmodel.h>
#include <fvgridinterface.h>
#include <fvinteractiontranslation.h>
#include <fvinteractionrotation.h>

/**
@author Robert Szmurlo
*/
class FVBoxGrid : public FVObject
{
    Q_OBJECT
    
public:
    
    FVBoxGrid(FVBoxMgr * manager, Grid * g, int x = 0, int y = 0);
    virtual ~FVBoxGrid();

    virtual QString classType() { return QString("FVBoxGrid"); }

    virtual FVInterface * getInterface( QString interfaceName );

public slots:
    void slotDraw();
    void slotDrawSolid();
    void slotDrawWireframe();
    void slotDrawElements();
    void slotDrawVertices();
    void slotDrawSubdomainWireframe();
    void slotLoadField();
    void slotNewHarmonicField();

    void slotDrawBoundaries();
    void slotDrawBoundingBox();
    void slotSave();

    void slotGridToSTL();

    void slotRotateManipulator();
    void slotTranslateManipulator();
    void slotRemoveManipulator();
    void slotTranslate( double, double, double);
    void slotRotate( double, double, double,
                     double, double, double, double );
    Grid* getGrid();
protected:
    Grid * grid;

    double P[3];
    double N[3];
    FVInteractionTranslation * fit;
    FVInteractionRotation * fir;



    virtual void setupAttributes();
    virtual void setupMenu();

private:
    FVGridInterface * fvGridInterface;
};

#endif
