//
// C++ Interface: FVBoxMesh
//
// Description:
//
//
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVBOXMESH_H
#define FVBOXMESH_H

#include <QObject>

#include <fvobject.h>
#include <boxbox.h>
#include <fvboxmgr.h>
#include <boxcon.h>
//#include <grid.h>
#include <attrmodel.h>
#include <fvgridinterface.h>
#include <fvinteractiontranslation.h>
#include <fvinteractionrotation.h>
#include <dolfin/mesh/Mesh.h>


class FVBoxMesh : public FVObject
{
    Q_OBJECT

public:

    FVBoxMesh(FVBoxMgr * manager, dolfin::Mesh * m, int x = 0, int y = 0);
    virtual ~FVBoxMesh();

    virtual QString classType() { return QString("FVBoxMesh"); }

    virtual FVInterface * getInterface( QString interfaceName );
    template <typename T>
    void createMeshFunction(QString& fname);

public slots:
    void slotDraw();
    void slotDrawSolid();
    void slotDrawWireframe();
    void slotDrawElements();
    void slotDrawVertices();
    void slotDrawSubdomainWireframe();
    void slotLoadField();

    void slotLoadMeshFunction();
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
    dolfin::Mesh* getMesh();
protected:
    dolfin::Mesh * mesh;

//    double P[3];
//    double N[3];
//    FVInteractionTranslation * fit;
//    FVInteractionRotation * fir;



    virtual void setupAttributes();
    virtual void setupMenu();

private:
    FVGridInterface * fvGridInterface;
};

#endif // FVBOXMESH_H
