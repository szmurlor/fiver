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
#ifndef FVGRIDINTERFACE_H
#define FVGRIDINTERFACE_H

#include <grid.h>
#include <fvinterface.h>
#include <dolfin/mesh/Mesh.h>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVGridInterface : public FVInterface {
public:
	FVGridInterface( Grid ** gridPtr);
        FVGridInterface( dolfin::Mesh * meshPtr);
        FVGridInterface( dolfin::BoundaryMesh * bmeshPtr);
        virtual ~FVGridInterface();
	
	virtual Grid * getGrid();
        virtual dolfin::Mesh * getMesh();
        virtual dolfin::BoundaryMesh * getBoundaryMesh();

private:
	// Pointer to the pointer
	Grid **gp;
  dolfin::Mesh * mp;
  dolfin::BoundaryMesh* bmp;
	
};

#endif
