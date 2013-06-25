//
// C++ Implementation: fvboxmesh
//
// Description:
//
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fvboxmesh.h"
#include "fvmeshdraw.h"
#include <fivermain.h>
#include <main.h>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QtDebug>
#include <fvgridinterface.h>
#include <fvboxmeshfunction.h>
#include <mesh/MeshData.h>
#include <mesh/MeshFunction.h>
#include <fvopenerxmlfield.h>
#include <fvhelpers.h>
#include <fvfieldwizard.h>
#include <configreader.h>
#include <fvboundbox.h>

#include <dolfin/function/Function.h>
#include <dolfin/function/FunctionSpace.h>
#include <dolfin/mesh/Mesh.h>
#include <dolfin/fem/FiniteElement.h>
#include <dolfin/fem/DofMap.h>
#include <dolfin/la/GenericVector.h>
#include <dolfin/common/Array.h>
#include <dolfin/io/XMLFile.h>
#include <dolfin/io/XMLMesh.h>
#include <dolfin/mesh/MeshData.h>
#include <dolfin/io/XMLMeshFunction.h>
#include <dolfin/io/pugixml.hpp>

typedef std::map<std::string, boost::shared_ptr<dolfin::MeshFunction<uint> > >::const_iterator mf_const_iterator;

FVBoxMesh::FVBoxMesh( FVBoxMgr * manager,  dolfin::Mesh * m, int x, int y )
: FVObject(manager,x,y)
{
        fvGridInterface = new FVGridInterface( m);
        mesh = m;

//        bmesh=0;
        time_t tstart;
        tstart = time(0);
        std::cout << "Boundary mesh computation started..." << std::endl;
        bmesh = new dolfin::BoundaryMesh(*mesh);
        fvBoundaryMeshInterface = new FVGridInterface(bmesh);
        std::cout << "Boundary mesh computation ended. Took " << time(0) - tstart << " seconds"<< std::endl;

        cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_GRID ).value<QColor>();

        setupAttributes();
        setupMenu();

        rRect.setWidth( 150 );

        std::map<std::string, boost::shared_ptr<dolfin::MeshFunction<uint> > > mf = mesh->data().getMeshFunctions();

        std::cout << std::endl;
        for (mf_const_iterator it = mf.begin(); it != mf.end(); ++it){
            std::cout << it->first << " (size = " << it->second->size() << ")" << std::endl;
            FVBoxMeshFunction * mf = new FVBoxMeshFunction( manager, this, it->first );
            mf->getMeshFunFromMesh();
            addChild( mf );
            mf->update();
        }
}


FVBoxMesh::~FVBoxMesh()
{
    delete fvGridInterface;
    if (bmesh != 0){
        delete bmesh;
        delete fvBoundaryMeshInterface;
    }
    delete mesh;
}

void FVBoxMesh::setupAttributes( )
{
        Attr * a;
        a = am->addAttr( tr("Type"), tr("Mesh"), QString("text") );
        a->setEditable(false);
        a = am->addAttr( tr("Filename"), tr(""), QString("text") );
        a->setEditable(false);

        a = am->addAttr( tr("No. Nodes"), QString("%1").arg( mesh->num_vertices() ) , QString("text") );
        a->setEditable(false);
        a = am->addAttr( tr("No. Elems"), QString("%1").arg( mesh->num_entities(3) ), QString("text") );
        a->setEditable(false);
        a = am->addAttr( tr("No. of Space Dim."), QString("%1").arg( mesh->geometry().dim() ), QString("text") );
        a->setEditable(false);
}

void FVBoxMesh::slotDraw( )
{
        FVMeshDraw * md = new FVMeshDraw( manager, this );
        addChild( md );
        md->update();
//    std::cout << "FVBoxMesh::slotDraw: mesh->geometry().size(): " << mesh->geometry().size() <<  std::endl;
//    std::cout << "FVBoxMesh::slotDraw: mesh->num_vertices(): " << mesh->num_vertices() <<  std::endl;
//    std::cout << "FVBoxMesh::slotDraw: fvGridInterface->getMesh()->str(false)" << fvGridInterface->getMesh()->str(false) << std::endl;
//    std::cout << "FVBoxMesh::slotDraw: fvGridInterface->getMesh()->geometry().size(): " << fvGridInterface->getMesh()->geometry().size() <<  std::endl;
//    std::cout << "FVBoxMesh::slotDraw: fvGridInterface->getMesh()->num_vertices(): " << fvGridInterface->getMesh()->num_vertices() <<  std::endl;
//    std::cout <<"a mesh wskazuje na adres: " << mesh << std::endl;
//    std::cout << "geometry" << mesh->geometry().str(true) << std::endl;
//    std::cout << "topology" << mesh->topology().str(true) << std::endl;
//    std::cout << "data" << mesh->data().str(true) << std::endl;
}

void FVBoxMesh::slotDrawSolid()
{
        FVMeshDraw * md = new FVMeshDraw( manager, this );
        addChild( md );
        md->setAttrValue( tr("Solid/Wire"), tr("Solid") );
        md->update();
}

void FVBoxMesh::slotDrawWireframe()
{
        FVMeshDraw * md = new FVMeshDraw( manager, this );
        addChild( md );
        md->setAttrValue( tr("Solid/Wire"), tr("Wireframe") );
        md->update();
}

void FVBoxMesh::slotDrawElements()
{
//        FVGridDraw * gd = new FVGridDraw( manager, this );
//        addChild( gd );
//        gd->setAttrValue( tr("Solid/Wire"), tr("Elements") );
//        gd->update();
}

void FVBoxMesh::slotDrawVertices()
{
        FVMeshDraw * md = new FVMeshDraw( manager, this );
        addChild( md );
        md->setAttrValue( tr("Solid/Wire"), tr("Vertices") );
        md->update();
}

void FVBoxMesh::slotDrawSubdomainWireframe()
{
//        FVGridDraw * gd = new FVGridDraw( manager, this );
//        addChild( gd );
//        gd->setAttrValue( tr("Solid/Wire"), tr("Subdomain wireframe") );
//        gd->update();
}

void FVBoxMesh::slotNewHarmonicField()
{
//    FVHarmonicFieldBox * box = new FVHarmonicFieldBox(manager,this, this->childSuggestedX(), this->childSuggestedY());
//    manager->addObj(box);
//    manager->addCon(this, box, tr(""), tr(""));
//    manager->update();
}

void FVBoxMesh::slotLoadField()
{
    QStringList files;
    QString fname;
    QString selectedFilter;

    QMap< QString, FVOpener* > filters;
    filters[tr("1 - Xml field file (*.xml*)")] =  new FVOpenerXmlField();

    //////////////////////////////////////////////////////////
    //Read config file
    //Read config file
    ConfigReader* cr = new ConfigReader();
    cr->ReadConfig();
//    char *configpath= getenv( "FIVER_XML_FIELD_CONFIG" );
//    cr->ReadFromFile(configpath);
//    cr->cutEndline();
    //////////////////////////////////////////////////////////


    FVFunctionSpaceWizard wizard(cr);
    wizard.exec();

    if (wizard.isFinished){
//        std::cout << "choosen vals: \n" << wizard.fieldType << " | "  << wizard.FEType << " | " << wizard.approxDeg << std::endl;
        dolfin::FunctionSpace* V = wizard.getFunctionSpace(mesh);

        if (wizard.optionExists) {
                FVOpener *opener = new FVOpenerXmlField();
                fname = wizard.fname;
                    FVObject* box = opener->open(manager, fname, 0);
                    dolfin::Function* fun = new dolfin::Function( boost::shared_ptr<const dolfin::FunctionSpace>(V), fname.toStdString() );

                    ////////////////////////////////////////////////////////////////////////
                    /////////////wypisywanie wartości///////////////////////////////////////
#if 0
		    unsigned int dim= fun->value_dimension(0);
                    dolfin::Array<double> val;
                    fun->compute_vertex_values(val,*mesh);
                    std::cout << "dim= " << dim << ", values size: " << val.size() << std::endl;
                    for (int i=0; i< val.size()/dim; i++){
			for( int d= 0; d < dim; d++ )
                            std::cout << val[i+d*val.size()/dim] << " ";
                        std::cout << std::endl;
                    }
#endif
                    ////////////////////////////////////////////////////////////////////////
                    if (box != 0) {
                        ((FVBoxFieldXml*) box)->setVector(wizard.isVector());
                        ((FVBoxFieldXml*) box)->setMesh(mesh);
                        ((FVBoxFieldXml*) box)->setField(fun);
                        ((FVBoxFieldXml*) box)->setAtt();


                        manager->addCon(this, box, tr(""), tr(""));
                        manager->autoArrangeChildren(this);

                    } else {
                        QMessageBox::warning(manager,"Loading field", tr("I have encountered an error processing text field from file: %1. See diagnostic messages to verify the problem.").arg(fname));
                    }
        }
        else {
            QMessageBox::warning(manager,"Loading xml field", tr("Choosen options are not supported."));
        }
    }else {
        std::cout << "Loading field was cancelled" << std::endl;
    }
}

void FVBoxMesh::slotLoadMeshFunction()
{
    QStringList files;
    QString fname;
    QString selectedFilter;

    QMap< QString, FVOpener* > filters;
    filters[tr("1 - Mesh function xml file (*.xml)")] =  new FVOpenerXmlField();

    //////////////////////////////////////////////////////
    // Show Dialog to Open File
    files = FVHelpers::openFiles(filters, selectedFilter);
    ///////////////////////////////////////////////////////


    foreach (fname, files) {
        createMeshFunction<uint>(fname);
//        createMF(fname);

    }
}


void FVBoxMesh::slotDrawBoundaries( )
{
    FVMeshDraw * md = new FVMeshDraw( manager, this );
    addChild( md );
    md->setAttrValue( tr("Solid/Wire"), tr("BoundaryWireframe") );
    md->update();
}

void FVBoxMesh::slotDrawBoundingBox( )
{
        FVBoundBox * bb = new FVBoundBox( manager );

        // Initialize the bounding box. This will we just fixed.
        double p1[3],p2[3];
        FVHelpers::getBBox(mesh,p1,p2);
        bb->setBBox(p1,p2);

        addChild( bb );
        bb->update();
}

void FVBoxMesh::slotGridToSTL()
{
//    manager->addGridToSTLExtractor(this);
}

void FVBoxMesh::slotSave( )
{
//    DpReader dpr;
//    qDebug() << "FVBoxGrid::slotSave";

//    QString startDir (fvsettings.value("/RSoft/FViewer/RecentDir","~").toString());
//    QFileDialog sd(manager);
//    sd.setDirectory( startDir );
//    sd.setAcceptMode(QFileDialog::AcceptSave);
//    QString fname= "";
//    if( sd.exec() ) {
//        fname = sd.selectedFiles().at(0);
//    }
//    // sprawdzic rozszerzenie i dokleic .grid
//    if ( ! fname.contains(".") || fname.section(".", -1) != "grid" ) {
//        fname.append(".grid");
//    }
//    if (fname.length() > 0) {
//        dpr.SaveToFile(grid, fname.toStdString(), NULL );


//        qDebug() << "Saving: " << fname;
//        fvsettings.setValue("/RSoft/FViewer/RecentDir",fname);
//        fvsettings.sync();
//    } else {
//        QMessageBox::warning(manager,tr("Grid save warning."), tr("Grid NOT saved."));
//    }
}

void FVBoxMesh::setupMenu( )
{
    contextMenuObj->clear();

//    QMenu * aMenu = contextMenuObj->addMenu( tr("&Manipulator") );
//    aMenu->addAction(tr("&Rotation"), this, SLOT( slotRotateManipulator() ));
//    aMenu->addAction(tr("&Translation"), this, SLOT( slotTranslateManipulator() ));
//    aMenu->addSeparator();
//    aMenu->addAction( tr("Re&move"), this, SLOT( slotRemoveManipulator() ));

    contextMenuObj->addAction(tr("&Draw"), this, SLOT( slotDraw() ) );
    contextMenuObj->addAction(tr("&Draw Solid"), this, SLOT( slotDrawSolid() ) );
    contextMenuObj->addAction(tr("&Draw Wireframe"), this, SLOT( slotDrawWireframe() ) );
//    contextMenuObj->addAction(tr("&Draw Elements"), this, SLOT( slotDrawElements() ) );
    contextMenuObj->addAction(tr("&Draw Vertices"), this, SLOT( slotDrawVertices() ) );
//    contextMenuObj->addAction(tr("&Draw Subdomain wireframe"), this, SLOT( slotDrawSubdomainWireframe() ) );
    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("Draw &Boundary Wireframe"), this, SLOT(  slotDrawBoundaries() ) );
    contextMenuObj->addAction(tr("Draw B&oundig Box"), this, SLOT(  slotDrawBoundingBox() ) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("Load &field"), this, SLOT(  slotLoadField() ) );
    contextMenuObj->addAction(tr("Load &mesh function"), this, SLOT(  slotLoadMeshFunction() ) );

//    contextMenuObj->addSeparator();
//    contextMenuObj->addAction(tr("New &harmonic field"), this, SLOT(  slotNewHarmonicField()) );
//    contextMenuObj->addAction(tr("Extract STL"), this, SLOT(  slotGridToSTL()) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("&Save"),this, SLOT(slotSave()) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );
}

FVInterface * FVBoxMesh::getInterface( QString interfaceName )
{
        if (interfaceName == QString("FVGridInterface"))
                return fvGridInterface;
        if (interfaceName == QString("FVBoundaryMeshInterface")){
            if  (bmesh == 0 ){
                time_t tstart;
                tstart = time(0);
                std::cout << "Boundary mesh computation started..." << std::endl;
                bmesh = new dolfin::BoundaryMesh(*mesh);
                fvBoundaryMeshInterface = new FVGridInterface(bmesh);
                std::cout << "Boundary mesh computation ended. Took " << time(0) - tstart << " seconds"<< std::endl;
            }
            return fvBoundaryMeshInterface;
        }
        return parentInterface( interfaceName );
}

void FVBoxMesh::slotRotateManipulator( )
{
//    if (fvinteraction != 0) {
//            delete fvinteraction;
//    }
//    double p[3];
//    getGrid()->getCenter(p);
//    double length, size;
//    double bb[3],b1[3],b2[3];
//    getGrid()->getBBox(b1,b2);
//    getGrid()->getCenter(p);
//    SUB(bb,b2,b1);
//    size = bb[0];
//    size = size > bb[1] ? size : bb[1];
//    size = size > bb[2] ? size : bb[2];

//    if(size == 0.0)
//        size = 1.0;

//    length = (size * 1.5)/2.0;
//    size = (size * 1.5)/2.0;

//    fir = new FVInteractionRotation(this, p, size, length);
//    fvinteraction = fir;
//    connect(fir, SIGNAL(signalRotate(double, double, double,
//                                     double, double, double, double)),
//            this,    SLOT(slotRotate(double, double, double,
//                                     double, double, double, double )) );
//    getCurrentViewer()->setInteractionModel(fir);
//    update();
}

void FVBoxMesh::slotTranslateManipulator( )
{
//        if (fvinteraction != 0) {
//                delete fvinteraction;
//        }

//        double length, size, w, h;
//        double bb[3],b1[3],b2[3];
//        double p[3], n[3], u[3], v[3];
//        getGrid()->getBBox(b1,b2);
//        getGrid()->getCenter(p);
//        SUB(bb,b2,b1);

//        ZERO(n);
//        ZERO(u);
//        ZERO(v);
//        n[2] = 1.0;
//        length = bb[2];
//        u[0] = 1.0;
//        w = bb[0];
//        v[1] = 1.0;
//        h = bb[1];

//        size = w > h ? w : h;

//        if(size == 0.0)
//            size = 1.0;
//        if(length == 0.0)
//            length = 1.0;

//        size = (size * 1.1)/2.0;
//        length = (length * 1.5)/2.0;
//        fit = new FVInteractionTranslation(this, p, n, u, v, length, size);
//        fvinteraction = fit;
//        connect(fit, SIGNAL(signalTranslate(double, double, double)), this, SLOT(slotTranslate(double, double, double)) );
//        getCurrentViewer()->setInteractionModel(fit);
//        update();
}

void FVBoxMesh::slotRotate( double px, double py, double pz,
                            double ax, double ay, double az, double angle){
//        qDebug() << "slotRotate:"
//                 << "c(" << pz << "," << py << "," << pz << "), "
//                 << "a(" << ax << ", " << ay << ", " << az << "), "
//                 << "@" << angle;
//        double p[3] = {px, py, pz};
//        double a[3] = {ax, ay, az};
//        getGrid()->rotate(p, a, angle);
//        manager->sendMessage( QString("update"), this, true );
}

void FVBoxMesh::slotTranslate( double dx, double dy, double dz)
{
//        qDebug() << "slotTranslate: "
//                 << "dx = " << dx << " dy = " << dy << "dz = " << dz;
//        double v[3];
//        //readAttributes();
//        v[0] = dx;
//        v[1] = dy;
//        v[2] = dz;
//        getGrid()->translate(v);
//        manager->sendMessage( QString("update"), this, true );
}

void FVBoxMesh::slotRemoveManipulator( )
{
//        if (fvinteraction != 0) {
//                delete fvinteraction;
//                fvinteraction = 0;
//                fit = 0;
//                getCurrentViewer()->setInteractionModel( getInteractionModel() );
//        }
}

dolfin::Mesh * FVBoxMesh::getMesh( )
{
    return mesh;
}

void FVBoxMesh::createMF( QString& fname)
{
    std::string str = fname.toStdString();
    unsigned found = str.find_last_of("/\\");

    std::cout << "plik o nazwie: " << fname.toStdString() << " a found to: " << found << " l: "<< str.length() <<  std::endl;
    std::cout << "wartość str: " << str <<std::endl;
    FVBoxMeshFunction * mf;
    //wczytanie danych z pliku
    dolfin::MeshFunction<unsigned int>* newMF = /*mesh->data().create_mesh_function(str.substr(found+1)).get();
    newMF = */new dolfin::MeshFunction<uint>(*mesh, fname.toStdString());
    std::cout << "FVBoxMesh: wczytano " << newMF->size() << "wartości" << std::endl;
    //dodanie boxa
    if ( found != str.length() )
        mf = new FVBoxMeshFunction( manager, this, str.substr(found+1) );
    else
        mf = new FVBoxMeshFunction( manager, this, "str");
    addChild( mf );
//    mf->setMeshFunction(newMF);
    mf->setAtt();
    mf->update();
}


template <typename T>
void FVBoxMesh::createMeshFunction( QString& fname)
{
    std::string str = fname.toStdString();
    unsigned found = str.find_last_of("/\\");
    FVBoxMeshFunction * mf;
    dolfin::MeshFunction<uint>* newMF = new dolfin::MeshFunction<uint>(*mesh);
    dolfin::XMLFile xmlfile(fname.toStdString());

    pugi::xml_document xml_doc;
    xmlfile.load_xml_doc(xml_doc);
    const pugi::xml_node dolfin_node = xmlfile.get_dolfin_xml_node(xml_doc);
    const pugi::xml_node mesh_fun_node = dolfin_node.child("mesh_function");
    const pugi::xml_node mesh_val_coll = mesh_fun_node.child("mesh_value_collection");
    const std::string mesh_fun_type = mesh_val_coll.attribute("type").value();

    if (mesh_fun_node){
        dolfin::XMLMeshFunction::read(*newMF, mesh_fun_type, dolfin_node);
        //dodanie boxa
        if ( found != str.length() )
            mf = new FVBoxMeshFunction( manager, this, str.substr(found+1) );
        else
            mf = new FVBoxMeshFunction( manager, this, "str");
        addChild( mf );
        mf->setMeshFunction(newMF);
        mf->setAtt();
        mf->update();
    } else {
        std::cerr << "Reading mesh function ended with error" << std::endl;
    }
}
