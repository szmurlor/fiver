/*****************************************************************************
 *   Copyright (C) 2006 Robert Szmurlo <robert@iem.pw.edu.pl>                *
 *                                                                           *
 *   Licensed under the Apache License, Version 2.0 (the "License");         *
 *   you may not use this file except in compliance with the License.        *
 *   You may obtain a copy of the License at                                 *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *   Unless required by applicable law or agreed to in writing, software     *
 *   distributed under the License is distributed on an "AS IS" BASIS,       *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *   See the License for the specific language governing permissions and     *
 *   limitations under the License.                                          *
 *****************************************************************************/

#include "guiattributes.h"

#include <QLayout>
#include <QPushButton>
#include <main.h>

GUIAttributes::GUIAttributes(QWidget * parent)
 : QDialog(parent)
{
	defColors.push_back( QColor(170,170,0) );
	defColors.push_back( QColor(0,255,0) );
	defColors.push_back( QColor(255,255,0) );
	defColors.push_back( QColor(255,170,127) );
	defColors.push_back( QColor(0,85,255) );
	defColors.push_back( QColor(255,85,0) );
	defColors.push_back( QColor(0,85,0) );
	defColors.push_back( QColor(85,0,127) );
	defColors.push_back( QColor(85,170,0) );
	defColors.push_back( QColor(255,170,255) );
	defColors.push_back( QColor(0,170,127) );
	defColors.push_back( QColor(85,85,255) );
	defColors.push_back( QColor(170,85,127) );
	defColors.push_back( QColor(85,255,255) );
	defColors.push_back( QColor(155,155,255) );
	defColors.push_back( QColor(224,255,64) );
	defColors.push_back( QColor(170,170,0) );
	defColors.push_back( QColor(170,170,0) );
	defColors.push_back( QColor(170,170,0) );
	defColors.push_back( QColor(170,170,0) );

        setGeometry(100,100,700,500);
	setUI();
}


GUIAttributes::~GUIAttributes()
{
}

void GUIAttributes::setUI( )
{
	QGridLayout *gl = new QGridLayout();
	
	lw = new QListWidget(this);
	gl->addWidget( lw,0,0);
	connect( lw, SIGNAL(currentItemChanged ( QListWidgetItem*, QListWidgetItem*)), this, SLOT(slotCurrentItemChanged ( QListWidgetItem*, QListWidgetItem*)) );
        lw->setMaximumWidth(150);
        lw->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred );
	
	buildSections();
	readSettings();
	
	at = new AttrTable(this);
	gl->addWidget( at,0,1,1,-1 );
        at->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding );
	
	QPushButton * pb;
	
	pb = new QPushButton( tr("OK"), this );
	gl->addWidget(pb,1,1);
	connect( pb, SIGNAL(clicked()), this, SLOT( slotOK() ) );
	
	pb = new QPushButton( tr("Cancel"), this );
	gl->addWidget(pb,1,2);
	connect( pb, SIGNAL(clicked()), this, SLOT( slotCancel() ) );
	
	setLayout( gl );
}

void GUIAttributes::saveSettings( )
{
        Attr * a;
	a = models[1]->getAttr( tr("Default Same Color") );
	if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/SingleColor"), QVariant(a->color()) );	
	
	a = models[1]->getAttr( tr("Viewer Background") );
	if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/ViewerBackground"), QVariant(a->color()) );	
	
	for (int i = 1; i <= 20; i++) {
		a = models[1]->getAttr( tr("Color %1").arg(i) );
		if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/Color%1").arg(i+1), QVariant(a->color()) );
	}	
	
        a = models[2]->getAttr( tr("vector.dont-miss-big-vectors"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/vector.dont-miss-big-vectors"), QVariant(a->value()) );
        a = models[2]->getAttr( tr("vector.lighting"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/vector.lighting"), QVariant(a->value()) );
        a = models[2]->getAttr( tr("vector.thickness"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/vector.thickness"), QVariant(a->value()) );
        a = models[2]->getAttr( tr("vector.head.length"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/vector.head.length"), QVariant(a->value()) );
        a = models[2]->getAttr( tr("vector.head.thickness"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/vector.head.thickness"), QVariant(a->value()) );
        a = models[2]->getAttr( tr("vector.head.faces"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/vector.head.faces"), QVariant(a->value()) );
        a = models[2]->getAttr( tr("vector.same.length"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/vector.same.length"), QVariant(a->value()) );

        a = models[3]->getAttr( tr("field types"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/field_types"), QVariant(a->value()));
        a = models[3]->getAttr( tr("finite elements types"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/finite_elements_types"), QVariant(a->value()));
        a = models[3]->getAttr( tr("approximation degrees"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/approximation_degrees"), QVariant(a->value()));
        a = models[3]->getAttr( tr("map"));
        if (a != 0) fvsettings.setValue( QString("/RSoft/FViewer/map"), QVariant(a->value()));

	fvsettings.sync();
}

void GUIAttributes::readSettings( )
{
	Attr * a;
	a = models[1]->getAttr( tr("Default Same Color") );

        if (a != 0) {
            QColor c = QColor::fromRgb(85,170,255,0);
            if ( fvsettings.contains(QString("/RSoft/FViewer/ViewerBackground")) ) {
                QVariant cVar = fvsettings.value( QString("/RSoft/FViewer/SingleColor") );
                c = cVar.value<QColor>();
            }
            a->setValue( c );
	}

	a = models[1]->getAttr( tr("Viewer Background") );
	if (a != 0) {
            QColor c = QColor::fromRgb(200,200,200,0);
            if ( fvsettings.contains(QString("/RSoft/FViewer/ViewerBackground")) ) {
                QVariant cVar = fvsettings.value( QString("/RSoft/FViewer/ViewerBackground") );
                c = cVar.value<QColor>();
            }
            a->setValue( c );
	}
		
	for (int i = 1; i <= 20; i++) {
		a = models[1]->getAttr( tr("Color %1").arg(i) );
		if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/Color%1").arg(i+1), QVariant(defColors[i-1])).value<QColor>() );
	}

        a = models[2]->getAttr( tr("vector.dont-miss-big-vectors"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/vector.dont-miss-big-vectors"), QVariant("true") ).toString() );
        a = models[2]->getAttr( tr("vector.lighting"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/vector.lighting"), QVariant("false") ).toString() );
        a = models[2]->getAttr( tr("vector.thickness"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/vector.thickness"), QVariant("0.04") ).toString() );
        a = models[2]->getAttr( tr("vector.head.length"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/vector.head.length"), QVariant("0.5") ).toString() );
        a = models[2]->getAttr( tr("vector.head.thickness"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/vector.head.thickness"), QVariant("0.2") ).toString() );
        a = models[2]->getAttr( tr("vector.head.faces"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/vector.head.faces"), QVariant("4") ).toString() );
        a = models[2]->getAttr( tr("vector.same.length"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/vector.same.length"), QVariant("true") ).toString() );

        a = models[3]->getAttr( tr("field types"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/field_types"), QVariant("scalar vector") ).toString() );
        a = models[3]->getAttr( tr("finite elements types"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/finite_elements_types"), QVariant("CG DG") ).toString() );
        a = models[3]->getAttr( tr("approximation degrees"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/approximation_degrees"), QVariant("0 1") ).toString() );
        a = models[3]->getAttr( tr("map"));
        if (a != 0) a->setValue(fvsettings.value( QString("/RSoft/FViewer/map"), QVariant("scalar DG 0 tetrascalar0\nscalar CG 1 tetrascalar1\nvector DG 0 tetravector0\nvector CG 1 tetravector1") ).toString() );

}

void GUIAttributes::slotOK( )
{
        for( QVector<AttrModel*>::iterator m = models.begin(); m != models.end(); m++){
            (*m)->update();
        }
	saveSettings();
	accept();
}

void GUIAttributes::slotCancel( )
{
	accept();
}

void GUIAttributes::slotCurrentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous )
{
	Q_UNUSED( previous );
	int iModel = current->data(1).toInt();
	
	if ((iModel >= 0) && (iModel < models.size()))
		at->setModel( models[iModel] );
	else
		qDebug("Invalid iModel = %d in GUIAttributes.", iModel);
	
}

void GUIAttributes::buildSections( )
{
	QListWidgetItem * it;
	AttrModel *m;
	
        //Paths
	m = new AttrModel();
	models.push_back( m );
	m->addAttr( tr("Last Grid Path"), QString("."), QString("text") );

	it = new QListWidgetItem(tr("Paths"),lw );
	it->setData(1,0);
        lw->addItem( it );
	
        // Colors
	m = new AttrModel();
	models.push_back( m );
	m->addAttr( tr("Default Same Color"), QColor(0,0,0), QString("color") );
	m->addAttr( tr("Viewer Background"), QColor(0,0,0), QString("color") );
	m->addSection( tr("Default Individual colors") );
	for (int i = 1; i <= 20; i++) {
		m->addAttr( tr("Color %1").arg(i), QColor(0,0,0), QString("color") );
	}

	it = new QListWidgetItem(tr("Colors"),lw );
	lw->addItem( it );
	it->setData(1,1);

        // Parameters
        m = new AttrModel();
        models.push_back( m );
        m->addAttr( tr("vector.dont-miss-big-vectors"), QString("true"), QString("text") );
        m->addAttr( tr("vector.lighting"), QString("false"), QString("text") );
        m->addAttr( tr("vector.thickness"), QString("0.5"), QString("text") );
        m->addAttr( tr("vector.head.length"), QString("0.5"), QString("text") );
        m->addAttr( tr("vector.head.thickness"), QString("0.2"), QString("text") );
        m->addAttr( tr("vector.head.faces"), QString("4"), QString("text") );
        m->addAttr( tr("vector.same.length"), QString("true"), QString("text") );

        it = new QListWidgetItem(tr("Parameters"),lw );
        it->setData(1,2);
        lw->addItem( it );

        // Wizard Properties
        m = new AttrModel();
        models.push_back( m );
        m->addAttr( tr("field types"), QString("scalar vector"), QString("text") );
        m->addAttr( tr("finite elements types"), QString("CG DG"), QString("text") );
        m->addAttr( tr("approximation degrees"), QString("0 1"), QString("text") );
        m->addAttr( tr("map"), QString("scalar DG 0 tetrascalar0\nscalar CG 1 tetrascalar1\nvector DG 0 tetravector0\nvector CG 1 tetravector1"), QString("text") );

        it = new QListWidgetItem(tr("Wizard Properties"),lw );
        it->setData(1,3);
        lw->addItem( it );
}
