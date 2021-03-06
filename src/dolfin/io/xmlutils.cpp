// Copyright (C) 2011 Anders Logg
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
// First added:  2011-09-15
// Last changed: 2011-11-15

#include <dolfin/common/NoDeleter.h>
#include <dolfin/log/log.h>
#include "xmlutils.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
boost::shared_ptr<const pugi::xml_node>
dolfin::get_node(const pugi::xml_node& xml_node,
                 std::string node_name)
{
  // Check node itself
  if (xml_node.name() == node_name)
    return reference_to_no_delete_pointer(xml_node);

  // Check child
  boost::shared_ptr<const pugi::xml_node>
    child_node(new pugi::xml_node(xml_node.child(node_name.c_str())));
  if (!*child_node)
  {
    dolfin_error("xmlutils.cpp",
                 "read DOLFIN XML data",
                 "Unable to find tag <%s>", node_name.c_str());
  }
  return child_node;
}
//-----------------------------------------------------------------------------
