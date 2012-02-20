#include "boxmgr.h"
//Added by qt3to4:
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QFrame>
#include <QPaintEvent>
#include <QPalette>
#include <QDebug>
#include <iostream>

BoxMgr::BoxMgr( QWidget * parent )
:QFrame(parent)
{
    last_x = -1;
    last_y = -1;
    last_box = 0;
    isExternalConnecting = false;
}

BoxMgr::~BoxMgr()
{
    clear();
}

void BoxMgr::clear()
{
    std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); it++) {
        delete (*it);
    }
	last_box = 0;
    parentForConnection = 0;
    
    objs.resize( 0 );
}

void BoxMgr::removeObjRecur( BoxObj * obj )
{
	if (obj->btType == BoxObj::TBox) {
		std::list<BoxObj*>::iterator it;
		for (it = objs.begin(); it != objs.end(); it++) {
			if ( (*it)->btType == BoxObj::TCon ) {
				BoxCon * bc = (BoxCon*) (*it);
				if ((bc->bsrc == obj) || (bc->bdst == obj ))  {
					if ((bc->bsrc == obj) && (obj->removeChildren()))
						removeObjRecur(bc->bdst);
					bc->btType = BoxObj::TToDel;
				}
			}
		}
	}
	obj->btType = BoxObj::TToDel;
}

void BoxMgr::removeObj( BoxObj * obj )
{
    removeObjRecur( obj );
    std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); ) {
        if ( (*it)->btType == BoxObj::TToDel ) {
            delete (*it);
            it = objs.erase( it );
        } else {
            it++;
        }
    }
    if (last_box == obj)
        last_box = 0;
    
    update();
}


void BoxMgr::paintEvent( QPaintEvent * )
{
    QPainter p( this );

	QColor cb(108,131,115);
	p.fillRect( QRect(0,0,width(),height()), cb );
	
    // First paint the connections.
    std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); it++) {
        if ((*it)->btType == BoxObj::TCon)
            (*it)->paintEvent( p );
    }

    // Second: paint the boxes.
    for (it = objs.begin(); it != objs.end(); it++) {
        if ((*it)->btType == BoxObj::TBox)
            (*it)->paintEvent( p );
    }

    drawFrame( &p );
}

void BoxMgr::keyPressEvent(QKeyEvent * event)
{
    if ( (getMouseState() == Connect) && (event->key() == Qt::Key_Escape) ) {
        setCursor(Qt::ArrowCursor);
        setMouseState(Select);
        parentForConnection = 0;
    }
}

void BoxMgr::mousePressEvent( QMouseEvent * e )
{
    if (getMouseState() == Connect) {
        BoxObj * tmp = selectObj( e->x(), e->y() );
        if ( (tmp != 0) && (tmp != parentForConnection)) {
            if (parentForConnection != 0) {
                addCon(parentForConnection, tmp, tr(""), tr(""));
            } else
                qDebug() << "ParentForConnection should not be 0.";
        } else
            qDebug() << "Incorectly selected object.";

        setCursor(Qt::ArrowCursor);
        setMouseState(Select);
        parentForConnection = 0;
    } else if (getMouseState() == STLConnect ){
        BoxObj * tmp = selectObj( e->x(), e->y() );
        if ( (tmp != 0) && (tmp != parentForConnection)) {
            if ( tmp->sType == QString("STLGrid")){
                if (parentForConnection != 0) {
                    if (isExternalConnecting){
                        addCon(parentForConnection, tmp, tr("externalSTLGrid"), tr(""));
                    } else {
                        addCon(parentForConnection, tmp, tr(""), tr(""));
                    }

                } else
                    qDebug() << "ParentForConnection should not be 0.";
            }else
                qDebug() << "Type of resource should be STLFile but is " << tmp->sType;

        } else
            qDebug() << "Incorectly selected object.";

        setCursor(Qt::ArrowCursor);
        setMouseState(Select);
        parentForConnection = 0;
    } else {
        BoxObj * tmp = selectObj( e->x(), e->y() );
        if ( last_box != 0) {
            last_box->bSelected = false;
            last_box->deselectedEvent();
            update(last_box->rRect);
            updateConnections( last_box );
            last_box = 0;
        }
        if (tmp != 0) {
            last_box = tmp;
             
            last_box->bSelected = true;
            last_box->selectedEvent();
            last_x = e->x();
            last_y = e->y();
            update(last_box->rRect);
            updateConnections( last_box );
        }
    //}
    }
    isExternalConnecting=false;
}

void BoxMgr::mouseReleaseEvent( QMouseEvent * )
{
    last_x = -1;

    update ();
}

void BoxMgr::moveBoxRecur( BoxObj * obj, int dx, int dy )
{
    if (obj->btType == BoxObj::TBox) {
        std::list<BoxObj*>::iterator it;
        for (it = objs.begin(); it != objs.end(); it++) {
            if ( (*it)->btType == BoxObj::TCon ) {
                BoxCon * bc = (BoxCon*) (*it);
                if (bc->bsrc == obj)
                    moveBoxRecur(bc->bdst, dx, dy);
            }
        }
    }
    update( obj->rRect );
    obj->rRect.translate( dx, dy );
}

void BoxMgr::mouseMoveEvent( QMouseEvent * e )
{
    if ( (e->buttons() & Qt::LeftButton) == Qt::LeftButton ) {
        if (last_box != 0) {
            update(last_box->rRect);
            updateConnections( last_box );
            last_box->rRect.translate( e->x()-last_x, e->y()-last_y );
            last_x = e->x();
            last_y = e->y();
            update();
            //update(last_box->rRect);
            updateConnections( last_box );
        }
   } else if ( (e->buttons() & Qt::MidButton) == Qt::MidButton ) {
        if (last_box != 0) {
            update(last_box->rRect);
            updateConnections( last_box );
            moveBoxRecur( last_box, e->x()-last_x, e->y()-last_y );
            last_x = e->x();
            last_y = e->y();
            update();
            //update(last_box->rRect);
            updateConnections( last_box );
        }
   }
}

BoxObj * BoxMgr::addObj( BoxObj * obj )
{
    obj->setParent(this);
    objs.push_back( obj );
    update();
    return obj;
}

BoxObj * BoxMgr::addBox( QString type, QString name, QColor color )
{
    BoxObj *b = new BoxBox(this);
    b->rRect.setRect( 10, 10, 200, 40 );
    b->sType = type;
    b->sName = name;
    b->cColor = color;
    
    return addObj( b );
}

BoxObj * BoxMgr::addCon( BoxObj * src, BoxObj * dst, QString type, QString name )
{
    BoxObj *b = new BoxCon(this,src,dst);
    b->sType = type;
    b->sName = name;
    
    return addObj( b );
}

void BoxMgr::removeExternalCon( BoxObj * obj )
{
    std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); it++) {
        BoxCon* con = (BoxCon*)(*it);
        if ( (*it)->btType == BoxObj::TCon && con->bsrc == obj ) {
            int test = ((BoxObj*)(*it))->sType.compare(tr("externalSTLGrid"));
            if ( test == 0 ) {
                std::cout << "usuwanie obiektu: "<< (*it) ;
                delete (*it);
                it = objs.erase( it );
            }
        }
    }
    update();
}

void BoxMgr::removeAllCon( BoxObj * obj )
{
    std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); ) {
        BoxCon* con = (BoxCon*)(*it);
        if ( (*it)->btType == BoxObj::TCon && (con->bsrc == obj || con->bdst == obj)) {
            std::cout << "usuwanie obiektu: "<< (*it) ;
            delete (*it);
            it = objs.erase( it );
        } else {
            it++;
        }
    }
    update();
}




BoxObj * BoxMgr::selectObj( BoxObj * obj )
{
    BoxObj * old = getSelected();
    if (old != 0)
        old->bSelected = false;

    obj->bSelected = true;

    emit signalSelected(obj);

    return obj;
}

BoxObj * BoxMgr::selectObj( int x, int y )
{
	BoxObj * old = getSelected();
	BoxBox * box = 0;
        std::list<BoxObj*>::iterator it;
	for (it = objs.begin(); it != objs.end(); it++) {
		(*it)->bSelected = false;
		if ( (*it)->rRect.contains(x,y) == true ) {
			if ( (*it)->btType == BoxObj::TBox ) {
				box = (BoxBox*) (*it);

				QRect trec(box->indRect);
				trec.translate( box->rRect.x(), box->rRect.y() );
			
				if (trec.contains(x,y)) {
					box->setCheckbox( ! box->checkbox() );
					setCheckboxBoxRecur( box, box->checkbox() );
				}
			}
		}
	}

	if (old != box) {
		if (box != 0)
			emit signalSelected( box );
	}
	return box;
}

void BoxMgr::contextMenuEvent ( QContextMenuEvent * e )
{
    if (getSelected() != 0) {
        getSelected()->contextMenu();
    }
	if (e!=0)
		return;
}

BoxObj * BoxMgr::getSelected()
{

    std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); it++) 
        if ((*it)->bSelected == true)
            return *it;

    return 0;
}


/*!
    \fn BoxMgr::slotDelete()
 */
void BoxMgr::slotDelete()
{
    removeObj( getSelected() );
}

void BoxMgr::updateConnections( BoxObj * obj )
{
    
    std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); it++) {
        if ( (*it)->btType == BoxObj::TCon ) {
            BoxCon * bc = (BoxCon*) (*it);
            if ((bc->bsrc == obj ) || (bc->bdst == obj)) {
                QRect r = bc->rect();
                //qDebug("updating connection: %d, %d, %d, %d",r.x(), r.y(), r.width(), r.height());
                update( r );
            }
        }
    }
}

void BoxMgr::setCheckboxBoxRecur( BoxObj * obj, bool newVal )
{
    if (obj->btType == BoxObj::TBox) {
        std::list<BoxObj*>::iterator it;
        for (it = objs.begin(); it != objs.end(); it++) {
            if ( (*it)->btType == BoxObj::TCon ) {
                BoxCon * bc = (BoxCon*) (*it);
                if (bc->bsrc == obj)
                    setCheckboxBoxRecur(bc->bdst, newVal);
            }
        }
	    update( obj->rRect );
    	((BoxBox*) obj)->setCheckbox( newVal );
    }
}

std::list<BoxBox*> BoxMgr::childrenOfBoxObj( BoxBox * obj )
{
	std::list<BoxBox *> lst;
	std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); it++) {
        if ( (*it)->btType == BoxObj::TCon ) {
            BoxCon * bc = (BoxCon*) (*it);
            if (bc->bsrc == obj )  {
				lst.push_back( (BoxBox*) bc->bdst );
            }
        }
    }

	return lst;
}

std::list<BoxBox*> BoxMgr::holesForMeshJoiner( BoxBox * obj )
{
        std::list<BoxBox *> lst;
        std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); it++) {
        if ( (*it)->btType == BoxObj::TCon ) {
            BoxCon * bc = (BoxCon*) (*it);
            if (bc->bsrc == obj && bc->sType != "externalSTLGrid" )  {
                                lst.push_back( (BoxBox*) bc->bdst );
            }
        }
    }

        return lst;
}

BoxBox* BoxMgr::externalForMeshJoiner( BoxBox * obj )
{
    //    std::list<BoxBox *> lst;
    std::list<BoxObj*>::iterator it;
    for (it = objs.begin(); it != objs.end(); it++) {
        if ( (*it)->btType == BoxObj::TCon ) {
            BoxCon * bc = (BoxCon*) (*it);
            if (bc->bsrc == obj && bc->sType == "externalSTLGrid" )  {
                //                            lst.push_back( (BoxBox*) bc->bdst );
                return (BoxBox*) bc->bdst;
            }
        }
    }
    return 0;
}

void BoxMgr::setMouseState( BoxMgr::MouseState state )
{
    this->mouseState = state;
}

BoxMgr::MouseState BoxMgr::getMouseState()
{
    return this->mouseState;
}

QString BoxMgr::connectChild(BoxObj * parent)
{
    if (parent != 0) {

        selectObj(parent);

        setMouseState(Connect);
        setCursor(Qt::PointingHandCursor);
        parentForConnection = (BoxBox*) parent;

        update();
    } else {
        qDebug() << "Parent can not be 0 in BoxMgr::connectChild()";
        return tr("Argument parent can not be 0.");
    }

    return 0;
}

QString BoxMgr::connectExternalChild(BoxObj * parent)
{
    if (parent != 0) {

        selectObj(parent);

        setMouseState(STLConnect);
        setCursor(Qt::PointingHandCursor);
        parentForConnection = (BoxBox*) parent;
        isExternalConnecting = true;

        update();
    } else {
        qDebug() << "Parent can not be 0 in BoxMgr::connectChild()";
        return tr("Argument parent can not be 0.");
    }

    return 0;
}


QString BoxMgr::connectSTLChild(BoxObj * parent, bool isExternal)
{
    if (parent != 0) {

        if (isExternal){
            connectExternalChild(parent);
        } else {
            selectObj(parent);

            setMouseState(STLConnect);
            setCursor(Qt::PointingHandCursor);
            parentForConnection = (BoxBox*) parent;

        update();
        }
    } else {
        qDebug() << "Parent can not be 0 in BoxMgr::connectChild()";
        return tr("Argument parent can not be 0.");
    }

    return 0;
}
