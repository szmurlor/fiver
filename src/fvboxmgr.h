/***************************************************************************
 *   Copyright (C) 2006 by Robert Szmurlo   *
 *   robert@iem.pw.edu.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef FVBOXMGR_H
#define FVBOXMGR_H

#include <boxmgr.h>

class FVBoxGrid;
class FVBoxSimulation;
class Grid;
class Field;
class FVBoxField;
class FVBoxFieldXml;
class FVFieldDraw;
class FVObject;
class FVFieldSlice;
class FVBoxCBlock;
class FVBoxSphere;
class FVBoxCoil;
class FVBoxSTL;
class FVGridToSTLExtractor;
class FVMeshJoiner;
class FVBoxXml;



/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
          Jacek Starzynski <jstar@iem.pw.edu.pl>
*/
class FVBoxMgr : public BoxMgr
{
private:
        int lastId;

public:
    FVBoxMgr(QWidget * parent);
    virtual ~FVBoxMgr();

    /** Factory of methods for creating new objects. They should be defined only
        for containers components. */
    FVBoxGrid * addBoxGrid(FVObject * parent, Grid *g, QString filename, QString type );
    FVBoxSimulation * addBoxSimulation( QString filename, QString type );
    FVBoxField * addBoxField( FVObject * parent, Field *f, QString name, QString type );
    FVBoxField * addBoxField( Field * f, QString name, QString type );
    FVBoxCBlock * addBoxCBlock( QString filename, QString type );
    FVBoxSphere * addBoxSphere( QString type );
    FVBoxCoil * addBoxCoil( QString type );
    FVBoxSTL * addBoxSTL(FVObject * parent, Grid *g, QString filename, QString type );
    FVMeshJoiner * addMeshJoiner( QString type );
    FVGridToSTLExtractor * addGridToSTLExtractor(FVBoxGrid *parentBoxGrid);

    /** Returns the first found parent of BoxBox * child. */
    FVObject * getParentOf( FVObject * child ); // python
	
    void sendMessageToChildrenOf( const QList<QString> &argv, FVObject * receiver );
    void sendMessage( const QList<QString> &argv, FVObject * receiver, bool populateToChildren = false );
    void sendMessage( const QString &arg, FVObject * receiver, bool populateToChildren = false );
	

    /* Export do pythona */
    FVObject * find( QString name );
    FVObject * find( QString type, QString name );
    FVObject * find( int id );
    QString createScript(QString filename);
    void autoArrangeChildren( FVObject * parent ); // python


    QString getNameFor(QString s);
};

#endif
