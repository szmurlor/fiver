#ifndef FVFIELDDRAWXML_H
#define FVFIELDDRAWXML_H

#include <fvobject.h>
#include <fvboxfieldxml.h>
#include <colormap.h>
#include <isolines.h>
#include <setofint.h>
#include <fvrequirefield.h>
#include <fvrequiregrid.h>
#include <QtDebug>
#include <QString>
#include <fvhelpers.h>
#include <dolfin/mesh/Cell.h>
#include <dolfin/mesh/Facet.h>
#include <dolfin/mesh/Face.h>
#include <dolfin/mesh/Vertex.h>


class FVBoxMgr;
class Field;

class FVFieldDrawXml : public FVObject
{

        Q_OBJECT

public:
        FVFieldDrawXml(FVBoxMgr * manager, FVObject * boxField, int x = 0, int y = 0);
        ~FVFieldDrawXml();

        virtual QString classType() { return QString("FVFieldDrawXml"); }

        virtual void paintGL();

    virtual FVInterface * getInterface( QString interfaceName );
    virtual void updateAttributes();

public slots:
    void slotDrawFieldLegend();

protected:
        FVObject * boxFieldXml;

        QString drawMode;

        int isolines_no;
        double isolinesWidth;

        double minValue;
        double maxValue;
        bool logScale;

        QString visibleSub;
        int minSub, maxSub;
        ColorMap::ColorMapType colormapType;

        /** Used for vector length. */
        double modelBaseLength;
        double baseLength;
        bool sameLength;
        bool sameColor;
        QColor sameColorColor;
        double vectorsDensity;

    virtual void setupAttributes();
        virtual void readAttributes();
    virtual void setupMenu();

        bool showElem( int ie );
        bool showNode( int in );

        void drawColormap( int ie, const dolfin::Facet & facet , dolfin::Array<double> val );
        void drawColormap( uint* maps, const dolfin::Face & face , dolfin::Array<double> val );
        void drawIsolines( int ie, const dolfin::Facet & facet , dolfin::Array<double> val);
        void drawIsolines( uint* maps, const dolfin::Face & face , dolfin::Array<double> val);
        void drawVectorElem( int ie , dolfin::Array<double> val );
        void drawVectorNode( int in , dolfin::Array<double> val);

        void drawVector( double c[3], double v[3] );

private:
        dolfin::Mesh* mesh;
        dolfin::BoundaryMesh* bmesh;
        dolfin::Function* field;
        dolfin::Array<double> val;
        ColorMap cm;
        IsoLines iso;
        SetOfInt visSub;

        void fillValuesOfFace(int ie,const dolfin::Facet &facet, double e[3][3], double v[3], dolfin::Array<double> val);
        void fillValuesOfFace(uint* maps ,const dolfin::Face &face, double e[3][3], double v[3], dolfin::Array<double> val);

        void calculateMinMax( void );

        FVRequireField reqField;
        FVRequireGrid reqGrid;

        double vectorHeadLength;
        double vectorHeadThickness;
        double vectorThickness;
        int headFaces;
        bool dontMissBigV;
};

#endif // FVFIELDDRAWXML_H
