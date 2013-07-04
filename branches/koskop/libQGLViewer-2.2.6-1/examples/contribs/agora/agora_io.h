/****************************************************************************

 Copyright (C) 2002-2006 Gilles Debunne (Gilles.Debunne@imag.fr)

This file is part of the QGLViewer library.
 Version 2.2.6-1, released on July 4, 2007.

 http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer

 libQGLViewer is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 libQGLViewer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with libQGLViewer; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

// =================================================================== //
// Time-stamp: <24 Jun 03 14:51:09 Jean-Guillaume.Dumas@imag.fr>
// =================================================================== //
#ifndef __Agora_IO_H__
#define __Agora_IO_H__

#include "agora_class.h"

template<class Ints>
std::ostream& operator<< (std::ostream& out, const Agora<Ints>& p) ;

template<class Ints>
std::ostream& details (std::ostream& out, const Agora<Ints>& p) ;

template<class Ints>
std::ostream& affiche (std::ostream& out, const Agora<Ints>& p) ;

template<class Ints>
std::istream& operator>> (std::istream& in, Agora<Ints>& p) ;

#endif
