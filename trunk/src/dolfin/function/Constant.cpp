// Copyright (C) 2006-2011 Anders Logg
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
// Modified by Martin Sandve Alnes 2008
// Modified by Garth N. Wells 2009-2011
//
// First added:  2006-02-09
// Last changed: 2011-11-14

#include <dolfin/log/log.h>
#include "Constant.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
Constant::Constant(double value)
{
  _values.resize(1);
  _values[0] = value;
}
//-----------------------------------------------------------------------------
Constant::Constant(double value0, double value1)
  : Expression(2)
{
  _values.resize(2);
  _values[0] = value0;
  _values[1] = value1;
}
//-----------------------------------------------------------------------------
Constant::Constant(double value0, double value1, double value2)
  : Expression(3)
{
  _values.resize(3);
  _values[0] = value0;
  _values[1] = value1;
  _values[2] = value2;
}
//-----------------------------------------------------------------------------
Constant::Constant(std::vector<double> values)
  : Expression(values.size()), _values(values)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
Constant::Constant(std::vector<uint> value_shape,
                   std::vector<double> values)
  : Expression(value_shape), _values(values)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
Constant::Constant(const Constant& constant)
  : Expression(constant)
{
  *this = constant;
}
//-----------------------------------------------------------------------------
Constant::~Constant()
{
  // Do nothing
}
//-----------------------------------------------------------------------------
const Constant& Constant::operator= (const Constant& constant)
{
  // Check value shape
  if (constant.value_shape != value_shape)
  {
    dolfin_error("Constant.cpp",
                 "assign value to constant",
                 "Value shape mismatch");
  }

  // Assign values
  _values = constant._values;

  return *this;
}
//-----------------------------------------------------------------------------
const Constant& Constant::operator= (double constant)
{
  // Check value shape
  if (value_shape.size() != 0)
  {
    dolfin_error("Constant.cpp",
                 "assigne scalar value to constant",
                 "Constant is not a scalar");
  }

  // Assign value
  dolfin_assert(_values.size() == 1);
  _values[0] = constant;

  return *this;
}
//-----------------------------------------------------------------------------
Constant::operator double() const
{
  // Check value shape
  if (value_shape.size() != 0)
  {
    dolfin_error("Constant.cpp",
                 "convert constant to double",
                 "Constant is not a scalar");
  }

  // Return value
  dolfin_assert(_values.size() == 1);
  return _values[0];
}
//-----------------------------------------------------------------------------
void Constant::eval(Array<double>& values, const Array<double>& x) const
{
  // Copy values
  for (uint j = 0; j < _values.size(); j++)
    values[j] = _values[j];
}
//-----------------------------------------------------------------------------
