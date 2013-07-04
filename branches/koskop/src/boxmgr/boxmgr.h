#ifndef _BOXMGR_H_IS_INCLUDED
#define _BOXMGR_H_IS_INCLUDED

#include <QObject>
#include <QFrame>
#include <QEvent>
#include <QPainter>
#include <QRect>
//Added by qt3to4:
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QPaintEvent>

#include <list>

#include "boxobj.h"
#include "boxbox.h"
#include "boxcon.h"



class BoxMgr:public QFrame {
  
  Q_OBJECT   

  public:
    enum MouseState { Select, Connect , STLConnect };

  protected:
    int last_x, last_y;
    BoxObj *last_box;
    MouseState mouseState;
    BoxBox * parentForConnection;

    void removeObjRecur(BoxObj * obj);
    void updateConnections(BoxObj * obj);
    void moveBoxRecur(BoxObj * obj, int dx, int dy);
    void setCheckboxBoxRecur(BoxObj * obj, bool newVal);

  public:

    std::list < BoxObj * >objs;

    BoxMgr(QWidget * parent);
    virtual ~BoxMgr();

    virtual void paintEvent(QPaintEvent * pe);
    virtual void mousePressEvent(QMouseEvent * e);
    virtual void mouseMoveEvent(QMouseEvent * e);
    virtual void mouseReleaseEvent(QMouseEvent * e);
    virtual void contextMenuEvent(QContextMenuEvent * e);
    virtual void keyPressEvent(QKeyEvent *);


    BoxObj *addCon(BoxObj * src, BoxObj * dst, QString type, QString name);
    BoxObj *addBox(QString type, QString name, QColor color);
    BoxObj *addObj(BoxObj * obj);
    BoxObj *selectObj(int x, int y);
    BoxObj *selectObj(BoxObj * obj);
    BoxObj *getSelected();
    void removeObj(BoxObj * obj);
    void removeExternalCon( BoxObj * obj );
    void removeAllCon( BoxObj * obj );

    std::list < BoxBox * > childrenOfBoxObj(BoxBox * obj);
    std::list < BoxBox * > holesForMeshJoiner(BoxBox * obj);
    BoxBox* externalForMeshJoiner(BoxBox * obj);

    void clear();

    void setMouseState( MouseState state );
    MouseState getMouseState();

    QString connectChild(BoxObj * parent);
    QString connectExternalChild(BoxObj * parent);
    QString connectSTLChild(BoxObj * parent, bool isExternal);
    bool isExternalConnecting;

  public slots: 
	void slotDelete();
	
  signals:
	void signalSelected( BoxObj * obj );
};


#endif
