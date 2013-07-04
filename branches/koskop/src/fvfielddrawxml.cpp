#include "fvfielddrawxml.h"

#include <math.h>

#include <geom.h>
//#include <field.h>
//#include <grid.h>
#include <elem.h>
#include <elemlist.h>

#include <fvboxmgr.h>
#include <fivermain.h>
#include <fvfieldinterface.h>
#include <fvgridinterface.h>
#include <fvfieldlegend.h>
#include <main.h>

#include <GL/glu.h>

FVFieldDrawXml::FVFieldDrawXml(FVBoxMgr * manager, FVObject * boxField, int x, int y)
 : FVObject(manager,x,y)
{
        this->boxFieldXml = boxField;


        minSub = 1;
        mesh = reqGrid.getMesh(boxField);
        if (mesh != 0){
            field = reqField.getFunction(boxField);
            if (field != 0){
//                dolfin::Array<double> a;
//                dolfin::Array<double> val;
                time_t tstart, tend;
                tstart = time(0);
                qDebug() << "Field values calculations started..." << endl;
                field->compute_vertex_values(val,*mesh);
                tend = time(0);
                qDebug() << "Values calculations took" << tend- tstart << " second(s)." << endl;

//                f->compute_vertex_values(a,*m);

                minValue= val.min();
                maxValue= val.max();
                maxSub = 1;
            }
        } else {
            minValue = 0;
            maxValue = 9999999;
            maxSub = 1;
        }

        modelBaseLength = 0;
        baseLength = 0;

        initDrawable( "DrawField", "Draw" );
}

void FVFieldDrawXml::updateAttributes( )
{
    // jstar: trochę dziwnie to tu wygląda - ale po nacisnieciu Apply
    // trzeba sprawdzic, czy nie zmienila sie lista rysowanych podobszarow
    // i w razie zmiany poprawić CustomMin i CustomMax
    // ale tylko wtedy, gdy użytkownik ich nie ruszal
    if (getAttrValue( tr("Use Custom Min/Max:") ) != "Yes") {
        visibleSub = getAttrValue( tr("Visible Subdomains") ); // pozniej dodac sprawdzenie
        visSub.init( visibleSub, minSub, maxSub );
        calculateMinMax();
        am->updateAttr( tr("Custom Min:"), tr("%1").arg(minValue), QString("text"), tr("%1").arg(minValue) );
        am->updateAttr( tr("Custom Max:"), tr("%1").arg(maxValue), QString("text"), tr("%1").arg(maxValue) );
    }
    FVObject::update();
//    manager->sendMessage(QString("update"), this, true );
}

FVInterface * FVFieldDrawXml::getInterface( QString interfaceName )
{
    return parentInterface(interfaceName);
}

FVFieldDrawXml::~FVFieldDrawXml()
{
}

bool FVFieldDrawXml::showElem( int ie )
{
//        Elem * el = grid->_elems[ ie ];

//        if ( ! el->isExternal() )
//                return false;
//        if( visibleSub.length() > 0 && ! visSub.find( el->subdomain() )  )
//                return false;

        return true;
}

void FVFieldDrawXml::fillValuesOfFace( int ie, const dolfin::Facet & facet, double e[3][3], double v[3], dolfin::Array<double> val )
{
        const uint* verts= facet.entities(0);
        SET(e[0], mesh->geometry().point(verts[0]).coordinates());
        SET(e[1], mesh->geometry().point(verts[1]).coordinates());
        SET(e[2], mesh->geometry().point(verts[2]).coordinates());

        if (((FVBoxFieldXml*)boxFieldXml)->isVector){
                v[0] = 0;
                v[1] = 0;
                v[2] = 0;
                for (int vi = 0; vi < 3; vi++ ){
                    v[0] += val[verts[0]+vi*val.size()/3] * val[verts[0]+vi*val.size()/3];
                    v[1] += val[verts[1]+vi*val.size()/3] * val[verts[1]+vi*val.size()/3];
                    v[2] += val[verts[2]+vi*val.size()/3] * val[verts[2]+vi*val.size()/3];
                }
                v[0] = sqrt(v[0]);
                v[1] = sqrt(v[1]);
                v[2] = sqrt(v[2]);
        } else {
                v[0] = val[verts[0]];
                v[1] = val[verts[1]];
                v[2] = val[verts[2]];
        }

}

void FVFieldDrawXml::fillValuesOfFace( uint* maps, const dolfin::Face & face, double e[3][3], double v[3], dolfin::Array<double> val )
{
        const uint* verts= face.entities(0);
        SET(e[0], bmesh->geometry().point(verts[0]).coordinates());
        SET(e[1], bmesh->geometry().point(verts[1]).coordinates());
        SET(e[2], bmesh->geometry().point(verts[2]).coordinates());

        uint mappedverts[3];
        mappedverts[0] = maps[verts[0]];
        mappedverts[1] = maps[verts[1]];
        mappedverts[2] = maps[verts[2]];

        if (((FVBoxFieldXml*)boxFieldXml)->isVector){
                v[0] = 0;
                v[1] = 0;
                v[2] = 0;
                for (int vi = 0; vi < 3; vi++ ){
                    v[0] += val[mappedverts[0]+vi*val.size()/3] * val[mappedverts[0]+vi*val.size()/3];
                    v[1] += val[mappedverts[1]+vi*val.size()/3] * val[mappedverts[1]+vi*val.size()/3];
                    v[2] += val[mappedverts[2]+vi*val.size()/3] * val[mappedverts[2]+vi*val.size()/3];
                }
                v[0] = sqrt(v[0]);
                v[1] = sqrt(v[1]);
                v[2] = sqrt(v[2]);
        } else {
                v[0] = val[mappedverts[0]];
                v[1] = val[mappedverts[1]];
                v[2] = val[mappedverts[2]];
        }

}

void FVFieldDrawXml::drawColormap( int ie, const dolfin::Facet & f, dolfin::Array<double> val )
{
    double e[3][3];
    double v[3];

    fillValuesOfFace(ie,f,e,v, val);

    dolfin::Point p = f.normal();
    double* norm;
    norm = p.coordinates();
    SET(cm.norm, norm);

    cm.drawTriangleGL(e,v);
}

void FVFieldDrawXml::drawColormap( uint* maps, const dolfin::Face & f, dolfin::Array<double> val )
{
    double e[3][3];
    double v[3];

    fillValuesOfFace(maps,f,e,v, val);

    const uint* p = f.entities(0);
    dolfin::Point points[3];
    points[0] = bmesh->geometry().point(p[0]);
    points[1] = bmesh->geometry().point(p[1]);
    points[2] = bmesh->geometry().point(p[2]);

    double norm[3];
    double center[3];
    FVHelpers::getCenter(bmesh,center);
    FVHelpers::normalny4p(points[0].coordinates(), points[1].coordinates(), points[2].coordinates(),center, norm);
    FVHelpers::normVec(points[0].coordinates(), points[1].coordinates(), points[2].coordinates(), norm);

//    double* norm;
//    dolfin::Point np = f.normal();
//    norm = np.coordinates();

    SET(cm.norm, norm);

    cm.drawTriangleGL(e,v);
}

void FVFieldDrawXml::drawIsolines( int ie, const dolfin::Facet & f, dolfin::Array<double> val )
{
        double e[3][3];
        double v[3];

        fillValuesOfFace(ie,f,e,v, val);

        iso.drawTriangleGL(e,v);
}

void FVFieldDrawXml::drawIsolines( uint* maps, const dolfin::Face & f, dolfin::Array<double> val )
{
        double e[3][3];
        double v[3];

        fillValuesOfFace(maps,f,e,v, val);

        iso.drawTriangleGL(e,v);
}

void FVFieldDrawXml::drawVector( double c[], double v[])
{

        double vn[3];
        double lv[3];
        double ang;
        double l;

        double headLgt= vectorHeadLength;
        double headThick= vectorHeadThickness;

#if 0
        // do testowania
        glDisable(GL_BLEND);
        glDisable(GL_LIGHTING);
        glLineWidth(1.0);
        glBegin( GL_LINES );

        glVertex3f( c[0], c[1], c[2] );
        glVertex3f( c[0]+(1-headLgt)*v[0], c[1]+(1-headLgt)*v[1], c[2]+(1-headLgt)*v[2] );
        glLineWidth(5.0);
        glVertex3f( c[0]+v[0], c[1]+v[1], c[2]+v[2] );
        glEnd();
#endif

        l = LENGTH(v);
        VERSOR(lv,v);
        vn[0] = -lv[1];
        vn[1] = lv[0];
        vn[2] = 0;

        /* ang is the angle between OZ axis and normalized lv */
        ang = acos( lv[2] ) * 180 / M_PI;

        vn[0] *= headLgt;
        vn[1] *= headLgt;

        glPushMatrix();
        //glTranslatef(c[0]+(1-headLgt)*v[0], c[1]+(1-headLgt)*v[1], c[2]+(1-headLgt)*v[2]);
        glTranslatef(c[0], c[1], c[2]);
        glScalef( l, l, l );
        glRotatef(ang, vn[0], vn[1], vn[2]);

        GLUquadric * quad = gluNewQuadric();

        //walec
        gluQuadricDrawStyle( quad, GLU_FILL );
        gluCylinder( quad, vectorThickness, vectorThickness, 1.0-headLgt, headFaces, 1 );

        // podstawa walca
        gluQuadricDrawStyle( quad, GLU_FILL );
        gluQuadricOrientation(quad, GLU_INSIDE);
        gluDisk(quad, 0.0, vectorThickness, headFaces, 1);

        // stozek
        gluQuadricDrawStyle( quad, GLU_FILL );
        gluQuadricOrientation(quad, GLU_OUTSIDE);
        glTranslatef(0,0,1-headLgt);
        gluCylinder( quad, headThick*headLgt, 0.0, headLgt, headFaces, 1 );

        // podstawa stozka
        gluQuadricDrawStyle( quad, GLU_FILL );
        gluQuadricOrientation(quad, GLU_INSIDE);
        gluDisk(quad, 0.0, headThick*headLgt, headFaces, 1);


        gluDeleteQuadric( quad );

        glPopMatrix();

}

void FVFieldDrawXml::drawVectorElem( int ie, dolfin::Array<double> val )
{
    std::vector<double> c;
    double fscale;
    double r,g,b;
    double modv;
    double v[3];

    if ( maxValue > 0 )
        if( logScale )
            fscale = 0.05 * modelBaseLength * baseLength / log(maxValue/minValue);
    else
        fscale = 0.05 * modelBaseLength *baseLength / maxValue;
    else
        fscale = 1;

    if( sameColor ) {
        r = (GLfloat) sameColorColor.red()/255;
        g = (GLfloat) sameColorColor.green()/255;
        b = (GLfloat) sameColorColor.blue()/255;
        glColor3f( r,g,b );
    }

    int de= mesh->num_cells() < 200 ? 1 : mesh->num_cells() / 400;
    de = (int)( de / vectorsDensity);

    dolfin::Cell el(*mesh, ie);

    c.assign(el.midpoint().coordinates(), el.midpoint().coordinates()+3);
    v[2] = 0;
    modv = 0;

    for (int i = 0; i < 3; i++) {
        modv += val[ie+i*val.size()/3] * val[ie+i*val.size()/3];
        v[i] = val[ie+i*val.size()/3];
    }


    modv = sqrt(modv);
    VERSOR( v, v );
    if( sameLength ) {
        MULT( v, v, 0.05* modelBaseLength *baseLength );
        //scale= 1.0;
    } else {
        if( logScale ) {
            MULT( v, v, log(modv/minValue) * fscale );
            //scale= log(modv/maxValue);
        } else {
            MULT( v, v, modv * fscale );
            //scale= modv/maxValue;
        }
    }

    cm.setMinMax(minValue,maxValue,logScale);

    if( (dontMissBigV && modv > 0.95 * maxValue) || de < 2 || el.index() % de == 0 ) { // dont miss big vectors!
        if( !sameColor ) {
            cm.getRGBColor( modv, r, g, b);
            glColor3f(r,g,b);
        }

        double c1[3];
        SET(c1,c);
        drawVector(c1,v);
    }
}

void FVFieldDrawXml::drawVectorElem( dolfin::Cell el, dolfin::Array<double> val )
{
    std::vector<double> c;
    double fscale;
    double r,g,b;
    double modv;
    double v[3];
    int ie = el.index();

    if ( maxValue > 0 )
        if( logScale )
            fscale = 0.05 * modelBaseLength * baseLength / log(maxValue/minValue);
    else
        fscale = 0.05 * modelBaseLength *baseLength / maxValue;
    else
        fscale = 1;

    if( sameColor ) {
        r = (GLfloat) sameColorColor.red()/255;
        g = (GLfloat) sameColorColor.green()/255;
        b = (GLfloat) sameColorColor.blue()/255;
        glColor3f( r,g,b );
    }

    int de= mesh->num_cells() < 200 ? 1 : mesh->num_cells() / 400;
    de = (int)( de / vectorsDensity);

    c.assign(el.midpoint().coordinates(), el.midpoint().coordinates()+3);
    v[2] = 0;
    modv = 0;

    for (int i = 0; i < 3; i++) {
        modv += val[ie+i*val.size()/3] * val[ie+i*val.size()/3];
        v[i] = val[ie+i*val.size()/3];
    }


    modv = sqrt(modv);
    VERSOR( v, v );
    if( sameLength ) {
        MULT( v, v, 0.05* modelBaseLength *baseLength );
        //scale= 1.0;
    } else {
        if( logScale ) {
            MULT( v, v, log(modv/minValue) * fscale );
            //scale= log(modv/maxValue);
        } else {
            MULT( v, v, modv * fscale );
            //scale= modv/maxValue;
        }
    }

    cm.setMinMax(minValue,maxValue,logScale);

    if( (dontMissBigV && modv > 0.95 * maxValue) || de < 2 || ie % de == 0 ) { // dont miss big vectors!
        if( !sameColor ) {
            cm.getRGBColor( modv, r, g, b);
            glColor3f(r,g,b);
        }

        double c1[3];
        SET(c1,c);
        drawVector(c1,v);
    }
}

void FVFieldDrawXml::drawVectorNode( int in , dolfin::Array<double> val )
{
    double fscale;
    double r,g,b;
    double modv;
    double v[3];

    if ( maxValue > 0 )
        if( logScale )
            fscale = 0.05 * modelBaseLength * baseLength / log(maxValue/minValue);
    else
        fscale = 0.05 * modelBaseLength *baseLength / maxValue;
    else
        fscale = 1;

    if( sameColor ) {
        r= (GLfloat) sameColorColor.red()/255;
        g= (GLfloat) sameColorColor.green()/255;
        b= (GLfloat) sameColorColor.blue()/255;
        glColor3f( r,g,b );
    }

    int de= mesh->num_vertices() < 200 ? 1 : mesh->num_vertices() / 400;
    de = (int)( de / vectorsDensity);

    double* coords = mesh->geometry().x(in);
    v[2] = 0;
    modv = 0;
    for (int i = 0; i < 3; i++) {
        modv += val[in+i*val.size()/3] * val[in+i*val.size()/3];
        v[i] = val[in+i*val.size()/3];
    }

    modv = sqrt(modv);
    VERSOR( v, v );
    if( sameLength ) {
        MULT( v, v, 0.05* modelBaseLength *baseLength );
        //scale= 1.0;
    } else {
        if( logScale ) {
            MULT( v, v, log(modv/minValue) * fscale );
            //scale= log(modv/maxValue);
        } else {
            MULT( v, v, modv * fscale );
            //scale= modv/maxValue;
        }
    }

    cm.setMinMax(minValue,maxValue,logScale);

    if( (dontMissBigV && modv > 0.95 * maxValue) || de < 2 || in % de == 0 ) { // dont miss big vectors!
        if( !sameColor ) {
            cm.getRGBColor( modv, r, g, b);
            glColor3f(r,g,b);
        }

        drawVector(coords,v);
    }
}

void FVFieldDrawXml::drawVectorNode( dolfin::Vertex vert , dolfin::Array<double> val )
{
    double fscale;
    double r,g,b;
    double modv;
    double v[3];
    int in = vert.index();

    if ( maxValue > 0 )
        if( logScale )
            fscale = 0.05 * modelBaseLength * baseLength / log(maxValue/minValue);
    else
        fscale = 0.05 * modelBaseLength *baseLength / maxValue;
    else
        fscale = 1;

    if( sameColor ) {
        r= (GLfloat) sameColorColor.red()/255;
        g= (GLfloat) sameColorColor.green()/255;
        b= (GLfloat) sameColorColor.blue()/255;
        glColor3f( r,g,b );
    }

    int de= mesh->num_vertices() < 200 ? 1 : mesh->num_vertices() / 400;
    de = (int)( de / vectorsDensity);

    double* coords = vert.point().coordinates();//mesh->geometry().x(in);
    v[2] = 0;
    modv = 0;
    for (int i = 0; i < 3; i++) {
        modv += val[in+i*val.size()/3] * val[in+i*val.size()/3];
        v[i] = val[in+i*val.size()/3];
    }

    modv = sqrt(modv);
    VERSOR( v, v );
    if( sameLength ) {
        MULT( v, v, 0.05* modelBaseLength *baseLength );
    } else {
        if( logScale ) {
            MULT( v, v, log(modv/minValue) * fscale );
        } else {
            MULT( v, v, modv * fscale );
        }
    }

    cm.setMinMax(minValue,maxValue,logScale);

    if( (dontMissBigV && modv > 0.95 * maxValue) || de < 2 || in % de == 0 ) { // dont miss big vectors!
        if( !sameColor ) {
            cm.getRGBColor( modv, r, g, b);
            glColor3f(r,g,b);
        }

        drawVector(coords,v);
    }
}


void FVFieldDrawXml::paintGL( )
{
        int ie, in;
        int iFace;
//        Elem * el;
//        CFace face;
        GLfloat tx,ty,tz;
        time_t dstart, dend;

        dstart = time(0);
        qDebug("FVFieldDrawXml::paintGL started");

        if (field ==0)
            field = reqField.getFunction(parentObject());
        if (mesh == 0)
            mesh = reqGrid.getMesh(parentObject());

        if (mesh == 0) {
                qWarning() << "FVFieldDrawXml: Grid pointer for " << sName << " is NULL. Unable to draw field.";
                return;
        }

        readAttributes();

        if (isFirstShow()) {
            double pmin[3], pmax[3];

            FVHelpers::getBBox(mesh,pmin,pmax);
            tx = (pmax[0] - pmin[0]) ;
            ty = (pmax[1] - pmin[1]) ;
            tz = (pmax[2] - pmin[2]) ;

            double td = sqrt(tx*tx + ty*ty + tz*tz);
            if ((td > 1e-10 ) && (td < 1e10)) {
                getCurrentViewer()->setSceneRadius( td );
                modelBaseLength = td;
            }
        }

    // Poinformuj wszystkie dzieci o aktualnej skali,
    // która zostanie przekazana  do ColorMapy
    QList<QString> args;
    args.append(QString("min_max_field"));
    args.append(QString::number(minValue));
    args.append(QString::number(maxValue));
    manager->sendMessageToChildrenOf( args, this );
    ///////////////////////////////////////////////

    fvlist->start();
    if (drawMode == "colormap" ) {
                cm.setDoGLInit(false);
                cm.setMinMax(minValue,maxValue,logScale);
                cm.init( colormapType );

                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                glBegin(GL_TRIANGLES);
        }
        if (drawMode == "isolines") {
                cm.setDoGLInit(true);
                iso.isolines_no = isolines_no;
                iso.setWidth(isolinesWidth);
                iso.setMinMax(minValue,maxValue,logScale);
                glDisable(GL_LIGHTING);
        }

        if ( (drawMode == "colormap") || (drawMode == "isolines") ) {
            bmesh = reqGrid.getBoundaryMesh(parentObject());

            //Rysowanie zewnętrznej siatki - zmodyfikowane
            dolfin::MeshFunction<uint> cellmap = bmesh->cell_map();
            uint* cellmaps = cellmap.values();
            int step = bmesh->num_cells()/10;
            int i=0;
            for (dolfin::FaceIterator f(*bmesh); !f.end(); ++f)
            {
                uint mappedindex = cellmaps[(*f).index()];
                dolfin::Facet facet (*mesh, mappedindex);
                if (i%step == 0)
                    std::cout << "drawing progress " << i/step*10 << "% during " << time(0)- dstart << " seconds" << std::endl;
                i++;
                if (drawMode == "colormap")
                    drawColormap( 1, facet , val);
                if (drawMode == "isolines")
                    drawIsolines( 1, facet, val );
            }

//            //Rysowanie tylko zewnętrznej siatki
//            dolfin::MeshFunction<uint> vermap = bmesh->vertex_map();
//            uint* vermaps = vermap.values();

//            for (dolfin::FaceIterator f(*bmesh); !f.end(); ++f)
//            {
//                if (drawMode == "colormap")
//                    drawColormap( vermaps, *f , val);
//                if (drawMode == "isolines")
//                    drawIsolines( vermaps, *f, val );
//            }

//          //Rysowanie wszystkiego...
//            int num_cells = mesh->num_cells();
//            int step = num_cells/10;
//            int i=0;

//            for (dolfin::CellIterator c(*mesh); !c.end(); ++c)
//            {
//                if (i%step == 0)
//                    std::cout << "narysowano " << i/step*10 << "% siatki przez " << time(0)- dstart << "sekund" << std::endl;
//                i++;
//                for (dolfin::FacetIterator f(*c); !f.end(); ++f)
//                {
//                        if (drawMode == "colormap")
//                            drawColormap( (*c).index(), *f , val);
//                        if (drawMode == "isolines")
//                            drawIsolines( (*c).index(), *f, val );
//                }
//            }
        }

        if ( drawMode == "vectors" ) {
            vectorThickness= fvsettings.value( QString("/RSoft/FViewer/vector.thickness"), QVariant("0.04") ).toDouble();
            dontMissBigV= fvsettings.value( QString("/RSoft/FViewer/vector.dont-miss-big-vectors"), QVariant("true") ).toBool();
            vectorHeadLength= fvsettings.value( QString("/RSoft/FViewer/vector.head.length"), QVariant("0.5") ).toDouble();
            vectorHeadThickness= fvsettings.value( QString("/RSoft/FViewer/vector.head.thickness"), QVariant("0.2") ).toDouble();
            headFaces= fvsettings.value( QString("/RSoft/FViewer/vector.head.faces"), QVariant("4") ).toInt();
            if( ! fvsettings.value( QString("/RSoft/FViewer/vector.lighting"), QVariant("false") ).toBool() )
                glDisable(GL_LIGHTING);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            cm.init( colormapType );
            if ( val.size() == mesh->num_cells() ){
                int i=0;
                int step = mesh->num_cells()/10;
                for (dolfin::CellIterator c(*mesh); !c.end(); ++c)
                {
                    if (i%step == 0)
                        std::cout << "drawing progress " << i/step*10 << "% during " << time(0)- dstart << " seconds" << std::endl;
                    i++;
                    drawVectorElem((*c), val);
                }
            } else {
                int step = mesh->num_vertices()/10;
                int i=0;
                for (dolfin::VertexIterator v(*mesh); !v.end(); ++v)
                {
                    if (i%step == 0)
                        std::cout << "drawing progress " << i/step*10 << "% during " << time(0)- dstart << "seconds" << std::endl;
                    i++;
                    drawVectorNode((*v).index(), val);
                }
            }
        }

        if (drawMode == "colormap" ) {
                glEnd();
        }
        glEnable(GL_LIGHTING);
        fvlist->end();
        dend = time(0);
        qDebug() << "Drawing took" << dend- dstart << " second(s)." << endl;

}

void FVFieldDrawXml::setupAttributes( )
{
        Attr * a;
        a = am->addAttr( tr("Type"), tr("Field"), QString("text") );
        a->setEditable(false);
        a = am->addAttr( tr("Name"), tr(""), QString("text") );
        a->setEditable(false);

        a = am->addAttr( tr("Use Custom Min/Max:"), QString("No"), QString("boolean") );
        a = am->addAttr( tr("Custom Min:"), tr("%1").arg(minValue), QString("text"), tr("%1").arg(minValue) );
        a = am->addAttr( tr("Custom Max:"), tr("%1").arg(maxValue), QString("text"), tr("%1").arg(maxValue) );

        a = am->addAttr( tr("View Style"), tr("colormap"), QString("textlist") );
        a->addListItem( tr("colormap") );
        a->addListItem( tr("isolines") );
        a->addListItem( tr("vectors") );

        a = am->addAttr( tr("Log. Scale"), tr("Linear"), QString("textlist") );
        a->addListItem( tr("Linear") );
        a->addListItem( tr("Log10") );

        a = am->addAttr( tr("Colormap Type"), tr("Jet"), QString("textlist") );
        a->addListItem( tr("Jet") );
        a->addListItem( tr("Gray") );
        a->addListItem( tr("XRay") );

        a = am->addAttr( tr("Visible Subdomains"), tr("%1-%2").arg(minSub).arg(maxSub), QString("text") );
        a = am->addAttr( tr("No. of Isolines"), tr("15"), QString("text") );
        a = am->addAttr( tr("Isolines Width"), tr("1.0"), QString("text") );

        am->addSection( tr("Vectors") );
        a = am->addAttr( tr("+ Length"), tr("1.0"), QString("text") );
        a = am->addAttr( tr("+ Same Color"), tr("No"), QString("boolean") );
        a = am->addAttr( tr("+ Same Color Value"), tr("(125,125,125)"), QString("color") );
        sameLength = fvsettings.value( QString("/RSoft/FViewer/vector.same.length"), QVariant("true") ).toBool();
        a = am->addAttr( tr("+ Same Length"), (sameLength ? tr("Yes") : tr("No")), QString("boolean") );
        a = am->addAttr( tr("+ Density"), tr("1.0"), QString("text") );

}

void FVFieldDrawXml::readAttributes( )
{
    QString cmt = getAttrValue( tr("Colormap Type") );
    if (cmt == "Jet") colormapType = ColorMap::Jet;
    if (cmt == "Gray") colormapType = ColorMap::Gray;
    if (cmt == "XRay") colormapType = ColorMap::XRay;

    visibleSub = getAttrValue( tr("Visible Subdomains") );
    visSub.init( visibleSub, minSub, maxSub );
    isolines_no = getAttrValue( tr("No. of Isolines") ).toInt();
    isolinesWidth = getAttrValue( tr("Isolines Width") ).toDouble();
    baseLength = getAttrValue( tr("+ Length") ).toDouble();
    sameColor = (getAttrValue( tr("+ Same Color") ) == "Yes");
    vectorsDensity = getAttrValue( tr("+ Density") ).toDouble();
    sameLength = (getAttrValue( tr("+ Same Length") ) == "Yes");
    Attr * a = getModel()->getAttr( tr("+ Same Color Value" ) );
    if (a != 0) sameColorColor = a->color();

    if (getAttrValue( tr("Use Custom Min/Max:") ) == "Yes") {
        minValue = getAttrValue( tr("Custom Min:") ).toDouble();
        maxValue = getAttrValue( tr("Custom Max:") ).toDouble();
    } // w przeciwnym wypadku (Use Custom.. == No) min i max ustawione w updateAtrri

    drawMode = getAttrValue( tr("View Style") );

    if ( (getAttrValue( tr("Log. Scale") ) == "Log10") && (minValue > 0) )
        logScale = true;
    else
        logScale = false;
}

void FVFieldDrawXml::calculateMinMax( void ) {
    if (field == 0)
        field = reqField.getFunction(parentObject());
    if (mesh ==0 )
        mesh = reqGrid.getMesh(parentObject());

    if( field != 0 ) {
//        if( visibleSub.length() == 0 ) {
//        dolfin::Array<double> val;
//        field->compute_vertex_values(val,*mesh);

            minValue= val.min( );
            maxValue= val.max( );
//        } else {
//            minValue= 1e78;
//            maxValue= -1e78;
//            qDebug() << "FVFieldDrawXml::calculateMinMax: iterating over visible subdomains";
//            for( std::set<int>::iterator i= visSub.begin(); i != visSub.end(); i++ ) {
//                double minI= field->min(grid,*i);
//                double maxI= field->max(grid,*i);
//                qDebug() << "FVFieldDrawXml::readAttributes: subDomain no. " << *i << ": min=" << minI << " max=" << maxI;
//                minValue = minValue < minI ? minValue : minI;
//                maxValue = maxValue > maxI ? maxValue : maxI;
//            }
//        }
    }
}

bool FVFieldDrawXml::showNode( int in )
{

//        Node * n = &grid->_nodes[ in ];
//        if( n == 0 )
//            return false;

//        if( visibleSub.length() == 0 )
//            return true;

//        vector<int> *elems= grid->getNodeNbrs( in);

//        for( vector<int>::iterator i= elems->begin(); i != elems->end(); i++ ) {
//            Elem * el = grid->_elems[ *i - 1 ];

//            if( visSub.find( el->subdomain() )  ) // wiadomo, że visibleSub.length() > 0
//                return true;
//        }

        return false;
}

void FVFieldDrawXml::setupMenu()
{
    contextMenuObj->clear();

    contextMenuObj->addAction(tr("&Draw Field Legend"), this, SLOT( slotDrawFieldLegend() ) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );

}

void FVFieldDrawXml::slotDrawFieldLegend()
{
    FVFieldLegend * fl = new FVFieldLegend( manager );
    addChild(fl);
    fl->update();
}
