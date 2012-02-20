//
// C++ Implementation: fvlist
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fvlist.h"
#include "fviewer.h"

FVList::FVList(FViewer * fv)
{
    parent = fv;
    fv->makeCurrent();
    id = glGenLists(1);
    bVisible = true;
    tx = ty =tz = 0;
    rx = ry = rz =0;
}


FVList::~FVList()
{
    parent->makeCurrent();
    glDeleteLists( id, 1 );
    
    parent->remove( this );
    parent->update();
}

void FVList::start()
{
    parent->makeCurrent();
    glNewList( id, GL_COMPILE_AND_EXECUTE );
}

void FVList::end()
{
    glEndList();
}

void FVList::setCoords( )
{
    glTranslatef(tx,ty,tz);
    if (rx != 0) glRotatef(rx,1,0,0);
    if (ry != 0) glRotatef(ry,0,1,0);
    if (rz != 0) glRotatef(rz,0,0,1);
}

void FVList::call( )
{
	setCoords();
	glCallList(id);
}
