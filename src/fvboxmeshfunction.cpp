#include "fvboxmeshfunction.h"
#include "fviewer.h"
#include "fivermain.h"
#include "main.h"
#include <fvgridinterface.h>
#include <attrmodel.h>
#include <attr.h>
#include "setofint.h"

#include <QtDebug>
#include <QMenu>
#include <QStringList>
#include <QMessageBox>

#include <ctime>
#include <set>

FVBoxMeshFunction::FVBoxMeshFunction(FVBoxMgr * manager, FVObject * parent, std::string name, int x, int y)
 : FVObject(manager, x,y)
{
    mesh = 0;
    mf = 0;
    this->parent = parent;
    this->name = name;
    initDrawable(QString::fromStdString(name), "MeshFun");
}

FVBoxMeshFunction::~FVBoxMeshFunction()
{
}

void FVBoxMeshFunction::updateAttributes( )
{
//    FVObject::update();
    manager->sendMessage(QString("update"), this, true );
}

int FVBoxMeshFunction::findMax( )
{
    int max = -99999999;
    if ( mf != 0 ){
        int* vals = mf->values();
        for (int i = 0; i < mf->size() ; i++ ){
            if ( vals[i] > max){
                max = vals[i];
            }
        }
        return max;
    }
    return 1;
}

int FVBoxMeshFunction::findMin( )
{
    int min = 99999999;
    if ( mf != 0 ){
        int* vals = mf->values();
        for (int i = 0; i < mf->size() ; i++ ){
            if ( vals[i] < min){
                min = vals[i];
            }
        }
        return min;
    }
    return 0;
}

void FVBoxMeshFunction::paintGL()
{
    time_t tstart, tend;

    tstart = time(0);
    if (mesh == 0) {
        qDebug() << classType() << ": Trying to draw mesh while getMesh() returned 0." << endl;
        return;
    }

    if (fvlist == 0) {
        qDebug() << classType() << ":fvlist == 0. Probably You have not set that this class is drawable. Please use setDrawable(true) method in the constructor." << endl;
        return;
    }

    fvlist->start();

    if (getAttrValue( tr("Visible") ) == tr("Yes")) {
        qDebug() << "Drawing..." << endl;
        if (getAttrValue( tr("Transparent") ) == tr("Yes") ) {
            glEnable( GL_BLEND );
            glEnable( GL_ALPHA_TEST );
            //glDisable(GL_DEPTH_TEST);
            glAlphaFunc( GL_LESS, 1.f );
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        if (mf != 0) {
            if ( mf->dim() == 3 )
                draw3();
            if ( mf->dim() == 2 )
                draw2b();
        }


        glEnable(GL_DEPTH_TEST);
        glDisable( GL_ALPHA_TEST );
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);

    } else
        qDebug() << "Not Drawing..." << endl;
    fvlist->end();
    tend = time(0);
    qDebug() << "Drawing took " << tend- tstart << " second(s)." << endl;
}


QColor FVBoxMeshFunction::getColor()
{
            Attr * a;
                    a = getAttr( tr("Color") );
                    if (a != 0)
                            return a->color();
            return QColor(200,200,200);
}

QColor FVBoxMeshFunction::getColor( int isub )
{
        Attr * a;
        if ( getAttrValue( tr("Sub.Color Policy") ) == "Single Color" ) {
                a = getAttr( tr("Color") );
                if (a != 0)
                        return a->color();
        } else {
                a = getAttr( tr("Function Value %1").arg( isub ) );
                if (a != 0)
                        return a->color();
        }

        return QColor(200,200,200);
}

void FVBoxMeshFunction::initSubColors( )
{
//        mesh = reqGrid.getGrid( parentObject(), parent );

//        if (mesh != 0) {
//                subColors.clear();
//                for( int i = 1; i <= (int) grid->_subdomains.size(); i++ )
//                        subColors[ i ] = getColor( i );
//        }
}

void FVBoxMeshFunction::initSubVisibility( )
{
//        grid = reqGrid.getGrid( parentObject(), parent );

//        if (grid != 0) {
//                SetOfInt so( getAttrValue( tr("Visible Subdomains") ), 1, grid->_subdomains.size() );
//                subVisible.clear();
//                for( int i = 1; i <= (int) grid->_subdomains.size(); i++ ) {

//                        if ( so.find( i ) ) {
//                                subVisible[ i ] = true;
//                        } else {
//                                subVisible[ i ] = false;
//                        }
//                }
//        }
}

void FVBoxMeshFunction::drawSubdomainWireframe()
{
//        bool bDraw;
//        bool bElements = false;
//        int i,j;
//        QColor cl;
//        GLfloat fTransparency = 0;

//        glDisable(GL_BLEND);
//        glDisable(GL_LIGHTING);
//        glEnable( GL_LINE_SMOOTH );
//        glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

//        //qDebug() << "getLineWidth = " << getLineWidth();
//        glLineWidth( getLineWidth() );

//        glBegin( GL_LINES );
//        for (i = 0; i < (int) grid->_elems.size(); i++) {
//                Elem * e;
//                        //Node * n;
//                int ie;
//                e = grid->_elems[ i ];
//                if( subVisible[ e->subdomain() ] == false  )
//                        continue;

//                cl = subColors[ e->subdomain() ];
//                glColor4f((GLfloat) cl.red()/255,
//                                   (GLfloat) cl.green()/255,
//                                   (GLfloat) cl.blue()/255,
//                                   fTransparency);

//                if (bElements || e->isExternal() ) {
//                        for (j=0; j < e->getFacesCount();j++) {
//                                bDraw=false;

//                                if ((ie = e->getNeighbourWithFaceNr(j)) == 0) {
//                                        bDraw = true;
//                                } else {
//                                        if (grid->_elems[ ie - 1 ]->subdomain() != e->subdomain())
//                                                bDraw = true;
//                                }

//                                if (bDraw) {
//                                        CEdge edge;
//                                        ElemList elms;
//                                        for (int k =0; k < e->getEdgesCount(); k++) {
//                                                edge = e->getEdge( k );
//                                                e->getNeighboursWithEdge( edge, elms );

//                                                if (elms.count() > 0 ) {
//                                                        bDraw = false;
//                                                        CElem * ce = elms.getFirst();
//                                                        while (ce != 0) {
//                                                                if (grid->getElem( ce->nr )->subdomain() != e->subdomain())
//                                                                        bDraw = true;

//                                                                ce = ce->next;
//                                                        }
//                                                } else
//                                                        bDraw = true;

//                                                if (bDraw) {
//                                                        Node *n1, *n2;
//                                                        n1 = grid->getNode( edge.n1 );
//                                                        n2 = grid->getNode( edge.n2 );
//                                                        glVertex3f(n1->x(), n1->y(), n1->z() );
//                                                        glVertex3f(n2->x(), n2->y(), n2->z() );
//                                                }

///*					CFace f;
//                                        f = e->getFaceOrder( j );
//                                        f.fill(grid->_nodes); // fill geometrical coordinates of CFace, from nodes vector
//                                        f.shrink( dShrink );

//                                        e->getNormalOfFace( j, nr );
//                                        SET(nf,nr);
//                                        VERSOR(nf,nf);
//                                        if (paintMode == "Solid" || (paintMode == "Elements") )
//                                                glNormal3f( nf[0], nf[1], nf[2] );
//                                        */
//                                        }
//                                }
//                        }
//                }
//        }
//        glEnd();
}

void FVBoxMeshFunction::draw3(/*QString & paintMode, double dShrink*/)
{
    int i,j,k;
    GLfloat fTransparency = 0;

    int min = findMin();
    int max = findMax();
    int value = 0;
    int* vals = mf->values();

    fTransparency = getAttrValue( tr("Transparency Ratio") ).toFloat();
    SetOfInt visEle( getAttrValue(tr("Interesting Elements")), 1, mesh->num_entities(mf->dim()) );
    SetOfInt visSub( getAttrValue(tr("Visible values")), min, max );

//    if ((paintMode == "Solid") || (paintMode == "Elements")) {
//            if (paintMode == "Elements") bElements = true;
            glShadeModel(GL_SMOOTH);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glBegin( GL_TRIANGLES );
//    }
//    if (paintMode == "Wireframe") {
//            glDisable(GL_BLEND);
//            glDisable(GL_LIGHTING);
//            glLineWidth( getLineWidth() );
//            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//            glBegin( GL_TRIANGLES );
//    }
    dolfin::MeshConnectivity con = mesh->topology()(3,0);
//        std::cout << std::endl << con.str(true) << std::endl;
//        std::cout << std::endl << mesh->topology()(3,0).str(true) << std::endl;
    const uint* connList = con();


    for (i = 0; i < (int) con.size(); i+=4) {
        if (visEle.find(i/4 + 1)){
            //ustawienie koloru wyświetlania
//            QColor cl = getColor();
            value = vals[i/4];
            if (visSub.find(value)){
                QColor cl = getColor(value);
                glColor4f((GLfloat) cl.red()/255,
                                   (GLfloat) cl.green()/255,
                                   (GLfloat) cl.blue()/255,
                                   fTransparency);
//                glColor4f((GLfloat) (min*value)/max,
//                          (GLfloat) (min*value)/max,
//                          (GLfloat) (min*value)/max,
//                          fTransparency);
                //pobranie punktów czworościanu
                dolfin::Point points[4];
                double n[3];
                points[0] = mesh->geometry().point(connList[i]);
                points[1] = mesh->geometry().point(connList[i+1]);
                points[2] = mesh->geometry().point(connList[i+2]);
                points[3] = mesh->geometry().point(connList[i+3]);

                //wyświetlanie każdej ze ścian
                normalny4p(points[0].coordinates(), points[1].coordinates(), points[2].coordinates(), points[3].coordinates(), n);
                glNormal3f( n[0], n[1], n[2] );
                glVertex3f( points[0].x(), points[0].y(), points[0].z() );
                glVertex3f( points[1].x(), points[1].y(), points[1].z() );
                glVertex3f( points[2].x(), points[2].y(), points[2].z() );

                normalny4p(points[1].coordinates(), points[2].coordinates(), points[3].coordinates(), points[0].coordinates(), n);
                glNormal3f( n[0], n[1], n[2] );
                glVertex3f( points[1].x(), points[1].y(), points[1].z() );
                glVertex3f( points[2].x(), points[2].y(), points[2].z() );
                glVertex3f( points[3].x(), points[3].y(), points[3].z() );

                normalny4p(points[2].coordinates(), points[3].coordinates(), points[0].coordinates(), points[1].coordinates(), n);
                glNormal3f( n[0], n[1], n[2] );
                glVertex3f( points[2].x(), points[2].y(), points[2].z() );
                glVertex3f( points[3].x(), points[3].y(), points[3].z() );
                glVertex3f( points[0].x(), points[0].y(), points[0].z() );

                normalny4p(points[3].coordinates(), points[0].coordinates(), points[1].coordinates(), points[2].coordinates(), n);
                glNormal3f( n[0], n[1], n[2] );
                glVertex3f( points[3].x(), points[3].y(), points[3].z() );
                glVertex3f( points[0].x(), points[0].y(), points[0].z() );
                glVertex3f( points[1].x(), points[1].y(), points[1].z() );
            }
        }
    }
    glEnd();
}

void FVBoxMeshFunction::draw2( )
{
    int i,j,k;
    GLfloat fTransparency = 0;

    int min = findMin();
    int max = findMax();
    int value = 0;
    int* vals = mf->values();

    fTransparency = getAttrValue( tr("Transparency Ratio") ).toFloat();
    SetOfInt visEle( getAttrValue(tr("Interesting Elements")), 1, mesh->num_entities(mf->dim()) );
    SetOfInt visSub( getAttrValue(tr("Visible values")), min, max );

    glShadeModel(GL_SMOOTH);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glBegin( GL_TRIANGLES );

    //trójkąty
    dolfin::MeshConnectivity con = mesh->topology()(2,0);
//            std::cout << std::endl << con.str(true) << std::endl;
//        std::cout << std::endl << mesh->topology()(3,0).str(true) << std::endl;
    const uint* connList = con();

    //trójkąty tworzące czworościany
    dolfin::MeshConnectivity con32 = mesh->topology()(3,2);
    const uint* connList32 = con32();
    //punkty tworzące czworościany
    dolfin::MeshConnectivity con30 = mesh->topology()(3,0);
    const uint* connList30 = con30();


    for (i = 0; i < (int) con.size(); i+=3) {
        if (visEle.find(i/3 + 1)){
            //i/3 to numer trójkąta
            //numery punktów stanowiących trójkąt
            int pp[3];
            pp[0] = connList[i];
            pp[1] = connList[i+1];
            pp[2] = connList[i+2];


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //tetrahedron - szukamy czworościanu z trójkątem który rozważamy
            int tetrahedron=0;
            for (int j=0; j < con32.size(); j++)
                if (connList32[j] == i/3 ){
                    tetrahedron = (int)(j/4);
                    break;
                }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // fourthP - szukamy czwartego punktu tworzącego czworościan do liczenia wektora normalnego
            int fourthP = 0;
            for (int j=0; j<4 ; j++)
                    if ( connList30[4*tetrahedron+j] != pp[0] && connList30[4*tetrahedron+j] != pp[1] && connList30[4*tetrahedron+j] != pp[2]){
                        fourthP = connList30[4*tetrahedron+j];
                        break;
                    }

            value = vals[i/3];
            if (visSub.find(value)){
                //ustawienie koloru wyświetlania
                QColor cl = getColor(value);
                glColor4f((GLfloat) cl.red()/255,
                                   (GLfloat) cl.green()/255,
                                   (GLfloat) cl.blue()/255,
                                   fTransparency);
                //pobranie punktów trójkąta
                dolfin::Point points[4];
                double n[3];
                points[0] = mesh->geometry().point(connList[i]);
                points[1] = mesh->geometry().point(connList[i+1]);
                points[2] = mesh->geometry().point(connList[i+2]);
                //czwarty znaleziony punkt
                points[3] = mesh->geometry().point(fourthP);

                //wyświetlanie trójkąta
                normalny4p(points[0].coordinates(), points[1].coordinates(), points[2].coordinates(), points[3].coordinates(), n);
                glNormal3f( n[0], n[1], n[2] );
                glVertex3f( points[0].x(), points[0].y(), points[0].z() );
                glVertex3f( points[1].x(), points[1].y(), points[1].z() );
                glVertex3f( points[2].x(), points[2].y(), points[2].z() );
            }
        }
    }
    glEnd();
}

void FVBoxMeshFunction::draw2b( )
{
    int i,j,k;
    GLfloat fTransparency = 0;

    int min = findMin();
    int max = findMax();
    int value = 0;
    int* vals = mf->values();

    fTransparency = getAttrValue( tr("Transparency Ratio") ).toFloat();
    SetOfInt visEle( getAttrValue(tr("Interesting Elements")), 1, mesh->num_entities(mf->dim()) );
    SetOfInt visSub( getAttrValue(tr("Visible values")), min, max );
    std::set<int> drawed;

    glShadeModel(GL_SMOOTH);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glBegin( GL_TRIANGLES );

    //trójkąty
    dolfin::MeshConnectivity con20 = mesh->topology()(2,0);
    const uint* connList20 = con20();
    //trójkąty tworzące czworościany
    dolfin::MeshConnectivity con32 = mesh->topology()(3,2);
    const uint* connList32 = con32();
    //punkty tworzące czworościany
    dolfin::MeshConnectivity con30 = mesh->topology()(3,0);
    const uint* connList30 = con30();


    //iterowanie po czworościanach
    for (i = 0; i < (int) con32.size(); i+=4) {
        //numery trójkątów w czworościanie
        int tri[4];
        tri[0] = connList32[i];
        tri[1] = connList32[i+1];
        tri[2] = connList32[i+2];
        tri[3] = connList32[i+3];

        //numery punktów stanowiących czworościan
        int pp[4];
        pp[0] = connList30[i];
        pp[1] = connList30[i+1];
        pp[2] = connList30[i+2];
        pp[3] = connList30[i+3];

        //iteracja po trójkątach w czworościanie
        for ( k=0; k < 4 ; k++ ){
            //czy trójkąt jest widoczny i nie został narysowany
            if (visEle.find(tri[k]+1) && drawed.find(tri[k]+1) == drawed.end()){
                //czy wartość meshfunction mu odpowiadająca nas interesuje
                value = vals[tri[k]];
                if (visSub.find(value)){
                    //numery punktów stanowiących trójkąt
                    int tri_points[3];
                    tri_points[0] = connList20[3*tri[k]];
                    tri_points[1] = connList20[3*tri[k]+1];
                    tri_points[2] = connList20[3*tri[k]+2];

                    // fourthP - szukamy czwartego punktu tworzącego czworościan do liczenia wektora normalnego
                    int fourthP = 0;
                    for (int j=0; j<4 ; j++)
                        if ( pp[j] != tri_points[0] && pp[j] != tri_points[1] && pp[j] != tri_points[2]){
                            fourthP = pp[j];
                            break;
                        }

                    //ustawienie koloru
                    QColor cl = getColor(value);
                    glColor4f((GLfloat) cl.red()/255,
                              (GLfloat) cl.green()/255,
                              (GLfloat) cl.blue()/255,
                              fTransparency);
                    //pobranie punktów trójkąta
                    dolfin::Point points[4];
                    double n[3];
                    points[0] = mesh->geometry().point(tri_points[0]);
                    points[1] = mesh->geometry().point(tri_points[1]);
                    points[2] = mesh->geometry().point(tri_points[2]);
                    //czwarty znaleziony punkt
                    points[3] = mesh->geometry().point(fourthP);

                    //wyświetlanie trójkąta
                    normalny4p(points[0].coordinates(), points[1].coordinates(), points[2].coordinates(), points[3].coordinates(), n);
                    glNormal3f( n[0], n[1], n[2] );
                    glVertex3f( points[0].x(), points[0].y(), points[0].z() );
                    glVertex3f( points[1].x(), points[1].y(), points[1].z() );
                    glVertex3f( points[2].x(), points[2].y(), points[2].z() );

                    drawed.insert(tri[k]+1);
                }
            }
        }

    }
    glEnd();
}


float FVBoxMeshFunction::getLineWidth()
{
        Attr *a = getAttr("Line width");

        if (a != 0)
                return a->value().toFloat();
        else
                return 1.0;

        return 1.0;
}

int FVBoxMeshFunction::getVertexSize( )
{
        Attr *a = getAttr("Vertex Size");

        if (a != 0)
                return a->value().toInt();
        else
                return 4;
}

void FVBoxMeshFunction::paintVertsNums( )
{
        SetOfInt visVerts( getAttrValue(tr("Interesting Vertices")), 1, mesh->num_vertices() );

        for ( int n = 0; n < (int) mesh->num_vertices(); n++) {
                if( ! visVerts.find(n+1) )
                        continue;

                getCurrentViewer()->fvRenderText(mesh->geometry().point(n).x(), mesh->geometry().point(n).y(), mesh->geometry().point(n).z(), QString::number( n+1 ) );
        }

}

void FVBoxMeshFunction::setupAttributes( )
{
        am->clear();
        mesh = reqGrid.getMesh(parentObject(), parent );
//        mf = mesh->data().mesh_function(name).get();

        qDebug() << "Setting up attributes for " << classType();
        // Here add the attributes
        Attr * a;
        QStringList lst;
        a = am->addSection( tr("Viewing") );
        a = am->addAttr(tr("Visible"), QString("No"), "textlist");
        lst.append("No");
        lst.append("Yes");
        a->setList(lst);

        if (mf != 0 ){
            a = am->addAttr( tr("name"), QString("name"), QString("text") );
            a->setEditable(false);
            a = am->addAttr( tr("Size"), QString("%1").arg( mf->size() ) , QString("text") );
            a->setEditable(false);
            a = am->addAttr( tr("Dimension"), QString("%1").arg( mf->dim() ), QString("text") );
            a->setEditable(false);
        }
//        a = am->addAttr( tr("Solid/Wire"), QString("Solid"), "textlist" );
//        QStringList lst;
//        lst.append("Solid");
//        lst.append("Wireframe");
//        lst.append("Elements");
//        lst.append("Vertices");
//        lst.append("Subdomain wireframe");
//        a->setList( lst );

//        a = am->addAttr( tr("Shrink Elems"), QString("1.0"), "text" );
        a = am->addAttr( tr("Vertex Size"), QString("4"), "text" );
        a = am->addAttr( tr("Line width"), QString("1.0"), "text" );

        a = am->addAttr( tr("Transparent"), QString("No"), "boolean" );
        a = am->addAttr( tr("Transparency Ratio"), fvsettings.value("/RSoft/FViewer/DefTransparencyRatio","0.4").toString(), "text" );

        a = am->addSection( tr("Colors") );
        a = am->addAttr( tr("Sub.Color Policy"), QString("Individual Color"), "textlist" );
        lst.clear();
        lst.append("Individual Colors");
        lst.append("Single Color");
        a->setList( lst );

        QVariant v;
        QColor defColor;
        v = fvsettings.value( QString("/RSoft/FViewer/SingleColor"), QVariant(QColor(200,200,200)) );
        defColor = v.value<QColor>();
        a = am->addAttr( tr("Color"), defColor, "color" );

        if (mesh != 0) {
                for (int i = findMin(); i <= findMax(); i++) {
                    v = fvsettings.value( QString("/RSoft/FViewer/Color%1").arg(i+1), QVariant(QColor(100,100,100)) );
                    defColor = v.value<QColor>();
                    a = am->addAttr( tr("Function Value %1").arg(i), defColor, "color" );
                }

                a = am->addSection( tr("Visibility") );
                a = am->addAttr( tr("Visible values"), tr("%1-%2").arg(findMin()).arg(findMax()), "text" );
//                a = am->addAttr( tr("Show Elems Nums"), QString("No"), "boolean" );
                if ( mf != 0 )
                    a = am->addAttr( tr("Interesting Elements"), tr("%1-%2").arg(1).arg(mesh->num_entities(mf->dim())), "text" );
//                a = am->addAttr( tr("Show Verts Nums"), QString("No"), "boolean" );
//                a = am->addAttr( tr("Interesting Vertices"), tr("%1-%2").arg(1).arg(mesh->num_vertices()), "text" );
        }
}

/*
void FVBoxMeshFunction::slotMousePress( QMouseEvent * )
{
        qDebug() << "FVBoxMeshFunction has received the MousePress event.";
}
*/

void FVBoxMeshFunction::setupMenu( )
{
        contextMenuObj->clear();
        /*
        contextMenuObj->addAction(tr("&Connect interaction"), this, SLOT( slotConnect() ) );
        contextMenuObj->addAction(tr("&Disconnect interaction"), this, SLOT(  slotDisconnect() ) );
        */
//        contextMenuObj->addSeparator();
//        contextMenuObj->addAction(tr("&Draw"),this, SLOT(slotDraw()) );
//        contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );

}

void FVBoxMeshFunction::slotDraw( )
{
//    mesh = reqGrid.getMesh( parentObject(), parent );
//    mf = mesh->data().mesh_function(name);
//    std::cout << "MESH FUNCTION name:  " << name << std::endl;
    //    std::cout << mf->str(true) << std::endl;
}

void FVBoxMeshFunction::setMeshFunction(dolfin::MeshFunction<int>* meshfun)
{
    mf = meshfun;
}



