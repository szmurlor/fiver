//
// C++ Implementation: fvboxgrid
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fvboxgrid.h"
#include "fvgriddraw.h"
#include "fvboundbox.h"
#include "fvgridbnd.h"
#include <fivermain.h>
#include <main.h>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QtDebug>
#include <fvgridinterface.h>
#include <fvopenertextfield.h>
#include <fvhelpers.h>
#include <fvharmonicfieldbox.h>
#include <diffreader.h>

FVBoxGrid::FVBoxGrid( FVBoxMgr * manager,  Grid * g, int x, int y )
: FVObject(manager,x,y)
{
	fvGridInterface = new FVGridInterface( &grid);
	grid = g;
	
	cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_GRID ).value<QColor>();

	setupAttributes();
	setupMenu();
	
    rRect.setWidth( 150 );
}


FVBoxGrid::~FVBoxGrid()
{
	delete fvGridInterface;
	delete grid;
}

void FVBoxGrid::setupAttributes( )
{
	Attr * a;
	a = am->addAttr( tr("Type"), tr("Grid"), QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("Filename"), tr(""), QString("text") );
	a->setEditable(false);
	
	a = am->addAttr( tr("No. Nodes"), QString("%1").arg( grid->getNoNodes() ) , QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("No. Elems"), QString("%1").arg( grid->getNoElems() ), QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("No. of Space Dim."), QString("%1").arg( grid->NumberOfSpaceDim ), QString("text") );
	a->setEditable(false);

}

void FVBoxGrid::slotDraw( )
{
        FVGridDraw * gd = new FVGridDraw( manager, this );
	addChild( gd );
        gd->update();
}

void FVBoxGrid::slotDrawSolid()
{
        FVGridDraw * gd = new FVGridDraw( manager, this );
        addChild( gd );
        gd->setAttrValue( tr("Solid/Wire"), tr("Solid") );
        gd->update();
}

void FVBoxGrid::slotDrawWireframe()
{
        FVGridDraw * gd = new FVGridDraw( manager, this );
        addChild( gd );
        gd->setAttrValue( tr("Solid/Wire"), tr("Wireframe") );
        gd->update();
}

void FVBoxGrid::slotDrawElements()
{
        FVGridDraw * gd = new FVGridDraw( manager, this );
        addChild( gd );
        gd->setAttrValue( tr("Solid/Wire"), tr("Elements") );
        gd->update();
}

void FVBoxGrid::slotDrawVertices()
{
        FVGridDraw * gd = new FVGridDraw( manager, this );
        addChild( gd );
        gd->setAttrValue( tr("Solid/Wire"), tr("Vertices") );
        gd->update();
}

void FVBoxGrid::slotDrawSubdomainWireframe()
{
        FVGridDraw * gd = new FVGridDraw( manager, this );
        addChild( gd );
        gd->setAttrValue( tr("Solid/Wire"), tr("Subdomain wireframe") );
        gd->update();
}

void FVBoxGrid::slotNewHarmonicField()
{
    FVHarmonicFieldBox * box = new FVHarmonicFieldBox(manager,this, this->childSuggestedX(), this->childSuggestedY());
    manager->addObj(box);
    manager->addCon(this, box, tr(""), tr(""));
    manager->update();
}

void FVBoxGrid::slotLoadField()
{
    QStringList files;
    QString fname;
    QString selectedFilter;

    QMap< QString, FVOpener* > filters;
    filters[tr("1 - Text field file (*.*)")] =  new FVOpenerTextField();

    //////////////////////////////////////////////////////
    // Show Dialog to Open File
    files = FVHelpers::openFiles(filters, selectedFilter);
    ///////////////////////////////////////////////////////


    foreach (fname, files) {
        FVOpener *opener = filters[selectedFilter];
        if (opener != 0) {
            FVObject* box = opener->open(manager, fname, 0);
            if (box != 0) {
                FVFieldInterface* fi= (FVFieldInterface*)box->getInterface(QString("FVFieldInterface"));
                if( fi != 0 && fi->getField() != 0) {
                    Field *f= fi->getField();
                    if( f->size() == grid->getNoNodes() || f->size() == grid->getNoElems() ) {
                        manager->addCon(this, box, tr(""), tr(""));
                        manager->autoArrangeChildren(this);
                    } else {
                        QMessageBox::warning(manager,"Loading text field", tr("I have encountered an error processing text field from file: %1.\nField size is not compatibe with the grid size.\nSee diagnostic messages to verify the problem.").arg(fname));
                        // ??? delete box;
                    }
                } else {
                    QMessageBox::warning(manager,"Loading text field", tr("I have encountered an error processing text field from file: %1.\nIt does not contain a valid field.\nSee diagnostic messages to verify the problem.").arg(fname));
                    // ??? delete box;
                }
            } else {
                QMessageBox::warning(manager,"Loading text field", tr("I have encountered an error processing text field from file: %1. See diagnostic messages to verify the problem.").arg(fname));
            }
        } else {
            QMessageBox::warning(manager,"Loading text field", tr("You must selected proper filter to point the expected file format."));
        }
    }
}



void FVBoxGrid::slotDrawBoundaries( )
{
	FVGridBnd * bb = new FVGridBnd( manager, this );
	addChild( bb );
	bb->update();
}

void FVBoxGrid::slotDrawBoundingBox( )
{
	FVBoundBox * bb = new FVBoundBox( manager );

	// Initialize the bounding box. This will we just fixed.
	double p1[3],p2[3];
	grid->getBBox(p1,p2);
	bb->setBBox(p1,p2);

	addChild( bb );
	bb->update();
}

void FVBoxGrid::slotGridToSTL()
{
    manager->addGridToSTLExtractor(this);
}

void FVBoxGrid::slotSave( )
{
    DpReader dpr;
    qDebug() << "FVBoxGrid::slotSave";

    QString startDir (fvsettings.value("/RSoft/FViewer/RecentDir","~").toString());
    QFileDialog sd(manager);
    sd.setDirectory( startDir );
    sd.setAcceptMode(QFileDialog::AcceptSave);
    QString fname= "";
    if( sd.exec() ) {
        fname = sd.selectedFiles().at(0);
    }
    // sprawdzic rozszerzenie i dokleic .grid
    if ( ! fname.contains(".") || fname.section(".", -1) != "grid" ) {
        fname.append(".grid");
    }
    if (fname.length() > 0) {
        dpr.SaveToFile(grid, fname.toStdString(), NULL );


        qDebug() << "Saving: " << fname;
        fvsettings.setValue("/RSoft/FViewer/RecentDir",fname);
        fvsettings.sync();
    } else {
        QMessageBox::warning(manager,tr("Grid save warning."), tr("Grid NOT saved."));
    }
}

void FVBoxGrid::setupMenu( )
{
	contextMenuObj->clear();

    QMenu * aMenu = contextMenuObj->addMenu( tr("&Manipulator") );
    aMenu->addAction(tr("&Rotation"), this, SLOT( slotRotateManipulator() ));
    aMenu->addAction(tr("&Translation"), this, SLOT( slotTranslateManipulator() ));
    aMenu->addSeparator();
    aMenu->addAction( tr("Re&move"), this, SLOT( slotRemoveManipulator() ));

	contextMenuObj->addAction(tr("&Draw"), this, SLOT( slotDraw() ) );
    contextMenuObj->addAction(tr("&Draw Solid"), this, SLOT( slotDrawSolid() ) );
    contextMenuObj->addAction(tr("&Draw Wireframe"), this, SLOT( slotDrawWireframe() ) );
    contextMenuObj->addAction(tr("&Draw Elements"), this, SLOT( slotDrawElements() ) );
    contextMenuObj->addAction(tr("&Draw Vertices"), this, SLOT( slotDrawVertices() ) );
    contextMenuObj->addAction(tr("&Draw Subdomain wireframe"), this, SLOT( slotDrawSubdomainWireframe() ) );
    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("Draw &Boundaries"), this, SLOT(  slotDrawBoundaries() ) );
	contextMenuObj->addAction(tr("Draw B&oundig Box"), this, SLOT(  slotDrawBoundingBox() ) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("Load &field"), this, SLOT(  slotLoadField() ) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("New &harmonic field"), this, SLOT(  slotNewHarmonicField()) );
    contextMenuObj->addAction(tr("Extract STL"), this, SLOT(  slotGridToSTL()) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("&Save"),this, SLOT(slotSave()) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );
}

FVInterface * FVBoxGrid::getInterface( QString interfaceName )
{
	if (interfaceName == QString("FVGridInterface"))
		return fvGridInterface;
	
	return parentInterface( interfaceName );;
}

void FVBoxGrid::slotRotateManipulator( )
{
    if (fvinteraction != 0) {
            delete fvinteraction;
    }
    double p[3];
    getGrid()->getCenter(p);
    double length, size;
    double bb[3],b1[3],b2[3];
    getGrid()->getBBox(b1,b2);
    getGrid()->getCenter(p);
    SUB(bb,b2,b1);
    size = bb[0];
    size = size > bb[1] ? size : bb[1];
    size = size > bb[2] ? size : bb[2];

    if(size == 0.0)
        size = 1.0;

    length = (size * 1.5)/2.0;
    size = (size * 1.5)/2.0;

    fir = new FVInteractionRotation(this, p, size, length);
    fvinteraction = fir;
    connect(fir, SIGNAL(signalRotate(double, double, double,
                                     double, double, double, double)),
            this,    SLOT(slotRotate(double, double, double,
                                     double, double, double, double )) );
    getCurrentViewer()->setInteractionModel(fir);
    update();
}

void FVBoxGrid::slotTranslateManipulator( )
{
        if (fvinteraction != 0) {
                delete fvinteraction;
        }

        double length, size, w, h;
        double bb[3],b1[3],b2[3];
        double p[3], n[3], u[3], v[3];
        getGrid()->getBBox(b1,b2);
        getGrid()->getCenter(p);
        SUB(bb,b2,b1);

        ZERO(n);
        ZERO(u);
        ZERO(v);
        n[2] = 1.0;
        length = bb[2];
        u[0] = 1.0;
        w = bb[0];
        v[1] = 1.0;
        h = bb[1];

        size = w > h ? w : h;

        if(size == 0.0)
            size = 1.0;
        if(length == 0.0)
            length = 1.0;

        size = (size * 1.1)/2.0;
        length = (length * 1.5)/2.0;
        fit = new FVInteractionTranslation(this, p, n, u, v, length, size);
        fvinteraction = fit;
        connect(fit, SIGNAL(signalTranslate(double, double, double)), this, SLOT(slotTranslate(double, double, double)) );
        getCurrentViewer()->setInteractionModel(fit);
        update();
}

void FVBoxGrid::slotRotate( double px, double py, double pz,
                            double ax, double ay, double az, double angle){
        qDebug() << "slotRotate:"
                 << "c(" << pz << "," << py << "," << pz << "), "
                 << "a(" << ax << ", " << ay << ", " << az << "), "
                 << "@" << angle;
        double p[3] = {px, py, pz};
        double a[3] = {ax, ay, az};
        getGrid()->rotate(p, a, angle);
        manager->sendMessage( QString("update"), this, true );
}

void FVBoxGrid::slotTranslate( double dx, double dy, double dz)
{
        qDebug() << "slotTranslate: "
                 << "dx = " << dx << " dy = " << dy << "dz = " << dz;
        double v[3];
        //readAttributes();
        v[0] = dx;
        v[1] = dy;
        v[2] = dz;
        getGrid()->translate(v);
        manager->sendMessage( QString("update"), this, true );
}

void FVBoxGrid::slotRemoveManipulator( )
{
        if (fvinteraction != 0) {
                delete fvinteraction;
                fvinteraction = 0;
                fit = 0;
                getCurrentViewer()->setInteractionModel( getInteractionModel() );
        }
}

Grid * FVBoxGrid::getGrid( )
{
        return grid;
}
