//
// C++ Implementation: setofint
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "setofint.h"
#include <qstringlist.h>

SetOfInt::SetOfInt(QString str, int min, int max)
{
	init( str, min, max );
}

SetOfInt::~SetOfInt()
{
}

void SetOfInt::init( QString str, int min, int max )
{
	s.clear();
	if( str.length() > 0 ) {
		QStringList vSL( str.split(' ') );
		QStringList::iterator vb;
		for (vb = vSL.begin(); vb != vSL.end(); vb++) {
			if( (*vb).contains('-') == 1 ) {
				int be= ((*vb).section('-',0,0)).toInt();
				int en= ((*vb).section('-',1,1)).toInt();
				for( int i= be; i <= en; i++ )
					if ( i <= max && i >= min ) {
						s.insert(i);
					}
			} else {
				int i = (*vb).toInt();
				if ( i <= max && i >= min ) {
					s.insert(i);
				}
			}
		}
	}
}

bool SetOfInt::find( int i )
{
	return s.find( i ) != s.end();
}


int SetOfInt::sum()
{
    return s.size();
}
