//
// C++ Interface: fvobject
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVOBJECT_H
#define FVOBJECT_H

#include <boxbox.h>
#include <fvinteractionmodel.h>
#include <fvboxmgr.h>
#include <fvlist.h>
#include <attrmodel.h>
#include <attr.h>
#include <fvinterface.h>

#include <QtDebug>
#include <QMetaMethod>



/**
@author Robert Szmurlo
*/
class FVObject : public BoxBox {

Q_OBJECT

public:
	/** x,y - positions of new object in the FVBoxMgr widget. */
	FVObject(FVBoxMgr * _manager, int x = 0, int y = 0);
	virtual ~FVObject();
		
	/** This method must filled only for objects drawable
	    in the viewer window.*/
	virtual void paintGL();
	bool isDrawable();

	FVObject * addChild( FVObject * child, QString name = QString::null, QString type = QString::null );
	FVObject * addParent( FVObject * parent, QString name = QString::null, QString type = QString::null );

	/** Virtual methods which should be implemented
	    for objects visible in the viewer window.
	
	    selected and deselected may be implemented to 
	    take specific object related functionality.
	    The functionality of displaying the atributes of
	    currently selected Box is made in FiverMain. */
	virtual void selected();
	virtual void deselected();
	virtual void update();
	virtual void initAttributes() {};
	virtual void updateAttributes();
    virtual QString refresh() { return tr("Empty implementation."); };
	
	/** Checkbox related. */
	virtual void indicatorSelected( bool checked );
	virtual void checkboxEvent( bool value );

	/** Suggested location for child boxes (FVObjects) */
	int childSuggestedX();
	int childSuggestedY();
	
	/**	Attributes model related methods.
	    To inform the system about attributes of the derived object
	    please fill in the Model in the constructor of derived class.
	*/
	AttrModel * getModel();
	Attr* getAttr( QString name );
        QString setAttrValue( QString name, QString value);
	QString getAttrValue( QString name );
        void setEditable( bool editable, char** set, int n );  // jstar: set editability of selected attributes

	/** Runtime type checking: */
	virtual bool isOfClassType( QString str );
	virtual QString classType();

	/** Interaction model emits some signals when user does some
	    interaction in the viewer window. 
	    The viewer window can draw many interaction models,
	    however only one can be active! The model of currently
	    selected box in the BoxMgr is active.
	
	    This function is used to get the interaction model
	    of currently selected object (Box).*/
	FVInteractionModel * getInteractionModel();
	
	/**
		Three dimensional manipulators used by manipulators.
		Those methods are optionally implemented only if FIVER manipulators
		can modify the geomtrical parameters of the object.
	*/
	virtual void translate3D(double , double , double ) {};
	void translate3D(double t[3]) { translate3D(t[0], t[1], t[2]); };
	
	virtual void rotate3D(double , double , double ) {};
	void rotate3D( double o[3], double angle );
	
	virtual void scale3D(double , double , double ) {};
	void scale3D( double s) { scale3D(s,s,s); };

	/** This is called only by FVBoxMgr::sendMessage and normally you 
	    should not call it directly. */
	void messageHandler( const QList<QString> &argv );
	
	/** This method returns specified interface. 
	    (Examples: FVGridInterface, FVFieldInterface)
	    The usual pupose of the interface is to provide some information
	    neccesary for child components. The child components receive
	    the information about the parent as a pointer to FVObject, thus
	    the information about some interface is necessary.
	*/
	virtual FVInterface * getInterface( QString interfaceName );
    QString dump();
    QString callSlot( QString slotName );

    void setName( QString name ) {
        this->sName = name;
        setAttrValue("Name", name);
    }
	
    void setType( QString type ) {
        this->sType = type;
        setAttrValue("Type", type);
    }

public slots:
	void slotHide();
	void slotShow();
	
	virtual void slotMousePress( QMouseEvent * ) {} ;
	virtual void slotKeyPress( QKeyEvent * ) {} ;
	virtual void slotMouseRelease( QMouseEvent * ) {};
	virtual void slotMouseMove( QMouseEvent * ) {};
	
protected:
	FVBoxMgr	*manager;
	AttrModel 	*am;
	FVList		*fvlist;
	bool		bVisible;
	FVInteractionModel * fvinteraction;
	
	/** Those 2 functions should be overriden in derived classes. */
	virtual void setupAttributes() {};
	virtual void setupMenu() {};

	/** Please use constructor attribute isDrawable to indicate the drawablitity,
	    This function should not be called. */
	void setDrawable( bool isDrawable );

	/** This method may be called in the derived class constructors.
        It is provided only for convenience. */
	void initDrawable( QString name, QString type );
	
	/** This function helps to perform some additional tasks
	    by the derived classes when they are painted for the first time. 
	    When the isFirstShow is called for the first time, it returns true.
	    Every next time it returns false. */
	bool isFirstShow();
	
	/**
	 * Method used for communication between objects. See: FVBoxMgr::sendMessage(...)
	 */
	virtual bool message( const QList<QString> &argv );

	FVObject * parentObject();
        FVInterface * parentInterface( QString interfaceName );

private:
        bool	bFirstShow;
};

#endif
