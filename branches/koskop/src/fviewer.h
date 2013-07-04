//
// C++ Interface: fviewer
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVIEWER_H
#define FVIEWER_H


#include <QGLViewer/qglviewer.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFont>
#include <QString>
#include "fvlist.h"
#include "fvinteractionmodel.h"

class FVFieldSlice;
class FVField;

struct Label
{
    int x, y;
    QString text;
};

/**
@author Robert Szmurlo
*/
class FViewer : public QGLViewer
{
private:
    qglviewer::Vec rotate( float x, float y, float z, float u, float v, float w, float a);

public:
    FViewer();
    FViewer(QWidget * parent);
    FViewer(const FViewer & fv);
    ~FViewer();

    void init();
	
    virtual void draw();
    virtual void drawWithNames();
    void minit();
    virtual QString helpString() const;

    /** List management operations */
    void add( FVList * fl );
    void remove( QString &name );
    void remove( FVList* what );
    FVList * find( QString &name );

    void fvRenderText( float x, float y, float z, QString str );
    void fvRenderText( int x, int y, QString str );

    void setInteractionModel( FVInteractionModel * i );

    void repaint(); // python
    void snapshot(QString filename);
    void rotx( double ang );
    void roty( double ang );
    void rotz( double ang );
    void trans( double x, double y, double z );
    void addLabel( int x, int y, QString text);

protected:
    std::list<FVList*> lists;
    std::list<Label*> labels;
    QFont *glfont;
    FVInteractionModel * fvinteraction;
    bool bFirstInit;

    virtual void mousePressEvent( QMouseEvent * e );
    virtual void mouseMoveEvent( QMouseEvent * e );
    virtual void mouseReleaseEvent( QMouseEvent * e );
    virtual void keyPressEvent( QKeyEvent * e );

    void fvDrawAxis();
    void fvDrawLabels();
    void updateList();
};

#endif
