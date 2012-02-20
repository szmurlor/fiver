#ifndef XDRVLIB_H
#define XDRVLIB_H
/*|--------------------------------------------------------------------------

 MAGELLAN X-Window Driver Header                        Version 4.00 09.06.97

 Copyright (C) 1984, 2002 3Dconnexion GmbH / Inc.

 Edited by Robert Szmurlo szmurlor@iem.pw.edu.pl

 An der Hartmuehle 8
 D - 82229 Seefeld

 mailto:development@3dconnexion.com

---------------------------------------------------------------------------|*/
#include <QtGlobal>
#include <X11/Xlib.h>

#ifndef TRUE
#define TRUE	(1==1)
#endif				/*  */

#ifndef FALSE
#define FALSE	(1==0)
#endif				/*  */


/* Data Structure for CADDS5 implementation */
#define MagellanInputMotionEvent 1
#define MagellanInputButtonPressEvent 2
#define MagellanInputButtonReleaseEvent 3

union _MagellanInputEventUnion_ {
    int data[7];
    int button;
};
typedef union _MagellanInputEventUnion_ MagellanIntUnion;
struct _MagellanIntEvent_ {
    int type;
    MagellanIntUnion u;
};
typedef struct _MagellanIntEvent_ MagellanIntEvent;

/* Data Structure for Pro/ENGINEER implementation */
enum _MagellanData_ { MagellanX, MagellanY, MagellanZ, MagellanA, MagellanB, MagellanC };
struct _MagellanFloatEvent_ {
    int MagellanType;
    int MagellanButton;
    double MagellanData[6];
    int MagellanPeriod;
};
typedef struct _MagellanFloatEvent_ MagellanFloatEvent;
int MagellanInit( Display * display, Window window);
int MagellanSetWindow( Display * display, Window window); 
int MagellanApplicationSensitivity( Display * display, double Sensitivity);
int MagellanInputEvent( Display * display, XEvent * Event, MagellanIntEvent * MagellanEvent);
int MagellanTranslateEvent( Display * display, XEvent * Event,
			      MagellanFloatEvent * MagellanEvent,
			      double MagellanTranslationScale,
			      double MagellanRotationScale);
int MagellanClose( Display * display);
Bool MagellanCheckMotionEvent( Display * display, XEvent * Event, char *Arg);
int MagellanRemoveMotionEvents( Display * display);
int MagellanRotationMatrix( double Rotate[4][4], double c, double b, double a);
int MagellanMultiplicationMatrix(
				    double MatrixA[4][4],
				    double MatrixB[4][4],
				    double MatrixC[4][4]
    );
extern Atom MagellanMotionEvent, MagellanButtonPressEvent, MagellanReleaseEvent;
extern Window MagellanWindow;
extern int MagellanExist;
enum _CommandMessages_ { NoCommandMessage, CommandMessageApplicationWindow = 27695, CommandMessageApplicationSensitivity };
union _MagellanTypeConversion_ {
    float Float;
    short Short[2];
};
typedef union _MagellanTypeConversion_ MagellanTypeConversion;

#define XHigh32( Value )        (((Value)>>16)&0x0000FFFF)
#define XLow32( Value )         ((Value)&0x0000FFFF)

#endif				/* Magellan X Window Driver Header */
