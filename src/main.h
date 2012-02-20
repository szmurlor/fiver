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

#ifndef _FVMAIN_IS_INCLUDED
#define _FVMAIN_IS_INCLUDED

#include <fvsettings.h>

#define FV_DEFAULT_BOX_COLOR_DRAW QColor(255,255,162)
#define FV_DEFAULT_BOX_COLOR_SIMULATION QColor(192,100,192)
#define FV_DEFAULT_BOX_COLOR_GRID QColor(36,219,88)
#define FV_DEFAULT_BOX_COLOR_FIELD QColor(160,255,0)
#define FV_DEFAULT_BOX_COLOR_ANIMATION QColor(85,255,255)
#define FV_DEFAULT_BOX_COLOR_MANIPULATOR QColor(255,255,162)
#define FV_DEFAULT_BOX_COLOR_OPERATOR QColor(255,117,117)
#define FV_DEFAULT_BOX_X 10
#define FV_DEFAULT_BOX_Y 10
#define FV_DEFAULT_BOX_Width 150
#define FV_DEFAULT_BOX_Height 20

extern FVSettings fvsettings;
extern bool __DEBUG_FIVER;

#endif
