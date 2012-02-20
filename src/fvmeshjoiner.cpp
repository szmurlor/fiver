#include "fvmeshjoiner.h"

#include <main.h>
#include <QtDebug>
#include <fvboxgrid.h>
#include <iostream>

FVMeshJoiner::FVMeshJoiner(FVBoxMgr * manager, int x, int y)
    : FVObject(manager,x,y)
{
    sName = "MeshJoiner";
    sType = "MeshJoiner";
    cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_ANIMATION ).value<QColor>();

    mj = new MeshJoiner();
    sFlags = QString(mj->flags.c_str());
    existExternal = false;
    setupAttributes();
    setupMenu();
}

FVMeshJoiner::~FVMeshJoiner()
{
}

void FVMeshJoiner::setupAttributes( )
{
    Attr * a;
    a = am->addAttr( tr("Type"), sType, QString("text") );
    a->setEditable(false);
    a = am->addAttr( tr("Name"), sName, QString("text") );
    a->setEditable(false);

    a = am->addAttr( tr("Flags"), sFlags, QString("text") );
    a->setEditable(false);

    a = am->addAttr( tr("STL's Dividing"), QString("No"), "textlist" );
    QStringList lst;
    lst.append("No");
    lst.append("Yes");
    lst.append("External Only");
    a->setList( lst );

    a = am->addAttr( tr("Radius-Edge Ratio"), QString("1.8"), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("Mesh Size"), QString("0.5"), QString("text") );
    a->setEditable(true);

}
void FVMeshJoiner::updateAttributes( )
{
    QString flags; // = getAttrValue( tr("Flags") );
    flags.append("pq");
    flags.append(getAttrValue( tr("Radius-Edge Ratio")));
    flags.append("a");
    flags.append(getAttrValue( tr("Mesh Size")));
    if ( getAttrValue( tr("STL's Dividing")) == QString("No"))
        flags.append("Y");
    sFlags = QString(flags);
    setAttrValue(tr("Flags"), sFlags);
    mj->flags.clear();
    mj->flags.append( string(flags.toStdString()));
    cout << "zmieniono flagi na: " << sFlags.toStdString() << endl;
    update();
    manager->sendMessage(QString("update"), this, true );
}

void FVMeshJoiner::setupMenu( )
{
    contextMenuObj->clear();

    contextMenuObj->addAction(tr("Add &External"), this, SLOT( slotExternal() ) );
    contextMenuObj->addAction(tr("Add &Hole"), this, SLOT( slotHole() ) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("Join &Meshes"), this, SLOT( slotJoin() ) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("Delete &Children"),this, SLOT( slotDeleteChildren()) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );
}

void FVMeshJoiner::slotExternal( )
{
    if ( existExternal ){
        //remove connection to external mesh
        manager->removeExternalCon( this );
    }
    existExternal = false;

    //add external
    if (manager->connectSTLChild(this,true) == 0 )
        existExternal = true;

    std::cout << "slot external execute!!!!" << endl;
}

void FVMeshJoiner::slotHole( )
{
    manager->connectSTLChild(this,false);
    std::cout << "slot hole execute!!!!" << endl;
}

void FVMeshJoiner::slotJoin( )
{
    FVBoxGrid* fg;
    Grid* newGrid;

    //zewnętrzna siatka
    BoxBox* ext = manager->externalForMeshJoiner((BoxBox*) this);
    if (ext == NULL ){
        qWarning("There is no grid to fill!");
        return;
    }
    mj->addGrid(((FVBoxGrid*)ext)->getGrid());

    //czytanie "dziur"
    std::list< BoxBox *> children = manager->holesForMeshJoiner((BoxBox*) this );
    std::list< BoxBox *>::iterator it;
    for (it = children.begin(); it != children.end(); it++) {
        fg = (FVBoxGrid*) (*it);
//        mj->gridAll = fg->getGrid();
        mj->addHole(fg->getGrid());

    }

    if (getAttrValue( tr("STL's Dividing")) == QString("External Only")){
        newGrid = mj->joinMeshes(true);
    } else {
        newGrid = mj->joinMeshes(false);
    }
    manager->addBoxGrid(NULL,newGrid,tr("TetGen - Generated"),tr("DiffpackGrid"));



    std::cout << "slot join execute!!!!" << endl;
}

void FVMeshJoiner::slotDeleteChildren( )
{
    manager->removeAllCon( this );
    existExternal = false;
    std::cout << "slot delete children execute!!!!" << endl;
}

