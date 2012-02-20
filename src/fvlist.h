//
// C++ Interface: fvlist
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVLIST_H
#define FVLIST_H

#include <QString>
#include <QGLWidget>

class FViewer;

/**
@author Robert Szmurlo
*/
class FVList{
protected:
    FViewer * parent;
	int id;
	
public:
    QString name;
	
    bool bVisible;
    GLfloat tx,ty,tz;
    GLfloat rx,ry,rz;
    
    FVList(FViewer * fv);
    ~FVList();
    
    void start();
    void end();
	void call();
    
    void setCoords();
};

#endif
