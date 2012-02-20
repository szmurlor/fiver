/***************************************************************************
 *   Copyright (C) 2006 by Robert Szmurlo   *
 *   robert@iem.pw.edu.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef FVOPENERDIFFPACKSIM_H
#define FVOPENERDIFFPACKSIM_H

#include <fvopener.h>

/**
	@author Robert Szmurlo <robert@iem.pw.edu.pl>
*/
class FVOpenerDiffpackSim : public FVOpener
{
public:
	FVOpenerDiffpackSim();
	FVOpenerDiffpackSim(QObject * parent);

    ~FVOpenerDiffpackSim();

    virtual FVObject* open( FVBoxMgr * bm, QString fname, int type );
};

#endif
