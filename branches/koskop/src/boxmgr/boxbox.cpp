//
// C++ Implementation: boxbox
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "boxbox.h"
#include "boxmgr.h"
//Added by qt3to4:
#include <QList>

BoxBox::BoxBox(BoxMgr * _parent)
 : BoxObj(_parent)
{
	btType = TBox;
	indChecked = true;
    indRect.setCoords( 3, 3, 13, 13 );
	cColor = QColor(203,197,144);

    contextMenuObj->addAction("&Delete",(QWidget*)parent, SLOT(slotDelete()) );
}


BoxBox::~BoxBox()
{
}

void BoxBox::paintEvent( QPainter & p )
{
    QRect r (rRect);
    r.setWidth( r.width() - 2 );
    r.setHeight( r.height() - 2 );
    p.setFont( QFont("fixed",8) );
    p.setBrush( QBrush(cColor, Qt::SolidPattern) );
    p.setPen(QPen(QColor(115,117,115),1, Qt::SolidLine));
    p.drawRect(r.x()+1,r.y()+1,r.width()-1,r.height()-1);

    p.setBrush( Qt::NoBrush );
    p.setPen( QPen(QColor(205,202,205),1, Qt::SolidLine));
    p.drawLine(r.x(),r.y(), r.x(), r.y() + r.height());
    p.drawLine(r.x(),r.y(), r.x() + r.width(), r.y());

    p.setPen( QPen(QColor(255,255,255),1, Qt::SolidLine));
    p.drawLine(r.x()+r.width(),r.y(), r.x()+r.width(), r.y() + r.height());
    p.drawLine(r.x(),r.y()+r.height(), r.x()+r.width(), r.y() + r.height());

    if (bSelected)
        p.setPen( QPen(QColor(255,255,255),1, Qt::SolidLine));
    else
        p.setPen( QPen(QColor(205,202,205),1, Qt::SolidLine));
    p.drawLine(r.x()+r.width()-2,r.y()+2, r.x()+r.width()-2, r.y() + r.height()-2);
    p.drawLine(r.x()+2,r.y()+r.height()-2, r.x()+r.width()-2, r.y() + r.height()-2);

    if (bSelected)
        p.setPen(QPen(QColor(115,117,115),1, Qt::SolidLine));
    else
        p.setPen( QPen(QColor(205,202,205),1, Qt::SolidLine));
    p.drawLine(r.x()+2,r.y()+2, r.x()+2, r.y() + r.height()-2);
    p.drawLine(r.x()+2,r.y()+2, r.x() + r.width()-2, r.y()+2);

    int x = r.x() + indRect.x() + indRect.width() + 2;
    int y = r.y();
    int dx = 6 + indRect.x() + indRect.width();
 
    p.setPen( QPen(QColor(0,0,0),1, Qt::SolidLine));
    QFont f = p.font();
    f.setBold(false);
    p.setFont( f );
    p.drawText( x, y + 3 , rRect.width()-dx, rRect.height()-8, Qt::AlignLeft, QString( getName() ) + QString(":") );
    QFontMetrics fm(f);
    int tw = fm.width( QString( getName() ) + QString(":") );

    f.setBold(true);
    p.setFont( f );

    p.drawText( tw + 3 + x, y + 4 /*+ p.fontInfo().pixelSize() */, rRect.width()-tw-3-dx, rRect.height()-8, Qt::AlignRight, QString( sType ) );

/*    if( ! indChecked ) {
      p.setPen( QPen(QColor(0,0,0),1, Qt::SolidLine));
      p.drawLine( r.x()+3, r.y()+r.height()-3, r.x()+r.width()-3, r.y()+3 );
    }
    */

	/* Drawing the visibility indicator. */
	QRect trec(indRect);
	trec.translate( rRect.x(), rRect.y() );

    drawCheckBox( &p, trec, indChecked );

	QRect tt (trec);
	tt.translate(-1,-1);
	tt.setWidth( tt.width() + 2 );
	tt.setHeight( tt.height() + 2 );
	p.setPen( QPen(QColor(231,231,231),1, Qt::SolidLine));
	p.drawRect( tt );
	
	p.setPen( QPen(QColor(0,0,0),1, Qt::SolidLine));
	if (indChecked) {
		p.setBrush( QBrush(QColor(55,231,46), Qt::SolidPattern) );
	} else {
		p.setBrush( QBrush(QColor(219,75,75), Qt::SolidPattern) );
	}
	p.drawRect( trec );
}

void BoxBox::setCheckbox( bool checked )
{
    if (indChecked != checked) {
        indChecked = checked;
        checkboxEvent( checked );
    }
}


void BoxBox::checkboxEvent( bool value )
{
	if (value)
		return;
}

bool BoxBox::checkbox( )
{
	return indChecked;
}

int BoxBox::sendMsg( const QList< QVariant > msg )
{
	// success
	if (msg.size()>0)
		return (0);
	else
		return(-1);
}

int BoxBox::sendMsgChildren( const QList< QVariant > msg )
{
	std::list<BoxBox*> lst = parent->childrenOfBoxObj( this );
	std::list<BoxBox*>::iterator it;
	for (it = lst.begin(); it != lst.end(); it++) {		
		(*it)->sendMsg( msg );
	}
	return (0);
}

void BoxBox::drawCheckBox( QPainter * , QRect & , bool )
{
}
