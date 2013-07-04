//
// C++ Interface: boxbox
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BOXBOX_H
#define BOXBOX_H

#include "boxobj.h"

#include <QCheckBox>
#include <QStyle>
#include <QPainter>
#include <QList>
#include <QVariant>
#include <list>

class BoxMgr;
/**
@author Robert Szmurlo
*/
class BoxBox : public BoxObj
{
protected:

	bool indChecked;
	void drawCheckBox( QPainter * p, QRect &rect, bool checked );
public:

    BoxBox(BoxMgr * _parent);
    virtual ~BoxBox();

    virtual void paintEvent( QPainter & p );

    virtual QString getName() { return sName; };

	virtual void setCheckbox( bool checked );
	virtual bool checkbox();
	virtual void checkboxEvent( bool value );

	// return 0 on success
	virtual int sendMsg( const QList< QVariant > msg );
	virtual int sendMsgChildren( const QList< QVariant > msg );
};

#endif
