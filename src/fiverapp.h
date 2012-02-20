//
// C++ Interface: fviewerapp
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FVIEWERAPP_H
#define FVIEWERAPP_H

#include <QApplication>

class FiverMain;

/**
@author Robert Szmurlo
*/
class FiverApp : public QApplication {
	
	Q_OBJECT 
			
protected:
    int _argc;
    char **_argv;
    FiverMain * fm;
    
public:
    FiverApp(int argc, char ** argv);

	virtual ~FiverApp();
	virtual int exec();
#ifdef Q_WS_X11
        virtual bool x11EventFilter( XEvent * );
#endif
	void printHelp();
	int interpArgs( int argc, char ** argv);
	
signals:
	void signalMagellan( float x, float y, float z, float A, float B, float C );
};

#endif
