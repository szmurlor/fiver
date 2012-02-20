#include "boxcon.h"
#include "boxmgr.h"


BoxCon::BoxCon( BoxMgr * _parent, BoxObj * src, BoxObj *dst )
: BoxObj(_parent)
{
    bsrc = src;
    bdst = dst;
    btType = TCon;
}

BoxCon::~BoxCon()
{
}

QPoint BoxCon::start()
{
    QPoint st;
    bool bhalf = false;

    if ( (bsrc->rRect.x() + bsrc->rRect.width()) < bdst->rRect.x()) {
        st.setX( bsrc->rRect.x() + bsrc->rRect.width() );
        bhalf = true;
    } else if (bsrc->rRect.x() > ( bdst->rRect.x() + bdst->rRect.width()) ) {
        st.setX( bsrc->rRect.x() );
        bhalf = true;
    } else {
        st.setX( bsrc->rRect.x() + bsrc->rRect.width() / 2);
    }

    if (bhalf) {
        st.setY( bsrc->rRect.y() + bsrc->rRect.height() / 2);
    } else if ( (bsrc->rRect.y() + bsrc->rRect.height()) < bdst->rRect.y()) {
        st.setY( bsrc->rRect.y() + bsrc->rRect.height() );
    } else if (bsrc->rRect.y() > ( bdst->rRect.y() + bdst->rRect.height()) ) {
        st.setY( bsrc->rRect.y() );
    } else {
        st.setY( bsrc->rRect.y() + bsrc->rRect.height() / 2);
    }

   return st;
}

QPoint BoxCon::end()
{
    QPoint st;
    bool bhalf = false;
    //st.setX( bdst->rRect.x() );
    //st.setY( bdst->rRect.y() + bdst->rRect.height() / 2 );

    if ( (bdst->rRect.x() + bdst->rRect.width()) < bsrc->rRect.x()) {
        st.setX( bdst->rRect.x() + bdst->rRect.width() );
        bhalf = true;
    } else if (bdst->rRect.x() > ( bsrc->rRect.x() + bsrc->rRect.width()) ) {
        st.setX( bdst->rRect.x() );
        bhalf = true;
    } else {
        st.setX( bdst->rRect.x() + bdst->rRect.width() / 2);
    }

    if (bhalf) {
        st.setY( bdst->rRect.y() + bdst->rRect.height() / 2);
    } else if ( (bdst->rRect.y() + bdst->rRect.height()) < bsrc->rRect.y()) {
        st.setY( bdst->rRect.y() + bdst->rRect.height() );
    } else if (bdst->rRect.y() > ( bsrc->rRect.y() + bsrc->rRect.height()) ) {
        st.setY( bdst->rRect.y() );
    } else {
        st.setY( bdst->rRect.y() + bdst->rRect.height() / 2);
    }

    return st;
}

void BoxCon::paintEvent( QPainter & p )
{
    #define BBOX_CON_W 0
    #define BBOX_CON_R 6
    #define BBOX_CON_R2 3
    
    QPoint p1,p2,p3,p4;
    p1 = start();
    p2 = end();
    p3 = p1;
    p3.setX( p3.x() + BBOX_CON_W );
    p4 = p2;
    p4.setX( p4.x() - BBOX_CON_W ); 
    
    p.setPen(2);
    p.drawLine(p1,p3);
    p.drawLine(p3,p4);
    p.drawLine(p4,p2);
    
    p.setBrush( Qt::SolidPattern );
    //p.drawEllipse( p1.x(), p1.y()-BBOX_CON_R2, BBOX_CON_R, BBOX_CON_R);
    p.drawEllipse( p2.x()-BBOX_CON_R, p2.y()-BBOX_CON_R2, BBOX_CON_R, BBOX_CON_R);
}

QRect BoxCon::rect()
{
    QRect tmp;
    int x,y,w,h;
    QPoint p1 = start(), p2 = end();
    if (p1.x() < p2.x()) {
        x = p1.x();
        w = p2.x() - x;
    } else {
        x = p2.x();
        w = p1.x() - x;
    }

    if (p1.y() < p2.y()) {
        y = p1.y();
        h = p2.y() - y;
    } else {
        y = p2.y();
        h = p1.y() - y;
    }

    tmp.setRect( x-15, y-15, w+30, h+30 );

    return (tmp);
}
