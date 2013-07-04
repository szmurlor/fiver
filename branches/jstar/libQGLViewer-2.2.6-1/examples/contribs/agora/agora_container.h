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
// Time-stamp: <23 Jun 03 10:42:45 Jean-Guillaume.Dumas@imag.fr>
// =================================================================== //
#ifndef __Agora_Container__
#define __Agora_Container__

// #include <vector>
// typedef std::vector AgoraContainer;

#include <iostream>

template<int POSS_SIZE, typename Elem> struct AgoraContainer {
private:
    typedef AgoraContainer<POSS_SIZE, Elem> Self_t;
    Elem _container[ POSS_SIZE ];
    Elem * _finish;
public:
    typedef Elem* iterator;
    typedef const Elem* const_iterator;
    AgoraContainer() : _finish(_container) { }
    AgoraContainer(int s) : _finish(_container+s) { }
    Case_t size() const { return Case_t(_finish - _container); }
    void clear() { _finish = _container; }
    void resize(int s) { _finish = _container + s; }
    iterator begin() { return _container; }
    iterator end() { return _finish; }
    const_iterator begin() const { return const_iterator(_container); }
    const_iterator end() const { return const_iterator(_finish); }
    void push_back(const Elem& c) {
        *_finish = c; ++_finish;
    }
};

#endif
