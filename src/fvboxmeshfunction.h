#ifndef FVBOXMESHFUNCTION_H
#define FVBOXMESHFUNCTION_H

#include "fvobject.h"
#include <qgl.h>
#include <QColor>

#include <dolfin/mesh/Mesh.h>
#include <dolfin/mesh/MeshFunction.h>
#include <elem.h>
#include <geom.h>
#include <math.h>
#include <field.h>
#include <fvrequiregrid.h>

class FViewer;

class FVBoxMeshFunction : public FVObject
{
    Q_OBJECT

public:
        FVBoxMeshFunction(FVBoxMgr * manager, FVObject * parent, std::string name, int x = 0, int y = 0);
        virtual ~FVBoxMeshFunction();
        virtual QString classType() { return QString("FVMeshDraw"); }

        virtual void paintGL();
    virtual void updateAttributes();

public slots:
        void slotDraw();
        /*
        virtual void slotMousePress( QMouseEvent * );
        void slotConnect();
        void slotDisconnect();
        */


protected:
        FVObject *parent;
        dolfin::Mesh * mesh;
        dolfin::MeshFunction<uint> * mf;
        std::string name;

        QMap< int, QColor > subColors;
        QMap< int, bool > subVisible;

        FVRequireGrid reqGrid;

        QColor getColor();
        QColor getColor(int isub);
        int getVertexSize();
        float getLineWidth();
        void initSubColors();
        void initSubVisibility();
        virtual void setupAttributes();
        virtual void setupMenu( );


        void paintElemsNums();
        void paintVertsNums();
        void draw3(/*QString & paintMode, double dShrink*/);
        void draw2();
        void draw2b();
        void drawSubdomainWireframe();
        uint findMax();
        uint findMin();

};

#endif // FVBOXMESHFUNCTION_H
