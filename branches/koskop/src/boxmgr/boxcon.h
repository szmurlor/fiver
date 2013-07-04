#ifndef _BOXCON_H_IS_INCLUDED
#define _BOXCON_H_IS_INCLUDED

#include "boxobj.h"
#include <qpoint.h>
#include <qrect.h>

class BoxMgr;

class BoxCon : public BoxObj
{
    public:
        BoxObj * bsrc, * bdst;
        
        BoxCon(BoxMgr * _parent, BoxObj *src, BoxObj *dst);
        virtual ~BoxCon();

        QPoint start();
        QPoint end();
        QRect rect();

        
        virtual void paintEvent( QPainter & p );
};


#endif
