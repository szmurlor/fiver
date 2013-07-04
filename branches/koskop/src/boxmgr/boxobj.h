#ifndef _BOXOBJ_H_IS_INCLUDED
#define _BOXOBJ_H_IS_INCLUDED

#include <QObject>
#include <QRect>
#include <QColor>
#include <QString>
#include <QMenu>

class BoxMgr;

class BoxObj : public QObject
{
protected:
	QMenu * contextMenuObj;
	BoxMgr * parent;

public:
	enum BoxObjType { TBox, TCon, TToDel };
	
	BoxObjType btType;
	
	QRect rRect;
        QRect indRect;
	QColor cColor;
	QString sType;
	QString sName;
	bool bSelected;

	BoxObj(BoxMgr * _parent);
	virtual ~BoxObj();
	virtual bool removeChildren() { return true; }

	void setParent( BoxMgr * _parent );
	
	virtual void paintEvent( QPainter & p );
	virtual void contextMenu(); 

	virtual void selectedEvent();
	virtual void deselectedEvent();

        QMenu * getContextMenuObj() { return contextMenuObj; }

};


#endif
