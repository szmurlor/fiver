#include "fvboxfieldxml.h"
#include <main.h>
#include <QtDebug>
#include <QDir>
#include <QMessageBox>
//#include <fvboxgrid.h>
#include <fvfielddrawxml.h>
#include <fvfieldslicexml.h>
//#include <field.h>
#include <fvanimation.h>

FVBoxFieldXml::FVBoxFieldXml(FVBoxMgr * manager, dolfin::Function * f, int x, int y)
 : FVObject(manager, x ,y )
{
    fvOpener = 0;
    fvFieldInterfaceXml = new FVFieldInterfaceXml( field );
    field = f;

    cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_FIELD ).value<QColor>();

    isVector = false;

    setupAttributes();
    setupMenu();

    rRect.setWidth( 150 );
}


FVBoxFieldXml::~FVBoxFieldXml()
{
    delete fvFieldInterfaceXml;
    delete fvOpener;
}

void FVBoxFieldXml::setupAttributes( )
{
        am->clear();

        Attr * a;
        a = am->addAttr( tr("Type"), tr("Field"), QString("text") );
        a->setEditable(false);
        QString name = tr("");
        if (field != NULL )
            name = tr(field->name().c_str());
        a = am->addAttr( tr("Name"), name , QString("text") );
        a->setEditable(false);

        setupAttributesMinMax();
}

void FVBoxFieldXml::setupAttributesMinMax( )
{
        Attr * a;
        int valno=0;
        double min=0;
        double max=0;


        if ( this->mesh != NULL && field != NULL){
            dolfin::Array<double> val;
            field->compute_vertex_values(val,*mesh);
            valno = val.size();
            min = val.min();
            max = val.max();
        }

        a = am->updateAttr( tr("Values No."), QString("%1").arg( valno ) , QString("text") );
        a->setEditable(false);
        a = am->updateAttr( tr("Min. Value"), QString("%1").arg( min ), QString("text") );
        a->setEditable(false);
        a = am->updateAttr( tr("Max. Value"), QString("%1").arg( max ), QString("text") );
        a->setEditable(false);
}

void FVBoxFieldXml::setupMenu( )
{
        contextMenuObj->clear();


        if (field != NULL ) {
            dolfin::Array<double> val;
            field->compute_vertex_values(val,*mesh);
            if (isVector){
                contextMenuObj->addAction(tr("&Draw Vectors"), this, SLOT( slotDrawVectors() ) );
                QString msg("Xml Field " + this->getName() + " may be drawn as vectors" );
                qDebug(msg.toAscii());
            } else {
                QString msg("Xml Field" + this->getName() + " will not be drawn as vectors");
                qDebug(msg.toAscii());
            }
        }
        contextMenuObj->addAction(tr("&Draw Colormap"), this, SLOT( slotDrawColormap() ) );

        contextMenuObj->addSeparator();
        contextMenuObj->addAction(tr("&Add Slice"), this, SLOT( slotSlice() ) );

        contextMenuObj->addSeparator();
        contextMenuObj->addAction(tr("&Animate Visualisation"), this, SLOT( slotAnimate() ) );

        contextMenuObj->addSeparator();
        contextMenuObj->addAction(tr("&Reload"), this, SLOT( slotReload() ) );

        contextMenuObj->addSeparator();
        contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );
}

void FVBoxFieldXml::slotDrawColormap( )
{
        FVFieldDrawXml * fd = new FVFieldDrawXml( manager, this );
        addChild(fd);
        fd->update();
}

void FVBoxFieldXml::slotDrawVectors( )
{
        FVFieldDrawXml * fd = new FVFieldDrawXml(manager, this );
        qDebug("FVFieldDraw created");
        fd->setAttrValue( QString("View Style"), QString("vectors") );
        qDebug("FVFieldDraw set up as vectors");
        addChild( fd );
        qDebug("FVFieldDraw added as child");
        fd->update();
        qDebug("FVFieldDraw updated");
}

void FVBoxFieldXml::slotSlice( )
{
        FVFieldSliceXml * fs = new FVFieldSliceXml(manager);
        addChild( fs );
        fs->slice();
        fs->update();
}

void FVBoxFieldXml::slotReload( )
{
    if (getOpener() != 0) {
        getOpener()->reload(this);
        update();
        setupAttributesMinMax();

        QStringList argv;
        argv.append( QString("update") );
        manager->sendMessageToChildrenOf(argv, this);
    } else {
        QMessageBox::information(parent, tr("B³¹d odwie¿ania"),tr("Ten komponent nie wspiera odwie¿ania."));
    }
}

FVInterface * FVBoxFieldXml::getInterface( QString interfaceName )
{
        if (interfaceName == QString("FVFieldInterfaceXml")){
            return fvFieldInterfaceXml;
        }
        return parentInterface(interfaceName);
}

void FVBoxFieldXml::slotAnimate( )
{
        FVAnimation * fa = new FVAnimation(manager);
        fa->addParent(this);
        fa->initAttributes();
        fa->update();
}

QString FVBoxFieldXml::getName() {
    QString name= sName;
    QString reg( "^.*" );
    reg.append(QDir::separator());
    name.remove( QRegExp(reg) );
    return name;
};

void FVBoxFieldXml::update( )
{
//        sName = /*field->getAttr("name").c_str()*/ field->name().c_str();
//        setAttrValue( QString("Name"), /*field->getAttr("name").c_str()*/ field->name().c_str());
}

bool FVBoxFieldXml::message( const QList< QString > & argv )
{
        if (argv.size() > 0) {
                if (argv[0] == QString("setTime") ) {
                        double tv = argv[1].toDouble();
//                        field->setCurTime( tv );
                        return true;
                }
        }

        return true;
}
