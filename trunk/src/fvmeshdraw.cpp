//
// C++ Implementation: fvgrid
//
// Description:
//
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fvmeshdraw.h"
#include "fviewer.h"
#include "fivermain.h"
#include "main.h"
#include <fvgridinterface.h>
#include <attrmodel.h>
#include <attr.h>
#include "setofint.h"
#include <elemlist.h>

#include <QtDebug>
#include <QMenu>
#include <QStringList>
#include <QMessageBox>

FVMeshDraw::FVMeshDraw(FVBoxMgr * manager, FVObject * parent, int x, int y)
 : FVObject(manager, x,y)
{
    mesh = 0;
    this->parent = parent;
    initDrawable("MeshDraw","Draw");
}

FVMeshDraw::~FVMeshDraw()
{
}

void FVMeshDraw::updateAttributes( )
{
    FVObject::update();
    manager->sendMessage(QString("update"), this, true );
}

void FVMeshDraw::paintGL()
{
        qDebug() << "Drawing..." << endl;

        mesh = reqGrid.getMesh( parentObject(), parent );

        if (mesh == 0) {
                qDebug() << classType() << ": Trying to draw mesh while getMesh() returned 0." << endl;
                return;
        }

        if (fvlist == 0) {
                qDebug() << classType() << ":fvlist == 0. Probably You have not set that this class is drawable. Please use setDrawable(true) method in the constructor." << endl;
                return;
        }

        fvlist->start();

//    GLfloat tx=0,ty=0,tz=0;
//                printf("%lf, %lf, %lf\n",tx,ty,tz);
//        if (isFirstShow()) {
//                double pmin[3], pmax[3], td;
//                grid->getBBox( pmin, pmax );

//                printf("Pmin = %lf, %lf, %lf\n",pmin[0],pmin[1],pmin[2]);
//                printf("Pmax = %lf, %lf, %lf\n",pmax[0],pmax[1],pmax[2]);
//                tx = (pmax[0] - pmin[0]);
//                ty = (pmax[1] - pmin[1]);
//                tz = (pmax[2] - pmin[2]);
//                printf("%lf, %lf, %lf\n",tx,ty,tz);
//                qDebug() << tx << ", " << ty << ", " << tz << endl;
//                td = sqrt(tx*tx + ty*ty + tz*tz);
//                if ((td > 1e-10 ) && (td < 1e10)) {
//                    getCurrentViewer()->setSceneRadius( td );
//                }
//        }
//        if (getAttrValue( tr("Transparent") ) == tr("Yes") ) {
//                glEnable( GL_BLEND );
//                glEnable( GL_ALPHA_TEST );
//                //glDisable(GL_DEPTH_TEST);
//                glAlphaFunc( GL_LESS, 1.f );
//                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        }

        if (mesh != 0) {
                initSubColors();
                initSubVisibility();

                double dShrink = 1.0;
                Attr * a = getAttr( tr("Shrink Elems") );
                if (a != 0)
                        dShrink = a->toDouble();

                QString paintMode = getAttrValue( tr("Solid/Wire") );
                if ( (paintMode == "Solid") || (paintMode == "Wireframe") || (paintMode == "Elements") ) {
                        drawNormal(paintMode, dShrink);
                }
                if ( paintMode == "Vertices" )
                        drawVertices();


//                if ( paintMode == "Subdomain wireframe" )
//                        drawSubdomainWireframe();

//                glDisable(GL_BLEND);
//                glDisable(GL_LIGHTING);

//                glColor4f(0.0f, 0.0f,0.0f,1.0f);
//                glDisable(GL_DEPTH_TEST);
////                if ( getAttrValue(tr("Show Elems Nums")) == "Yes" ) {
////                        paintElemsNums();
////                }
////                if ( getAttrValue(tr("Show Verts Nums")) == "Yes" ) {
////                        paintVertsNums();
////                }
        }


//        glEnable(GL_DEPTH_TEST);
//        glDisable( GL_ALPHA_TEST );
//        glDisable(GL_BLEND);
//        glEnable(GL_LIGHTING);

        fvlist->end();
}

void FVMeshDraw::paintElemsNums( )
{
//        SetOfInt visEle( getAttrValue(tr("Interesting Elements")), 1, grid->_elems.size() );

//        if (visEle.sum() > 1000) {
//            if (QMessageBox::question( 0, tr("To many objects warning."), tr("You are trying to draw over 1000 element numbers. This process may take a long time. Do you want to continue?"), QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No) {
//                setAttrValue(tr("Show Elems Nums"), "No" );
//                return;
//            }
//        }
//        for ( int e = 0; e < (int) grid->_elems.size(); e++) {
//                Elem * el = grid->_elems[ e ];
//                if( ! subVisible[ el->subdomain() ] )
//                        continue;
//                if( ! visEle.find(e+1) )
//                        continue;
//                float ec[3];
//                ec[0]= ec[1]= ec[2]= 0.0;
//                int nNodes= el->_nodes.size();
//                for ( int n=0; n < nNodes; n++) {
//                        int v= el->_nodes[n];
//                        for( int d= 0; d < 3; d++ )
//                                ec[d] += grid->_nodes[v-1]._Coords[d];
//                }
//                for( int d= 0; d < 3; d++ )
//                        ec[d] /= nNodes;
//                getCurrentViewer()->fvRenderText(ec[0], ec[1], ec[2], QString::number( e+1 ) );
//        }
}


QColor FVMeshDraw::getColor( int isub )
{
        Attr * a;
        if ( getAttrValue( tr("Sub.Color Policy") ) == "Single Color" ) {
                a = getAttr( tr("Single Color") );
                if (a != 0)
                        return a->color();
        } else {
                a = getAttr( tr("Subdomain %1").arg( isub ) );
                if (a != 0)
                        return a->color();
        }

        return QColor(200,200,200);
}

void FVMeshDraw::initSubColors( )
{
//        mesh = reqGrid.getGrid( parentObject(), parent );

//        if (mesh != 0) {
//                subColors.clear();
//                for( int i = 1; i <= (int) grid->_subdomains.size(); i++ )
//                        subColors[ i ] = getColor( i );
//        }
}

void FVMeshDraw::initSubVisibility( )
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

void FVMeshDraw::drawSubdomainWireframe()
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

void FVMeshDraw::drawNormal(QString & paintMode, double dShrink)
{
        bool bDraw;
        bool bElements = false;
        int i,j,k;
        QColor cl;
        GLfloat fTransparency = 0;
        fTransparency = getAttrValue( tr("Transparency Ratio") ).toFloat();
        SetOfInt visEle( getAttrValue(tr("Interesting Elements")), 1, mesh->num_faces() );

        if ((paintMode == "Solid") || (paintMode == "Elements")) {
                if (paintMode == "Elements") bElements = true;
                glShadeModel(GL_SMOOTH);
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                glBegin( GL_TRIANGLES );
        }
        if (paintMode == "Wireframe") {
                glDisable(GL_BLEND);
                glDisable(GL_LIGHTING);
                glLineWidth( getLineWidth() );
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                glBegin( GL_TRIANGLES );
        }

        dolfin::MeshConnectivity con = mesh->topology()(2,0);
//        std::cout << std::endl << con.str(true) << std::endl;
//        std::cout << std::endl << mesh->topology()(3,0).str(true) << std::endl;
        const uint* connList = con();

        for (i = 0; i < (int) con.size(); i+=3) {
//            std::cout << connList[i] << " ";
            dolfin::Point points[3];

            points[0] = mesh->geometry().point(connList[i]);
            points[1] = mesh->geometry().point(connList[i+1]);
            points[2] = mesh->geometry().point(connList[i+2]);

            double Ax = points[0].x() - points[1].x();
            double Ay = points[0].y() - points[1].y();
            double Az = points[0].z() - points[1].z();

            double Bx = points[2].x() - points[1].x();
            double By = points[2].y() - points[1].y();
            double Bz = points[2].z() - points[1].z();

            double Nx = Ay*Bz - By*Az;
            double Ny = Bx*Az - Ax*Bz;
            double Nz = Ax*By - Bx*Ay;

            glColor4f((GLfloat) 85/255,
                      (GLfloat) 170/255,
                      (GLfloat) 255/255,
                      fTransparency);

            glNormal3f( Nx, Ny, Nz );
            glVertex3f( points[0].x(), points[0].y(), points[0].z() );
            glVertex3f( points[1].x(), points[1].y(), points[1].z() );
            glVertex3f( points[2].x(), points[2].y(), points[2].z() );


//////////////////////////////////////
//            for (uint e = 0; e < con.num_entities; e++)
//            {
//              s << "  " << e << ":";
//              for (uint i = con.offsets[e]; i < con.offsets[e + 1]; i++)
//                s << " " << con.connections[i];
//              s << std::endl;
//            }


//            std::cout << i << std::endl;
//////////////////////////////////////

//                Elem * e;
//                        //Node * n;
//                int ie;
//                double nr[3];
//                float nf[3];
//                e = grid->_elems[ i ];
//                if( subVisible[ e->subdomain() ] == false  )
//                        continue;

//                if (bElements) {
//                        if( ! visEle.find(i+1) )
//                                continue;
//                }

//                cl = subColors[ e->subdomain() ];
//                glColor4f((GLfloat) cl.red()/255,
//                                   (GLfloat) cl.green()/255,
//                                   (GLfloat) cl.blue()/255,
//                                   fTransparency);

//                if (bElements || e->isExternal() ) {
//                        for (j=0; j < e->getFacesCount();j++) {
//                                bDraw=false;
//                                if (bElements || ((ie = e->getNeighbourWithFaceNr(j)) == 0)) {
//                                        bDraw = true;
//                                } else {
//                                        if (grid->_elems[ ie - 1 ]->subdomain() != e->subdomain())
//                                                bDraw = true;
//                                }

//                                if (bDraw) {
//                                        CFace f;
//                                        f = e->getFaceOrder( j );
//                                        f.fill(grid->_nodes); // fill geometrical coordinates of CFace, from nodes vector
//                                        f.shrink( e->center(), dShrink );

//                                        e->getNormalOfFace( j, nr );
//                                        SET(nf,nr);
//                                        VERSOR(nf,nf);
//                                        if (paintMode == "Solid" || (paintMode == "Elements") )
//                                                glNormal3f( nf[0], nf[1], nf[2] );
//                                        for (k = 0; k < 3; k++) {
//                                                glVertex3f(f.x(k), f.y(k), f.z(k) );

//                                                if (paintMode == "Wireframe") {
//                                                        if (k<2) {
//                                                                glVertex3f(f.x(k+1), f.y(k+1), f.z(k+1) );
//                                                        } else {
//                                                                glVertex3f(f.x(0), f.y(0), f.z(0) );
//                                                        }
//                                                }
//                                        }
//                                }
//                        }
//                }
        }
        glEnd();
}

void FVMeshDraw::drawVertices( )
{
        SetOfInt visVert( getAttrValue(tr("Interesting Vertices")), 1, mesh->num_vertices() );
        float fPointSize = getVertexSize();

        /*
        if (visVert.sum() > 1000) {
            if (QMessageBox::question( 0, tr("To many objects warning."), tr("You are trying to draw over 1000 vertices numbers. This process may take a long time. Do you want to continue?"), QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No) {
                setAttrValue(tr("Show Verts Nums"), "No" );
                return;
            }
        }
        */

        glEnable(GL_POINT_SMOOTH);
        glPointSize( fPointSize );
        glBegin(GL_POINTS);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0);
        dolfin::Point p;

        for (int i = 0; (unsigned int) i < mesh->num_vertices(); i++) {
//                if (visVert.find( i+1 )){
                    p = mesh->geometry().point(i);
                    glVertex3f( p.x(), p.y(), p.z() );
                    std::cout << p.x() << " " <<p.y() << " " <<p.z() << " \n";
//                }
        }
        glEnd();
        glDisable(GL_POINT_SMOOTH);
}

float FVMeshDraw::getLineWidth()
{
        Attr *a = getAttr("Line width");

        if (a != 0)
                return a->value().toFloat();
        else
                return 1.0;

        return 1.0;
}

int FVMeshDraw::getVertexSize( )
{
        Attr *a = getAttr("Vertex Size");

        if (a != 0)
                return a->value().toInt();
        else
                return 4;
}

void FVMeshDraw::paintVertsNums( )
{
//        SetOfInt visVerts( getAttrValue(tr("Interesting Vertices")), 1, grid->_nodes.size() );

//        for ( int n = 0; n < (int) grid->_nodes.size(); n++) {
//                if( ! visVerts.find(n+1) )
//                        continue;

//                getCurrentViewer()->fvRenderText(grid->_nodes[n]._Coords[0], grid->_nodes[n]._Coords[1], grid->_nodes[n]._Coords[2], QString::number( n+1 ) );
//        }

}

void FVMeshDraw::setupAttributes( )
{
//        grid = reqGrid.getGrid( parentObject(), parent );

        qDebug() << "Setting up attributes for " << classType();
        // Here add the attributes
        Attr * a;

        a = am->addSection( tr("Viewing") );
        a = am->addAttr( tr("Solid/Wire"), QString("Solid"), "textlist" );
        QStringList lst;
        lst.append("Solid");
        lst.append("Wireframe");
        lst.append("Elements");
        lst.append("Vertices");
        lst.append("Subdomain wireframe");
        a->setList( lst );

//        a = am->addAttr( tr("Shrink Elems"), QString("1.0"), "text" );
//        a = am->addAttr( tr("Vertex Size"), QString("4"), "text" );
//        a = am->addAttr( tr("Line width"), QString("1.0"), "text" );

//        a = am->addAttr( tr("Transparent"), QString("No"), "boolean" );
//        a = am->addAttr( tr("Transparency Ratio"), fvsettings.value("/RSoft/FViewer/DefTransparencyRatio","0.4").toString(), "text" );

//        a = am->addSection( tr("Colors") );
//        a = am->addAttr( tr("Sub.Color Policy"), QString("Single Color"), "textlist" );
//        lst.clear();
//        lst.append("Single Color");
//        lst.append("Individual Colors");
//        a->setList( lst );

//        QVariant v;
//        QColor defColor;
//        v = fvsettings.value( QString("/RSoft/FViewer/SingleColor"), QVariant(QColor(200,200,200)) );
//        defColor = v.value<QColor>();
//        a = am->addAttr( tr("Single Color"), defColor, "color" );

//        if (grid != 0) {

//                for (unsigned int i = 0; i < grid->_subdomains.size(); i++) {
//                        v = fvsettings.value( QString("/RSoft/FViewer/Color%1").arg(i+1), QVariant(QColor(100,100,100)) );
//                        defColor = v.value<QColor>();
//                        a = am->addAttr( tr("Subdomain %1").arg(i+1), defColor, "color" );
//                }


//                a = am->addSection( tr("Visibility") );
//                a = am->addAttr( tr("Visible Subdomains"), tr("%1-%2").arg(1).arg(grid->_subdomains.size()), "text" );
//                a = am->addAttr( tr("Show Elems Nums"), QString("No"), "boolean" );
//                a = am->addAttr( tr("Interesting Elements"), tr("%1-%2").arg(1).arg(grid->getNoElems()), "text" );
//                a = am->addAttr( tr("Show Verts Nums"), QString("No"), "boolean" );
//                a = am->addAttr( tr("Interesting Vertices"), tr("%1-%2").arg(1).arg(grid->getNoNodes()), "text" );

//        }
}

/*
void FVMeshDraw::slotMousePress( QMouseEvent * )
{
        qDebug() << "FVMeshDraw has received the MousePress event.";
}
*/

void FVMeshDraw::setupMenu( )
{
        contextMenuObj->clear();
        /*
        contextMenuObj->addAction(tr("&Connect interaction"), this, SLOT( slotConnect() ) );
        contextMenuObj->addAction(tr("&Disconnect interaction"), this, SLOT(  slotDisconnect() ) );
        */
        contextMenuObj->addSeparator();
        contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );

}

/*
void FVMeshDraw::slotConnect( )
{
        if (fvinteraction == 0) {
                fvinteraction = new FVInteractionModel(this);
                connect (fvinteraction, SIGNAL( signalMousePress( QMouseEvent * ) ), this, SLOT( slotMousePress( QMouseEvent * ) ) );
        }
}

void FVMeshDraw::slotDisconnect( )
{
        if (fvinteraction != 0) {
                delete fvinteraction;
                fvinteraction = 0;
        }
}
*/


