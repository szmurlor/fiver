/***************************************************************************
 *   Copyright (C) 2006 by Robert Szmurlo   *
 *   robert@iem.pw.edu.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "fvanimation.h"
#include <main.h>
#include <field.h>
#include <QtDebug>


FVAnimation::FVAnimation(FVBoxMgr * manager, int x, int y)
    : FVObject(manager,x,y)
{
    sName = "Animation";
    sType = "Animation";
    cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_ANIMATION ).value<QColor>();

    tmr = 0;

    setupAttributes();
    setupMenu();
    lastPlaySpeed = tr("Turtle");
}


FVAnimation::~FVAnimation()
{
}

void FVAnimation::setupAttributes( )
{
    Attr * a;
    a = am->addAttr( tr("Type"), tr("Animation"), QString("text") );
    a->setEditable(false);
    a = am->addAttr( tr("Name"), tr("Unknown"), QString("text") );
    a->setEditable(false);

    a = am->addAttr( tr("Start Time:"), QString("%1").arg( 0 ) , QString("text") );
    a->setEditable(false);
    a = am->addAttr( tr("End Time:"), QString("%1").arg( 0 ), QString("text") );
    a->setEditable(false);
    a = am->addAttr( tr("Time Step:"), QString("%1").arg( 0 ), QString("text") );
    a->setEditable(false);

    a = am->addAttr( tr("Current Time:"), QString("%1").arg( 0 ), QString("textlist") );

    a = am->addAttr( tr("Play Speed:"), tr("Stop"), QString("textlist") );
    a->addListItem( tr("Stop") );
    a->addListItem( tr("Turtle") );
    a->addListItem( tr("Slow") );
    a->addListItem( tr("Normal") );
    a->addListItem( tr("Fast") );
    a->addListItem( tr("Cheetah") );
}

void FVAnimation::setupMenu( )
{
    contextMenuObj->clear();

    acPlay = contextMenuObj->addAction(tr("&Play"), this, SLOT( slotPlay() ) );
    acPause = contextMenuObj->addAction(tr("&Pause"), this, SLOT( slotPause() ) );
    acStop = contextMenuObj->addAction(tr("&Stop"), this, SLOT( slotStop() ) );

    acPause->setEnabled( false );
    acStop->setEnabled( false );


    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("&Update"), this, SLOT( slotUpdate() ) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );
}

void FVAnimation::update( )
{
    std::list< BoxBox *> children = manager->childrenOfBoxObj( (BoxBox*) this );
    std::list< BoxBox *>::iterator it;
    for (it = children.begin(); it != children.end(); it++)
        manager->sendMessage("update", (FVObject*) (*it), true);
}

void FVAnimation::slotUpdate( )
{
    update();
}

void FVAnimation::slotPlay()
{
    setAttrValue( tr("Play Speed:"), lastPlaySpeed );
    updateAttributes();
}


void FVAnimation::slotPause()
{
    lastPlaySpeed = getAttrValue( tr("Play Speed:") );
    setAttrValue( tr("Play Speed:"), tr("Stop") );
    updateAttributes();
}

void FVAnimation::slotStop()
{
    setAttrValue( tr("Play Speed:"), tr("Stop") );
    updateAttributes();
}


void FVAnimation::initAttributes()
{
    FVObject * fo;
    double start = 1e10, end=-1e10, step=1e10;
    Field * f;
    QString sFields("");

    std::list< BoxBox *> children = manager->childrenOfBoxObj( (BoxBox*) this );
    std::list< BoxBox *>::iterator it;
    for (it = children.begin(); it != children.end(); it++) {
        fo = (FVObject*) (*it);
        f = reqField.getField( fo );

        if (f != 0) {
            sFields = sFields + f->getAttr("name").c_str() + " ";
            if ( f->isDynamic() ) {
                if (start > f->getMinTime()) start = f->getMinTime();
                if (end < f->getMaxTime()) end = f->getMaxTime();
                if (step > f->getTimeStep()) step = f->getTimeStep();
            } else {
                start = end = step = 0;
                break;
            }
        } else {
            qWarning() << "Unable to read field parameters. Field * f = 0.";
        }
    }
    setAttrValue( tr("Name"), sFields);
    sName = sFields;
    setAttrValue( tr("Start Time:"), QString("%1").arg( start ) );
    setAttrValue( tr("End Time:"), QString("%1").arg( end ) );
    setAttrValue( tr("Time Step:"), QString("%1").arg( step ) );

    setAttrValue( tr("Current Time:"), QString("%1").arg( start ) );

    Attr * a = getAttr( tr("Current Time:") );
    if (a != 0) {
        if (start <= end) {
            a->addListItem( QString("%1").arg( start ) );
        } else
            for (double v = start; v <= end; v += step) {
            a->addListItem( QString("%1").arg( v ) );
        }
    }
}

void FVAnimation::updateAttributes( )
{
    QStringList argv;
    argv.append( QString("setTime") );
    argv.append( getAttrValue( tr("Current Time:") ) );

    manager->sendMessage( argv, this, true );


    update();

    int speed = 0;
    if (tmr != 0) {
        delete tmr;
        tmr = 0;
    }
    QString s = getAttrValue( tr("Play Speed:") );
    if (s == tr("Turtle")) speed = 3000;
    if (s == tr("Slow")) speed = 1000;
    if (s == tr("Normal")) speed = 500;
    if (s == tr("Fast")) speed = 250;
    if (s == tr("Cheetah")) speed = 100;

    if (speed > 0) {
        tmr = new QTimer();
        connect( tmr,SIGNAL(timeout()), this, SLOT( slotTimer() ) );
        tmr->start( speed );
    }

    if (speed > 0) {
        acPlay->setEnabled( false );
        acPause->setEnabled( true );
        acStop->setEnabled( true );
    } else {
        acPlay->setEnabled( true );
        acPause->setEnabled( false );
        acStop->setEnabled( false );
    }
}

void FVAnimation::slotTimer( )
{
    double v = getAttrValue( tr("Current Time:") ).toDouble();
    double st = getAttrValue( tr("Time Step:") ).toDouble();
    v += st;
    setAttrValue( tr("Current Time:") , QString("%1").arg( v  ));

    updateAttributes();
}


