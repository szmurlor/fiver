/*|--------------------------------------------------------------------------

 05-Oct-95  G-01-08  VS   $$1 Magellan 3D Controller support for X Windows

 MAGELLAN X-Window Driver Library                       Version 4.00 09.06.97

 Copyright (C) 1984, 2002 3Dconnexion GmbH / Inc.

 An der Hartmuehle 8
 D - 82229 Seefeld

 mailto:development@3dconnexion.com

----------------------------------------------------------------------------|*/

#include <QtGlobal>
#include <QDebug>

#include <string.h>
#include <math.h>
#include <stdio.h>
#ifdef Q_WS_X11


#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#define ParameterCheck
#define MagellanDEBUG
#include "xdrvlib.h"

/* Define printf Function */
#define cprintf printf
Atom MagellanMotionEvent;	/* MotionEvent */
Atom MagellanButtonPressEvent;	/* ButtonPressEvent */
Atom MagellanButtonReleaseEvent;	/* ButtonReleaseEvent */
Atom MagellanCommandEvent;	/* CommandEvent */
Window MagellanWindow = InputFocus;	/* Magellan Driver Window */
int MagellanExist = FALSE;	/* Magellan Exist Flag */

int MagellanInit(Display * display, Window window)

{

/* Define the Magellan Event types */
    MagellanMotionEvent = XInternAtom(display, "MotionEvent", TRUE);
    MagellanButtonPressEvent =
	XInternAtom(display, "ButtonPressEvent", TRUE);
    MagellanButtonReleaseEvent =
	XInternAtom(display, "ButtonReleaseEvent", TRUE);
    MagellanCommandEvent = XInternAtom(display, "CommandEvent", TRUE);

/* TRUE if Driver exist */
    MagellanExist = (MagellanMotionEvent != 0)
	&& (MagellanButtonPressEvent != 0)
	&& (MagellanButtonReleaseEvent != 0)
	&& (MagellanCommandEvent != 0);
    if (MagellanExist == FALSE)
	return FALSE;
    MagellanExist = MagellanSetWindow(display, window);
    if (MagellanExist == FALSE)
	return FALSE;

   return MagellanExist;
}


int MagellanApplicationSensitivity( Display *display, double Sensitivity)
{
    XTextProperty MagellanWindowName;
    XEvent CommandMessage;
    Atom ActualType;
    int ActualFormat;
    unsigned long NItems, BytesReturn;
    unsigned char *PropReturn;
    Window root;
    MagellanTypeConversion MagellanType;
    int result = FALSE;

/* Read the window of the Magellan Driver */
    root = RootWindow(display, DefaultScreen(display));

    PropReturn = NULL;
    XGetWindowProperty(display, root, MagellanCommandEvent, 0, 1, FALSE,
		       AnyPropertyType, &ActualType, &ActualFormat,
		       &NItems, &BytesReturn, &PropReturn);
    MagellanWindow = InputFocus;
    if (PropReturn != NULL) {
	MagellanWindow = *(Window *) PropReturn;
	XFree(PropReturn);

	if (XGetWMName(display, MagellanWindow, &MagellanWindowName)
	    != 0) {

	    if (strcmp
		((char *) "Magellan Window",
		 (char *) MagellanWindowName.value) == 0) {

/* Send the application window to the Magellan X Window Driver */
		CommandMessage.type = ClientMessage;
		CommandMessage.xclient.format = 16;
		CommandMessage.xclient.send_event = FALSE;
		CommandMessage.xclient.display = display;
		CommandMessage.xclient.window = MagellanWindow;
		CommandMessage.xclient.message_type = MagellanCommandEvent;
		MagellanType.Float = (float) Sensitivity;
		CommandMessage.xclient.data.s[0] =
		    (short) MagellanType.Short[0];
		CommandMessage.xclient.data.s[1] =
		    (short) MagellanType.Short[1];
		CommandMessage.xclient.data.s[2] =
		    CommandMessageApplicationSensitivity;
		result =
		    XSendEvent(display, MagellanWindow, FALSE, 0x0000,
			       &CommandMessage) != 0;

		XFlush(display);
	    };
	};
    };
    //XSetErrorHandler(ApplicationErrorHandler);
    return result;
}

int MagellanSetWindow(
Display *display,
Window window)

{
    XTextProperty MagellanWindowName;
    XEvent CommandMessage;
    Atom ActualType;
    int ActualFormat;
    unsigned long NItems, BytesReturn;
    unsigned char *PropReturn;
    Window root;
    int result;

    result = FALSE;

/* Read the window of the Magellan Driver */
    root = RootWindow(display, DefaultScreen(display));

    PropReturn = NULL;
    XGetWindowProperty(display, root, MagellanCommandEvent, 0, 1, FALSE,
		       AnyPropertyType, &ActualType, &ActualFormat,
		       &NItems, &BytesReturn, &PropReturn);
    MagellanWindow = InputFocus;
    if (PropReturn != NULL) {
	MagellanWindow = *(Window *) PropReturn;
	XFree(PropReturn);

	if (XGetWMName(display, MagellanWindow, &MagellanWindowName)
	    != 0) {

	    if (strcmp
		((char *) "Magellan Window",
		 (char *) MagellanWindowName.value) == 0) {

		/* Send the application window to the Magellan X Window Driver */
        qDebug() << "Registering Magellan...";
		CommandMessage.type = ClientMessage;
		CommandMessage.xclient.format = 16;
		CommandMessage.xclient.send_event = FALSE;
		CommandMessage.xclient.display = display;
		CommandMessage.xclient.window = MagellanWindow;
		CommandMessage.xclient.message_type = MagellanCommandEvent;
		CommandMessage.xclient.data.s[0] = (short) XHigh32(window);
		CommandMessage.xclient.data.s[1] = (short) XLow32(window);
		CommandMessage.xclient.data.s[2] =
		    CommandMessageApplicationWindow;
		result =
		    XSendEvent(display, MagellanWindow, FALSE, 0x0000,
			       &CommandMessage) != 0;

		XFlush(display);
	    };
	};
    }
    return result;
}

int MagellanClose(Display *display)

{
    MagellanInit(display, (Window) InputFocus);
    MagellanExist = FALSE;
    return TRUE;
}

int MagellanInputEvent( Display *display, XEvent *Event, MagellanIntEvent *MagellanEvent)
{

    if (MagellanExist == FALSE)
	return FALSE;
    if (Event->type == ClientMessage) {
	if (Event->xclient.message_type == MagellanMotionEvent) {
	    MagellanEvent->type = MagellanInputMotionEvent;
	    MagellanEvent->u.data[0] = Event->xclient.data.s[2];
	    MagellanEvent->u.data[1] = Event->xclient.data.s[3];
	    MagellanEvent->u.data[2] = Event->xclient.data.s[4];
	    MagellanEvent->u.data[3] = Event->xclient.data.s[5];
	    MagellanEvent->u.data[4] = Event->xclient.data.s[6];
	    MagellanEvent->u.data[5] = Event->xclient.data.s[7];
	    MagellanEvent->u.data[6] =
		Event->xclient.data.s[8] * 1000 / 60;
	    return MagellanInputMotionEvent;
	}

	else if (Event->xclient.message_type == MagellanButtonPressEvent) {
	    MagellanEvent->type = MagellanInputButtonPressEvent;
	    MagellanEvent->u.button = Event->xclient.data.s[2];
	    return MagellanInputButtonPressEvent;
	}

	else if (Event->xclient.message_type == MagellanButtonReleaseEvent) {
	    MagellanEvent->type = MagellanInputButtonReleaseEvent;
	    MagellanEvent->u.button = Event->xclient.data.s[2];
	    return MagellanInputButtonReleaseEvent;
	};
    };
    return display == NULL;
}

int
MagellanTranslateEvent( Display *display, XEvent *Event, MagellanFloatEvent *MagellanEvent, double MagellanTranslationScale, double MagellanRotationScale)
{
    if (MagellanExist == FALSE)
	return FALSE;
    if (Event->type == ClientMessage) {
	if (Event->xclient.message_type == MagellanMotionEvent) {
	    MagellanEvent->MagellanType = MagellanInputMotionEvent;
	    MagellanEvent->MagellanData[MagellanX] =
		Event->xclient.data.s[2] * MagellanTranslationScale;
	    MagellanEvent->MagellanData[MagellanY] =
		Event->xclient.data.s[3] * MagellanTranslationScale;
	    MagellanEvent->MagellanData[MagellanZ] =
		Event->xclient.data.s[4] * MagellanTranslationScale;
	    MagellanEvent->MagellanData[MagellanA] =
		Event->xclient.data.s[5] * MagellanRotationScale;
	    MagellanEvent->MagellanData[MagellanB] =
		Event->xclient.data.s[6] * MagellanRotationScale;
	    MagellanEvent->MagellanData[MagellanC] =
		Event->xclient.data.s[7] * MagellanRotationScale;
	    MagellanEvent->MagellanPeriod = Event->xclient.data.s[8];
	    return MagellanInputMotionEvent;
	}

	else if (Event->xclient.message_type == MagellanButtonPressEvent) {
	    MagellanEvent->MagellanType = MagellanInputButtonPressEvent;
	    MagellanEvent->MagellanButton = Event->xclient.data.s[2];
	    return MagellanInputButtonPressEvent;
	}

	else if (Event->xclient.message_type == MagellanButtonReleaseEvent) {
	    MagellanEvent->MagellanType = MagellanInputButtonReleaseEvent;
	    MagellanEvent->MagellanButton = Event->xclient.data.s[2];
	    return MagellanInputButtonReleaseEvent;
	};
    };
    return display == NULL;
}

int MagellanRotationMatrix(
double Rotate[4][4],
double c, double b, double a)

{
    double ca, cb, cc, sa, sb, sc;
    ca = cos(a);
    sa = sin(a);
    cb = cos(b);
    sb = sin(b);
    cc = cos(c);
    sc = sin(c);
    Rotate[0][0] = ca * cb;
    Rotate[1][0] = sa * cb;
    Rotate[2][0] = -sb;
    Rotate[0][1] = -sa * cc + ca * sc * sb;
    Rotate[1][1] = ca * cc + sa * sc * sb;
    Rotate[2][1] = cb * sc;
    Rotate[0][2] = sa * sc + ca * cc * sb;
    Rotate[1][2] = -ca * sc + sa * cc * sb;
    Rotate[2][2] = cb * cc;
    Rotate[0][3] = 0.0;
    Rotate[1][3] = 0.0;
    Rotate[2][3] = 0.0;
    Rotate[3][0] = 0.0;
    Rotate[3][1] = 0.0;
    Rotate[3][2] = 0.0;
    Rotate[3][3] = 1.0;
    return TRUE;
}

int MagellanMultiplicationMatrix(
double MatrixA[4][4], double MatrixB[4][4], double MatrixC[4][4])

{
    double MatrixR[4][4];
    MatrixR[0][0] =
	MatrixB[0][0] * MatrixC[0][0] + MatrixB[0][1] * MatrixC[1][0] +
	MatrixB[0][2] * MatrixC[2][0];
    MatrixR[0][1] =
	MatrixB[0][0] * MatrixC[0][1] + MatrixB[0][1] * MatrixC[1][1] +
	MatrixB[0][2] * MatrixC[2][1];
    MatrixR[0][2] =
	MatrixB[0][0] * MatrixC[0][2] + MatrixB[0][1] * MatrixC[1][2] +
	MatrixB[0][2] * MatrixC[2][2];
    MatrixR[1][0] =
	MatrixB[1][0] * MatrixC[0][0] + MatrixB[1][1] * MatrixC[1][0] +
	MatrixB[1][2] * MatrixC[2][0];
    MatrixR[1][1] =
	MatrixB[1][0] * MatrixC[0][1] + MatrixB[1][1] * MatrixC[1][1] +
	MatrixB[1][2] * MatrixC[2][1];
    MatrixR[1][2] =
	MatrixB[1][0] * MatrixC[0][2] + MatrixB[1][1] * MatrixC[1][2] +
	MatrixB[1][2] * MatrixC[2][2];
    MatrixR[2][0] =
	MatrixB[2][0] * MatrixC[0][0] + MatrixB[2][1] * MatrixC[1][0] +
	MatrixB[2][2] * MatrixC[2][0];
    MatrixR[2][1] =
	MatrixB[2][0] * MatrixC[0][1] + MatrixB[2][1] * MatrixC[1][1] +
	MatrixB[2][2] * MatrixC[2][1];
    MatrixR[2][2] =
	MatrixB[2][0] * MatrixC[0][2] + MatrixB[2][1] * MatrixC[1][2] +
	MatrixB[2][2] * MatrixC[2][2];
    MatrixR[0][3] = MatrixR[1][3] = MatrixR[2][3] = MatrixR[3][0] =
	MatrixR[3][1] = MatrixR[3][2] = 0.0;
    MatrixR[3][3] = 1.0;
    memcpy(MatrixA, MatrixR, sizeof(MatrixA));
    return TRUE;
}

Bool MagellanCheckMotionEvent( Display * , XEvent *event, char * )

{
    if (event->type == ClientMessage)
	if (event->xclient.message_type == MagellanMotionEvent)
	    return True;
    return False;
}

int MagellanRemoveMotionEvents(
Display *display)

{
    XEvent event;
    while (XCheckIfEvent(display, &event, MagellanCheckMotionEvent, NULL) == True) ;
    return TRUE;
}
#endif
