// Copyright (C) 2006-2008 Anders Logg
//
// This file is part of DOLFIN.
//
// DOLFIN is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// DOLFIN is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with DOLFIN. If not, see <http://www.gnu.org/licenses/>.
//
#include <cmath>
#include "Point.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
double Point::distance(const Point& p) const
{
  const double dx = p._x[0] - _x[0];
  const double dy = p._x[1] - _x[1];
  const double dz = p._x[2] - _x[2];

  return std::sqrt(dx*dx + dy*dy + dz*dz);
}
//-----------------------------------------------------------------------------
double Point::norm() const
{
  return std::sqrt(_x[0]*_x[0] + _x[1]*_x[1] + _x[2]*_x[2]);
}
//-----------------------------------------------------------------------------
const Point Point::cross(const Point& p) const
{
  Point q;

  q._x[0] = _x[1]*p._x[2] - _x[2]*p._x[1];
  q._x[1] = _x[2]*p._x[0] - _x[0]*p._x[2];
  q._x[2] = _x[0]*p._x[1] - _x[1]*p._x[0];

  return q;
}
//-----------------------------------------------------------------------------
double Point::dot(const Point& p) const
{
  return _x[0]*p._x[0] + _x[1]*p._x[1] + _x[2]*p._x[2];
}
//-----------------------------------------------------------------------------
std::string Point::str(bool verbose) const
{
  std::stringstream s;
  s << "<Point x = " << x() << " y = " << y() << " z = " << z() << ">";
  return s.str();
}
//-----------------------------------------------------------------------------

