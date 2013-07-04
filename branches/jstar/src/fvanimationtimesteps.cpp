#include "fvanimationtimesteps.h"

#include "fvanimation.h"
#include <main.h>
#include <math.h>
#include <QtDebug>


FVAnimationTimeSteps::FVAnimationTimeSteps(FVBoxMgr * manager, int x, int y)
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


FVAnimationTimeSteps::~FVAnimationTimeSteps()
{
}

void FVAnimationTimeSteps::setupAttributes( )
{
    Attr * a;
    a = am->addAttr( tr("Type"), tr("Animation"), QString("text") );
    a->setEditable(false);
    a = am->addAttr( tr("Name"), tr("Unknown"), QString("text") );
    a->setEditable(false);

    a = am->addAttr( tr("Start Time:"), QString("%1").arg( 0 ) , QString("text") );
    a = am->addAttr( tr("End Time:"), QString("%1").arg( 0 ), QString("text") );
    a = am->addAttr( tr("Time Step:"), QString("%1").arg( 0 ), QString("text") );
    a = am->addAttr( tr("Current Time:"), QString("%1").arg( 0 ), QString("text") );

    a = am->addAttr( tr("Play Speed:"), tr("Stop"), QString("textlist") );
    a->addListItem( tr("Stop") );
    a->addListItem( tr("Turtle") );
    a->addListItem( tr("Slow") );
    a->addListItem( tr("Normal") );
    a->addListItem( tr("Fast") );
    a->addListItem( tr("Cheetah") );
}

void FVAnimationTimeSteps::setupMenu( )
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
    contextMenuObj->addAction(tr("&Connect child"), this, SLOT( slotConnectChild()) );

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );
}

void FVAnimationTimeSteps::slotConnectChild()
{
    manager->connectChild(this);
}

void FVAnimationTimeSteps::slotUpdate( )
{
    update();
}

void FVAnimationTimeSteps::slotPlay()
{
    setAttrValue( tr("Play Speed:"), lastPlaySpeed );
    updateAttributes();
}


void FVAnimationTimeSteps::slotPause()
{
    lastPlaySpeed = getAttrValue( tr("Play Speed:") );
    setAttrValue( tr("Play Speed:"), tr("Stop") );
    updateAttributes();
}

void FVAnimationTimeSteps::slotStop()
{
    setAttrValue( tr("Play Speed:"), tr("Stop") );
    updateAttributes();
}


void FVAnimationTimeSteps::initAttributes()
{
    double tstop= 2 * M_PI;
    std::list< BoxBox *> children = manager->childrenOfBoxObj( this );
    std::list< BoxBox *>::iterator it;
    for (it = children.begin(); it != children.end(); it++) {
        FVObject* fvo= (FVObject*)*it;
        double freq= fvo->getAttrValue("frequency [Hz]").toDouble();
        if( freq != 0 )
            tstop = 1 / freq; // tstop <- T
    }

    setAttrValue( tr("Start Time:"), tr("0.0") );
    setAttrValue( tr("End Time:"), QString::number(tstop) );
    setAttrValue( tr("Time Step:"), QString::number(tstop / 20) );
    setAttrValue( tr("Current Time:"), tr("0.0") );

}

void FVAnimationTimeSteps::updateAttributes( )
{
    QStringList argv;
    argv.append( QString("setTime") );
    argv.append( getAttrValue( tr("Current Time:") ) );
    manager->sendMessageToChildrenOf( argv, this);

    argv.clear();
    argv.append( QString("update") );
    manager->sendMessageToChildrenOf(argv, this);

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

void FVAnimationTimeSteps::slotTimer( )
{
    double v = getAttrValue( tr("Current Time:") ).toDouble();
    double st = getAttrValue( tr("Time Step:") ).toDouble();
    v += st;
    setAttrValue( tr("Current Time:") , QString("%1").arg( v  ));

    updateAttributes();
}


