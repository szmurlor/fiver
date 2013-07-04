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

#include "fvboxfield.h"
#include <main.h>
#include <QtDebug>
#include <QDir>
#include <QMessageBox>
#include <fvboxgrid.h>
#include <fvfielddraw.h>
#include <fvfieldslice.h>
#include <field.h>
#include <fvanimation.h>

FVBoxField::FVBoxField(FVBoxMgr * manager, Field * f, int x, int y)
 : FVObject(manager, x ,y )
{
    fvOpener = 0;
    fvFieldInterface = new FVFieldInterface( &field );
    field = f;

    cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_FIELD ).value<QColor>();

    setupAttributes();
    setupMenu();

    rRect.setWidth( 150 );
}


FVBoxField::~FVBoxField()
{
	delete fvFieldInterface;
	delete field;
}

void FVBoxField::setupAttributes( )
{
	am->clear();
	
	Attr * a;
	a = am->addAttr( tr("Type"), tr("Field"), QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("Name"), tr(""), QString("text") );
	a->setEditable(false);
	
        setupAttributesMinMax();
}

void FVBoxField::setupAttributesMinMax( )
{
        Attr * a;

        a = am->updateAttr( tr("Values No."), QString("%1").arg( field->size() ) , QString("text") );
        a->setEditable(false);
        a = am->updateAttr( tr("Min. Value"), QString("%1").arg( field->min() ), QString("text") );
        a->setEditable(false);
        a = am->updateAttr( tr("Max. Value"), QString("%1").arg( field->max() ), QString("text") );
        a->setEditable(false);
}

void FVBoxField::setupMenu( )
{
	contextMenuObj->clear();

        if (field->dim() > 1) {
		contextMenuObj->addAction(tr("&Draw Vectors"), this, SLOT( slotDrawVectors() ) );
                QString msg("Field" + this->getName() + " may be drawn as vectors" );
                qDebug(msg.toAscii());
            } else {
            QString msg("Field" + this->getName() + " will not be drawn as vectors");
                            qDebug(msg.toAscii());
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

void FVBoxField::slotDrawColormap( )
{
	FVFieldDraw * fd = new FVFieldDraw( manager, this );
	addChild(fd);
	fd->update();
}

void FVBoxField::slotDrawVectors( )
{
	FVFieldDraw * fd = new FVFieldDraw(manager, this );
        qDebug("FVFieldDraw created");
	fd->setAttrValue( QString("View Style"), QString("vectors") );
        qDebug("FVFieldDraw set up as vectors");
	addChild( fd );
        qDebug("FVFieldDraw added as child");
	fd->update();
        qDebug("FVFieldDraw updated");
}

void FVBoxField::slotSlice( )
{
	FVFieldSlice * fs = new FVFieldSlice(manager);
	addChild( fs );
	fs->slice();
	fs->update();
}

void FVBoxField::slotReload( )
{
    if (getOpener() != 0) {
        getOpener()->reload(this);
        update();
        setupAttributesMinMax();

        QStringList argv;
        argv.append( QString("update") );
        manager->sendMessageToChildrenOf(argv, this);
    } else {
        QMessageBox::information(parent, tr("B³¹d odœwie¿ania"),tr("Ten komponent nie wspiera odœwie¿ania."));
    }
}

FVInterface * FVBoxField::getInterface( QString interfaceName )
{
	if (interfaceName == QString("FVFieldInterface"))
		return fvFieldInterface;

	return parentInterface(interfaceName);
}

void FVBoxField::slotAnimate( )
{
	FVAnimation * fa = new FVAnimation(manager);
	fa->addParent(this);
	fa->initAttributes();
	fa->update();
}

QString FVBoxField::getName() {
    QString name= sName;
    QString reg( "^.*" );
    reg.append(QDir::separator());
    name.remove( QRegExp(reg) );
    return name;
};

void FVBoxField::update( )
{
	sName = field->getAttr("name").c_str();
	setAttrValue( QString("Name"), field->getAttr("name").c_str());
}

bool FVBoxField::message( const QList< QString > & argv )
{
	if (argv.size() > 0) {
		if (argv[0] == QString("setTime") ) {
			double tv = argv[1].toDouble();
			field->setCurTime( tv );
			return true;
		}
	}
	
	return true;
}
