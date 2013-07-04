/*****************************************************************************
 *   Copyright (C) 2006 Robert Szmurlo <robert@iem.pw.edu.pl>                *
 *                                                                           *
 *   Licensed under the Apache License, Version 2.0 (the "License");         *
 *   you may not use this file except in compliance with the License.        *
 *   You may obtain a copy of the License at                                 *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *   Unless required by applicable law or agreed to in writing, software     *
 *   distributed under the License is distributed on an "AS IS" BASIS,       *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 *   See the License for the specific language governing permissions and     *
 *   limitations under the License.                                          *
 *****************************************************************************/



#include <fiverapp.h>
#include "main.h"
#include "fivermain.h"
#include <cstdio>
#include <QDebug>

FVSettings fvsettings("ZETiIS","FIVER2");
bool __DEBUG_FIVER;

void FIVERMessageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
		case QtDebugMsg:
			if (__DEBUG_FIVER)
				fprintf(stderr, "Debug: %s\n", msg);
			break;
		case QtWarningMsg:
			fprintf(stderr, "Warning: %s\n", msg);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "Critical: %s\n", msg);
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s\n", msg);
			abort();
	}
}

int main( int argc, char ** argv ) {
	__DEBUG_FIVER = false;
	
	//qInstallMsgHandler(FIVERMessageOutput);
	
        //qDebug() << "Konstruuje...";
        FiverApp app( argc, argv );
        //qDebug() << "Skontruowa³em... i uruchamiam g³ówn¹ pêtlê obs³ugi komunikatów...";

        return app.exec();
        return 0;
}
