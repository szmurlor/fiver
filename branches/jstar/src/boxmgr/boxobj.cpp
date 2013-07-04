#include "boxobj.h"
#include <QLabel>
#include <QMenu>
#include <QPainter>

BoxObj::BoxObj(BoxMgr * _parent)
:QObject(0)
{
    bSelected = false;
    btType = TBox;
    parent = _parent;
    indRect.setCoords( 0,0,0,0 );

    contextMenuObj = new QMenu( (QWidget*) parent );
}

BoxObj::~BoxObj()
{
    delete contextMenuObj;
}

void BoxObj::paintEvent( QPainter & )
{	
    qWarning("Trying to paint BoxObj which is abstract container.");
}

void BoxObj::contextMenu()
{
    contextMenuObj->exec( QCursor::pos() );
}

void BoxObj::selectedEvent()
{
}

void BoxObj::deselectedEvent()
{
}


void BoxObj::setParent( BoxMgr * _parent )
{
	parent = _parent;
	contextMenuObj->setParent( (QWidget*) _parent );
}

