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
#ifndef FVBOXFIELD_H
#define FVBOXFIELD_H

#include <fvobject.h>
#include <fvboxmgr.h>
#include <grid.h>
#include <field.h>
#include <fvfieldinterface.h>
#include <fvopener.h>

/**
	The class is a container for the scalar or vector fields. 
	It implements FVFieldInterface and should provide 
	the FVGridInterface from its parent component.

	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVBoxField : public FVObject
{
	Q_OBJECT
    
public:
	FVBoxField(FVBoxMgr * manager, Field * f, int x = 0, int y = 0);
	virtual ~FVBoxField();

	virtual QString classType() { return QString("FVBoxField"); }
	virtual FVInterface * getInterface( QString interfaceName );

	virtual void update();
        virtual QString getName();

        void setOpener(FVOpener *opener) {
            this->fvOpener = opener;
        }
        FVOpener * getOpener() {
            return this->fvOpener;
        }

public slots:
	void slotDrawColormap();
	void slotDrawVectors();
	void slotAnimate();	
	void slotSlice();
        void slotReload();

protected:
	Field * field;
	FVFieldInterface * fvFieldInterface;
        FVOpener * fvOpener;

	virtual void setupAttributes();
        virtual void setupAttributesMinMax();
	virtual void setupMenu();
	virtual bool message( const QList<QString> &argv );

};

#endif
