//
// C++ Interface: setofint
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SETOFINT_H
#define SETOFINT_H

#include <set>
#include <qstring.h>

/**
Stores set of integer numbers. 
Allows initialization from a QString, with range restrictions.

@author Jacek Starzynski
*/
class SetOfInt{
protected:
	std::set< int > s;
// do sprawdzenia
//	__gnu_cxx::hash_set< int, __gnu_cxx::hash< int >, __gnu_cxx::equal_to< int > > s;

public:
	SetOfInt() {};
	SetOfInt( QString s, int min, int max );
	~SetOfInt();

	void init( QString s, int min, int max );

	bool find( int );
        int sum();
        int size() { return s.size(); }
        std::set<int>::iterator begin() { return s.begin(); }
        std::set<int>::iterator end() { return s.end(); }
};

#endif
