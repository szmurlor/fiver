/*****************************************************************************
 *   Copyright (C) 2006 Robert Szmurlo <robert@iem.pw.edu.pl>                *
 *                                                                           *
 *   Licensed under the Apache License, Version 2.0 (the "License");         *
 *   you may not use this file except in compliance with the License.        *
 *   You may obtain a copy of the License at                                 *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *   Unless required by applicable law or agreed to in writing, software     *
 *   distributed under the License is distributed on an "AS IS" BASIS,       *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *   See the License for the specific language governing permissions and     *
 *   limitations under the License.                                          *
 *****************************************************************************/

#include "fvboxmgr.h"

#include <QFile>
#include <QTextStream>

#include <fvobject.h>
#include <fvboxgrid.h>
#include <fvboxfield.h>
#include <fvboxsimulation.h>
#include <fvfielddraw.h>
#include <fvfieldslice.h>
#include <fvboxcblock.h>
#include <fvboxsphere.h>
#include <fvboxcoil.h>
#include <fvboxstl.h>
#include <fvmeshjoiner.h>
#include <fvgridtostlextractor.h>


FVBoxMgr::FVBoxMgr(QWidget * parent)
 : BoxMgr(parent)
{
    lastId = 1;
}


FVBoxMgr::~FVBoxMgr()
{
}

FVObject * FVBoxMgr::getParentOf( FVObject * child )
{
	std::list<BoxObj*>::iterator it;
	for (it = objs.begin(); it != objs.end(); it++) {
                if ( (*it)->btType == BoxObj::TCon ) {
			BoxCon * bc = (BoxCon*) (*it);
			if (bc->bdst == child)
				return (FVObject*) bc->bsrc;
		}
	}
	return 0;
}


FVBoxGrid * FVBoxMgr::addBoxGrid( FVObject * parent, Grid *g, QString filename, QString type )
{
	FVBoxGrid * bg;
	if ( parent != 0 )
		bg = new FVBoxGrid(this, g, parent->childSuggestedX(),parent->childSuggestedY());
	else
		bg = new FVBoxGrid(this, g);
    bg->sName = getNameFor("bg");
	bg->sType = type;
	bg->setAttrValue("Filename", filename);
        bg->setAttrValue("Type", type );

	addObj( bg );
	
	if (parent != 0)
		addCon( parent, bg, QString(""), QString("") );

	return bg;
}

FVGridToSTLExtractor * FVBoxMgr::addGridToSTLExtractor(FVBoxGrid *parentBoxGrid)
{
    FVGridToSTLExtractor * stl;
    stl = new FVGridToSTLExtractor(this, parentBoxGrid->childSuggestedX(), parentBoxGrid->childSuggestedY());

    stl->sName = getNameFor("stlext");
    stl->sType = QString("STLExtractor");

    addObj( stl );
    addCon( parentBoxGrid, stl, QString(""), QString("") );

    return stl;
}


FVBoxSTL * FVBoxMgr::addBoxSTL( FVObject * parent, Grid *g, QString filename, QString type )
{
        FVBoxSTL * bstl;
        if ( parent != 0 )
                bstl = new FVBoxSTL(this, g, parent->childSuggestedX(),parent->childSuggestedY());
        else
                bstl = new FVBoxSTL(this, g);

        bstl->sName = getNameFor("bstl");
        bstl->sType = type;
        bstl->setAttrValue("Filename", filename);
        bstl->setAttrValue("Type", type );

        addObj( bstl );

        if (parent != 0)
                addCon( parent, bstl, QString(""), QString("") );

        return bstl;
}

FVBoxCBlock * FVBoxMgr::addBoxCBlock(QString filename, QString type)
{
	qDebug() << "addBoxCblock called";

        FVBoxCBlock *cblock;
        cblock = new FVBoxCBlock(this, 20, 20);
        cblock->sName = getNameFor("cb");
	cblock->sType = type;
	cblock->setAttrValue("Filename", filename);
	cblock->setAttrValue("Type", type );

	qDebug() << "Created Cblock" << cblock;
	addObj( cblock );
	
	return cblock;
}

FVBoxSphere * FVBoxMgr::addBoxSphere( QString type )
{
        qDebug() << "addBoxSphere called";

        FVBoxSphere *s;
        s = new FVBoxSphere(this, 20, 20);
        s->sName = getNameFor("sp");
        s->sType = type;

        qDebug() << "Created Sphere" << s;
        addObj( s );

        s->update();

        return s;
}

FVBoxCoil * FVBoxMgr::addBoxCoil( QString type )
{
        qDebug() << "addBoxCoil called";

        FVBoxCoil *s;
        s = new FVBoxCoil(this, 20, 20);
        s->sName = getNameFor("co");
        s->sType = type;

        qDebug() << "Created Coil" << s;
        addObj( s );

        s->update();

        return s;
}

FVMeshJoiner * FVBoxMgr::addMeshJoiner( QString type )
{
        qDebug() << "addMeshJoiner called";

        FVMeshJoiner *mj;
        mj = new FVMeshJoiner(this, 20, 20);
        mj->sName = getNameFor("mj");
        mj->sType = type;

        qDebug() << "Created MeshJoiner" << mj;
        addObj( mj );

        mj->update();

        return mj;
}

FVBoxSimulation * FVBoxMgr::addBoxSimulation( QString filename, QString type )
{
	FVBoxSimulation * bs = new FVBoxSimulation(this);
	bs->sName = filename;
	bs->sType = type;
	bs->setAttrValue("Filename", filename);
	bs->setAttrValue("Type", type );
	
	addObj( bs );
	
	return bs;
}

FVBoxField * FVBoxMgr::addBoxField( Field * f, QString name, QString type )
{
    FVBoxField * bf;
    bf = new FVBoxField(this, f);

    bf->sName = name;
    bf->sType = type;
    bf->setAttrValue("Name", name);
    bf->setAttrValue("Type", type );

    addObj( bf );

    bf->update();

    return bf;

}

FVBoxField * FVBoxMgr::addBoxField( FVObject* parent, Field * f, QString name, QString type )
{
	FVBoxField * bf;
	if ( parent != 0 )
		bf = new FVBoxField(this, f, parent->childSuggestedX(),parent->childSuggestedY());
	else
		bf = new FVBoxField(this, f);

	bf->sName = name;
	bf->sType = type;
	bf->setAttrValue("Name", name);
	bf->setAttrValue("Type", type );
	
	addObj( bf );
	
	if (parent != 0)
		addCon( parent, bf, QString(""), QString("") );

	bf->update();
	
	return bf;
	
}

void FVBoxMgr::autoArrangeChildren( FVObject * parent )
{
	std::list<BoxBox*> children;
	std::list<BoxBox*>::iterator child;

	int lastY = 0;
	int lastHeight = 0;
	children = childrenOfBoxObj( (BoxBox*) parent );
	for (child = children.begin(); child != children.end(); child++) {
		if (lastY == 0)
			lastY = parent->childSuggestedY();
		else
			lastY = lastY + lastHeight + 5;

		(*child)->rRect.moveTo( parent->childSuggestedX(), lastY );
		lastHeight = (*child)->rRect.height();
	}
	for (child = children.begin(); child != children.end(); child++) {
		autoArrangeChildren( (FVObject*) *child );
	}
}

void FVBoxMgr::sendMessageToChildrenOf( const QList<QString> &argv, FVObject * receiver )
{
    std::list< BoxBox *> children = childrenOfBoxObj( (BoxBox*) receiver );
    std::list< BoxBox *>::iterator it;
    for (it = children.begin(); it != children.end(); it++)
        sendMessage(argv, (FVObject*) (*it), true);
}

void FVBoxMgr::sendMessage( const QList<QString> &argv, FVObject * receiver, bool populateToChildren )
{
	if (receiver != 0) {
        // qDebug()<< "Passing message: " << argv << " to " << receiver->sName;
		receiver->messageHandler( argv );
        // qDebug()<< "Finished passing message: " << argv << " to " << receiver->sName;
	}
	
	if (populateToChildren) {
        // qDebug()<< "Iterating over children of : " << receiver->sName;
		std::list< BoxBox *> children = childrenOfBoxObj( (BoxBox*) receiver );
		std::list< BoxBox *>::iterator it;
		for (it = children.begin(); it != children.end(); it++) 
			sendMessage(argv, (FVObject*) (*it), populateToChildren);
	}
}

void FVBoxMgr::sendMessage( const QString & arg, FVObject * receiver, bool populateToChildren )
{
	QList< QString > argv;
	argv.append( arg );
	
    //qDebug()<< "Sending message: " << argv;
	sendMessage( argv, receiver, populateToChildren );
    //qDebug()<< "Finished sending message: " << argv;
}

FVObject * FVBoxMgr::find( QString name )
{
        std::list<BoxObj*>::iterator it;
        for (it = objs.begin(); it != objs.end(); it++) {
            qDebug() << "\""<< (*it)->sName << "\"";
                if ( (*it)->sName == name &&
                     (*it)->btType == BoxObj::TBox ) {
                        return (FVObject*)(*it);
                }
        }
        return 0;
}

FVObject * FVBoxMgr::find( QString type, QString name )
{
        std::list<BoxObj*>::iterator it;
        for (it = objs.begin(); it != objs.end(); it++) {            
                if ( (*it)->sName == name &&
                     (*it)->sType == type &&
                     (*it)->btType == BoxObj::TBox ) {
                        return (FVObject*)(*it);
                }
        }
        return 0;
}

FVObject * FVBoxMgr::find( int )
{
    // Not implemented yet
    return 0;
}

QString FVBoxMgr::createScript(QString filename)
{
    QString script("# Script generated by fiver\n");
    for (list<BoxObj*>::iterator it = objs.begin(); it != objs.end(); it++) {
        FVObject *o = (FVObject*)*it;
        script += o->dump();
        script += "\n";
    }
    filename = filename.trimmed();
    QFile f(filename);
    QTextStream fin(&f);
    if(f.open(QFile::WriteOnly)) {
        fin << script;
        return QString("OK");
    } else {
        return QString("Could not write to file");
    }
}

QString FVBoxMgr::getNameFor(QString s)
{
    return QString(s + QString::number(lastId++));
}
