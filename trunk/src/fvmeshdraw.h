//
// C++ Interface: fvgrid
//
// Description:
//
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVMESH_H
#define FVMESH_H

#include "fvobject.h"
#include <qgl.h>
#include <QColor>

#include <dolfin/mesh/Mesh.h>
#include <elem.h>
#include <geom.h>
#include <math.h>
#include <field.h>
#include <fvrequiregrid.h>

class FViewer;

class FVMeshDraw : public FVObject
{
    Q_OBJECT

public:
        FVMeshDraw(FVBoxMgr * manager, FVObject * parent, int x = 0, int y = 0);
        virtual ~FVMeshDraw();
        virtual QString classType() { return QString("FVMeshDraw"); }

        virtual void paintGL();
    virtual void updateAttributes();

public slots:
        /*
        virtual void slotMousePress( QMouseEvent * );
        void slotConnect();
        void slotDisconnect();
        */


protected:
        FVObject *parent;
        dolfin::Mesh * mesh;
        QMap< int, QColor > subColors;
        QMap< int, bool > subVisible;

        FVRequireGrid reqGrid;

        QColor getColor();
        int getVertexSize();
        float getLineWidth();
        void initSubColors();
        void initSubVisibility();
        virtual void setupAttributes();
        virtual void setupMenu( );

        void paintElemsNums();
        void paintVertsNums();
        void drawNormal(QString & paintMode, double dShrink);
        void drawVertices();
        void drawSubdomainWireframe();

        void getBBox(dolfin::Mesh* m, double minP[3], double maxP[3]);
        void getCenter( dolfin::Mesh* m, double P[3] );


};

#endif

