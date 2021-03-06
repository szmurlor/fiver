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
#ifndef FVREQUIREGRID_H
#define FVREQUIREGRID_H

#include <fvobject.h>
#include <fvgridinterface.h>
#include <grid.h>
#include <dolfin/mesh/Mesh.h>
#include <dolfin/mesh/BoundaryMesh.h>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVRequireGrid{
public:
	FVRequireGrid();
	~FVRequireGrid();

	Grid * getGrid( FVObject * fvo, FVObject * alternateObject = 0 );
        dolfin::Mesh * getMesh( FVObject * fvo, FVObject * alternateObject = 0 );
        dolfin::BoundaryMesh * getBoundaryMesh( FVObject * fvo, FVObject * alternateObject = 0 );
};

#endif
