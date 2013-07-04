#ifndef _TRI_H_IS_INCLUDED
#define _TRI_H_IS_INCLUDED

#include "point.h"
#include <vector>


/**
 * Pure geometric class.
*/
class Tri {
	public:
		Point p[3];
		std::vector<double> v[3];
};

#endif
