//
// C++ Implementation: fvobject
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QtDebug>
#include <fvobject.h>
#include <fivermain.h>
#include <main.h>
#include <iostream>

FVObject::FVObject(FVBoxMgr * _manager, int x, int y)
:BoxBox(_manager)
{
	fvlist = 0;
	setDrawable( false );
    bVisible = true;
	bFirstShow = true;
    manager = _manager;
	am = new AttrModel( this );
	
	fvinteraction = 0;

	if (x != 0)	rRect.setX(x);
	else rRect.setX(fvsettings.value( classType() + "_DefaultX", FV_DEFAULT_BOX_X ).toInt());
	if (y != 0)	rRect.setY(y);
	else rRect.setY(fvsettings.value( classType() + "_DefaultY", FV_DEFAULT_BOX_Y ).toInt());
	rRect.setWidth( fvsettings.value( classType() + "_DefaultWidth", FV_DEFAULT_BOX_Width ).toInt() );
	rRect.setHeight( fvsettings.value( classType() + "_DefaultHeight", FV_DEFAULT_BOX_Height ).toInt() );
	
	cColor = fvsettings.colorValue( classType()+"_DefaultColor" , FV_DEFAULT_BOX_COLOR_DRAW );
}


FVObject::~FVObject()
{
    if (fvlist != 0) {
        delete fvlist;
        fvlist = 0;
    }
	
	if (fvinteraction != 0) {
        getCurrentViewer()->setInteractionModel(NULL);
		delete fvinteraction;
		fvinteraction = 0;
	}
}

/*!
    \fn FVObject::paintGL
 */
void FVObject::paintGL()
{
    /// @todo implement me
}

void FVObject::selected( )
{
}

void FVObject::deselected( )
{
}

FVObject * FVObject::addChild( FVObject * child, QString name, QString type )
{
        if (name != QString::null) {
		child->sName = name;
		child->setAttrValue("Name", name);
	} else {
		child->setAttrValue("Name", child->sName);
	}

	if (type != QString::null) {
		child->sType = type;
		child->setAttrValue("Type", type);
	} else {
		child->setAttrValue("Type", child->sType);
	}

	child->rRect.moveTo( childSuggestedX(), childSuggestedY() );

	manager->addObj( child );
	manager->addCon( this, child, QString(""), QString("") );
	
	child->initAttributes();
	
	return child;
}

FVObject * FVObject::addParent( FVObject * parent, QString name, QString type )
{
	if (name != QString::null) {
		sName = name;
		setAttrValue("Name", name);
	} else {
		setAttrValue("Name", sName);
	}

	if (type != QString::null) {
		sType = type;
		setAttrValue("Type", type);
	} else {
		setAttrValue("Type", sType);
	}
	
	if (parent != 0)
		rRect.moveTo( parent->childSuggestedX(), parent->childSuggestedY());
	
	manager->addObj(this);
	manager->addCon( this,parent, QString(""), QString("") );
	
	initAttributes();
	
	return this;
}

void FVObject::update( )
{
	getCurrentViewer()->makeCurrent();
	paintGL();
	getCurrentViewer()->updateGL();
}

void FVObject::slotHide()
{
	setCheckbox(false);
	bVisible = false;
	fvlist->bVisible = false;
	
	manager->update();
	getCurrentViewer()->update();
}

void FVObject::slotShow()
{
	setCheckbox(true);
	bVisible = true;
	fvlist->bVisible = true;
	
	manager->update();
	getCurrentViewer()->update();
}

void FVObject::indicatorSelected( bool checked )
{
	bVisible = checked;
	if (fvlist != 0) { 
		// only for objects visible in viewer
		fvlist->bVisible = checked;
		getCurrentViewer()->update();
	}
	manager->update();
}

AttrModel * FVObject::getModel( )
{
	if (am == 0)
		qDebug() << "Invalid model pointer (am) for FVObject: " << sName;
	
	return am;
}

Attr * FVObject::getAttr( QString name )
{
	if (getModel() == 0) {
		return 0;
	}
	
	return getModel()->getAttr( name );
}

QString FVObject::setAttrValue( QString name, QString value )
{
        bool found = true;

	if (getModel() == 0)
                return QString("Error: No model");
	
	Attr * a = getModel()->getAttr( name );
	if (a != 0) {
                a->setValue( value );
                found = true;
	} else {
                qDebug() << "Unknow attribute: " << name << " for FVObject: " << sName;
                found = false;
	}
        getModel()->submit();
        getModel()->update();
        if(found)
            return QString("OK");
        return QString("Error: Attribute '"+name+"' not found");
}

QString FVObject::getAttrValue( QString name )
{
	if (getModel() == 0)
		return QString::null;
	
	Attr * a = getModel()->getAttr( name );
	if (a != 0) {
		return a->value();
	} else {
		qDebug() << "Unknow attribute: " << name << " for FVObject: " << sName;
	}
	return QString::null;
}

void FVObject::setEditable( bool editable, char** set, int n ) {
    for( int i=0; i < n; i++ ) {
        am->getAttr(tr(set[i]))->setEditable(editable);
    }
}


void FVObject::checkboxEvent( bool value )
{
	if (fvlist != 0) {
		fvlist->bVisible = value;
		getCurrentViewer()->updateGL();
	}
}

void FVObject::setDrawable( bool isDrawable )
{
	if (isDrawable) {
		if (fvlist == 0) {
			FViewer * fv = getCurrentViewer();
			fvlist = new FVList( fv );
			if (fv != 0) {
				fv->add( fvlist );
			}
		}
	} else {
		if (fvlist != 0) {
			delete fvlist;
		}
		fvlist = 0;
	}
}

bool FVObject::isDrawable( )
{
	return (fvlist != 0);
}

bool FVObject::isFirstShow( )
{
	if (bFirstShow) {
		bFirstShow = false;
		return true;
	}
	return false;
}

FVInteractionModel * FVObject::getInteractionModel( )
{
	return fvinteraction;
}

bool FVObject::isOfClassType( QString str )
{
	return str == classType();
}

QString FVObject::classType( )
{
	return QString("FVObject");
}

int FVObject::childSuggestedX( )
{
    return rRect.x() + rRect.width();
}

int FVObject::childSuggestedY( )
{
	return rRect.y() + rRect.height() + 5;
}

void FVObject::initDrawable( QString name, QString type )
{
	setDrawable(true);
	sName = name;
	sType = type;
	setupAttributes();
	setupMenu();
}

void FVObject::messageHandler( const QList< QString > & argv )
{
	bool bProcessed = false;

	// Interpreting standard messages.
	bProcessed = message( argv );
		
	if (!bProcessed)
		if (argv[0] == QString("update")) {
			update();
			bProcessed = true;
		}

	if (!bProcessed) 
		qDebug() << "Warning: " << sName << " unprocessed message: " << argv[0];
}

bool FVObject::message( const QList< QString > &  )
{
	return false;
}

void FVObject::updateAttributes( )
{
	
	update();
}

void FVObject::rotate3D( double *, double  )
{
	qDebug() << "Warning: " << sName << " rotate3D o[], angle not yet implemented.";
}

FVInterface * FVObject::getInterface( QString )
{
	return 0;
}

FVObject * FVObject::parentObject( )
{
	return manager->getParentOf( this );
}

FVInterface * FVObject::parentInterface( QString interfaceName )
{
	FVObject * fvo = manager->getParentOf( this );
	if (fvo != 0) 
		return fvo->getInterface( interfaceName );
	return 0;
}

QString FVObject::dump()
{
    QString s;
    s += sName + " = fvboxmgr.find(" + sName + ")\n";
    // for attr in all_attrs
    // s += "print " + sName + ".set('" + key + "','" + value "')\n"'
    return s;
}

QString FVObject::callSlot( QString slotName )
{
    // UWAGA na razie nie wiem co zrobic, ale jezeli
    // slot nie istnieje to program sie czasami wywraca
    // (zawiesza sie wewnatrz funkcji indexOfSlot!)

    const QMetaObject * mo = metaObject();


    // Tylko do sledzenia jakie sa w danej klasie zadeklarowane
    // sloty
    /*
    for (int i = 0; i < mo->methodCount(); i++)
    {
        QMetaMethod met = mo->method(i);
        qDebug() << "metoda[" << i << "]: " << met.signature();
    }
    */

    // inedxOfSlot porównuje całą sygnaturę slotu
    // z uwzglednieniem argumentów do przeciazania
    // taka postac nazywa sie: znormalizowana
    // Cytat:
    // "Note that the method has to be in normalized
    // form, as returned by normalizedSignature()."
    QString slotNameNormalized = slotName + "()";
    int idx = mo->indexOfSlot( slotNameNormalized.toStdString().c_str() );
    if (idx >=0 ) {
        mo->invokeMethod( (QObject*) this, slotName.toStdString().c_str()  );
        return QString::null;
    } else
        return tr("The slot could not be invoked.");

}
