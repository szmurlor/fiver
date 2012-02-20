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
// Time-stamp: <20 Jun 03 18:45:45 Jean-Guillaume.Dumas@imag.fr>
// =================================================================== //
#ifndef __Agora_Coup__
#define __Agora_Coup__
#include "agora_types.h"
#include <iostream>

struct AgoraCoup {
    AgoraCoup() {}
    AgoraCoup(Case_t d, Case_t a, bool b)
            : _depart(d), _arrivee(a), _dessus(b) {}

    Case_t depart() const { return _depart; }
    Case_t arrivee() const { return _arrivee; }
    bool dessus() const { return _dessus; }



    void copy( const AgoraCoup& c) {
        _depart = c._depart;
        _arrivee = c._arrivee;
        _dessus = c._dessus;
    }

    bool operator==( const AgoraCoup& c) {
        return (    (_depart  == c._depart)
                    && (_arrivee == c._arrivee)
                    && (_dessus  == c._dessus)   );
    }

    friend std::ostream& operator<< (std::ostream& out, const AgoraCoup& c) {
        return out << (Size_t)(c._depart) << (c._dessus?"/":"\\") << (Size_t)(c._arrivee);
    }

    friend std::istream& operator>> (std::istream& in, AgoraCoup& c) {
        char s; Size_t d, a;
        in >> d >> s >> a;
        c._depart = (Case_t)d;
        c._arrivee = (Case_t)a;
        if (s == '/') c._dessus = 1;
        else c._dessus = 0;
        return in;
    }

private:
    Case_t _depart, _arrivee;
    bool _dessus;
};

#endif
