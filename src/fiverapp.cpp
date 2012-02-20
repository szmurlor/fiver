//
// C++ Implementation: fviewerapp
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
#include "fiverapp.h"
#include "fivermain.h"
#include "main.h"

#ifdef Q_OS_UNIX

    #ifdef ENABLE_MAGELLAN
        #include "./magellan/xdrvlib.h"
    #endif

    #include <X11/Xlib.h>
    #include <QX11Info>
#endif




FiverApp::FiverApp(int argc, char ** argv)
: QApplication( argc, argv )
{
    _argc = argc;
    _argv = argv;
    setApplicationName(__FIVER_NAME__);
}


FiverApp::~FiverApp()
{
}

int FiverApp::exec( )
{    
        if (interpArgs(_argc,_argv) == 0) {
		
            FiverMain * fm = new FiverMain();
            fm->show();
            QByteArray geom = fvsettings.value("/RSoft/FViewer/FiverMainGeometry","").toByteArray();
            fm->restoreGeometry(geom);

            connect( this, SIGNAL(lastWindowClosed()), this, SLOT(quit()) );

            #ifdef Q_OS_UNIX
            #ifdef ENABLE_MAGELLAN
                    /*** Magellan intialization: */
                Display * xdisplay;
                Window xwindow;

                QWidget * w = QApplication::allWidgets().first();

                xdisplay = w->x11Info().display();
                xwindow = (Window) w->x11PictureHandle();
                int res = FALSE;
                if ( (res = MagellanInit(xdisplay, xwindow)) == FALSE )
                    qDebug() << "Error! Caninitialize Magellan!";
                else {
                    qDebug() << "Magellan registered successfully with result = " << res;
                    connect( this, SIGNAL( signalMagellan(float,float,float,float,float,float) ), fm, SLOT( slotMagellan(float,float,float,float,float,float) ) );
                }
            #endif
            #endif

            return QApplication::exec();
	} else
		return 0;
}

int FiverApp::interpArgs( int argc, char ** argv )
{
	int iarg;	

	for (iarg = 1; iarg < argc; iarg++) {
		if (strcmp(argv[iarg],"-d") == 0) {
			__DEBUG_FIVER = true;
		}
		
/*
		if (strcmp(argv[iarg],"-db") == 0) {
			fv->openDb( QString( argv[++iarg] ) );
		}
*/
		
		if ( (strcmp(argv[iarg],"-h") == 0) ||
			 (strcmp(argv[iarg],"--help") == 0) ) {
			printHelp();
			return (1);
		}
		
		iarg++;
	}
	return (0);
}

void FiverApp::printHelp( )
{
	printf("fiver arguments:\n");
	printf("-h | --help: show this help text\n");
	printf("-d         : display debug messages\n");
	//printf("-db simu.db: load database with the name of the database file: simu.db\n");
}

#ifdef Q_OS_UNIX
bool FiverApp::x11EventFilter(XEvent * event)
{
    // qDebug() << "eventFilter: " << event->type;

#ifdef ENABLE_MAGELLAN
	if (event->type == ClientMessage) {
		MagellanFloatEvent MagellanEvent;
		Display * xdisplay;
		QWidget * w = QApplication::allWidgets().first();
		xdisplay = w->x11Info().display();
		
		if (xdisplay != 0 ) {
			MagellanTranslateEvent (xdisplay, event, &MagellanEvent, 1.0, 1.0);
            /*
            qDebug() << "x = " << MagellanEvent.MagellanData[MagellanX] <<
					"y = " << MagellanEvent.MagellanData[MagellanY] <<
					"z = " << MagellanEvent.MagellanData[MagellanZ] <<
					"A = " << MagellanEvent.MagellanData[MagellanA] <<
					"B = " << MagellanEvent.MagellanData[MagellanB] <<
					"C = " << MagellanEvent.MagellanData[MagellanC];
            */

			emit signalMagellan( 	MagellanEvent.MagellanData[MagellanX], 
						MagellanEvent.MagellanData[MagellanY], 
						MagellanEvent.MagellanData[MagellanZ], 
						MagellanEvent.MagellanData[MagellanA], 
						MagellanEvent.MagellanData[MagellanB], 
						MagellanEvent.MagellanData[MagellanC]);
		}
	}
#endif
	return false;
}
#endif
