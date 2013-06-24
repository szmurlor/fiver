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
// First added:  2011-06-30
// Last changed: 2011-11-15

#ifndef __XML_MESH_VALUE_COLLECTION_H
#define __XML_MESH_VALUE_COLLECTION_H

#include <dolfin/mesh/MeshValueCollection.h>
#include "pugixml.hpp"
#include "xmlutils.h"
#include "XMLMesh.h"

namespace dolfin
{

  class XMLMeshValueCollection
  {
  public:

    // Read mesh value collection from XML file
    template <typename T>
    static void read(MeshValueCollection<T>& mesh_value_collection,
                     const std::string type,
                     const pugi::xml_node xml_node);

    /// Write mesh value collection to XML file
    template<typename T>
    static void write(const MeshValueCollection<T>& mesh_value_collection,
                      const std::string type,
                      pugi::xml_node xml_node);

  };

  //---------------------------------------------------------------------------
  template <typename T>
  void XMLMeshValueCollection::read(MeshValueCollection<T>& mesh_value_collection,
                                    const std::string type,
                                    const pugi::xml_node xml_node)
  {
    // Get node
    boost::shared_ptr<const pugi::xml_node>
      mvc_node = get_node(xml_node, "mesh_value_collection");
    dolfin_assert(mvc_node);

    // Get attributes
    const std::string type_file = mvc_node->attribute("type").value();
    const uint dim = mvc_node->attribute("dim").as_uint();

    // Set dim
    mesh_value_collection.set_dim(dim);

    // Check that types match
    if (type != type_file)
    {
      dolfin_error("XMLMeshValueCollection.h",
                   "read mesh value collection from XML file",
                   "Type mismatch, found \"%s\" but expecting \"%s\"",
                   type_file.c_str(), type.c_str());
    }

    // Check that dimension matches
    if (mesh_value_collection.dim() != dim)
    {
      dolfin_error("XMLMeshValueCollection.h",
                   "read mesh value collection from XML file",
                   "Dimension mismatch, found %d but expecting %d",
                   dim, mesh_value_collection.dim());
    }

    // Clear old values
    mesh_value_collection.clear();

    // Choose data type
    if (type == "uint")
    {
      for (pugi::xml_node_iterator it = mvc_node->begin();
           it != mvc_node->end(); ++it)
      {
        const uint cell_index = it->attribute("cell_index").as_uint();
        const uint local_entity = it->attribute("local_entity").as_uint();
        const uint value = it->attribute("value").as_uint();
        mesh_value_collection.set_value(cell_index, local_entity, value);
      }
    }
    else if (type == "int")
    {
      for (pugi::xml_node_iterator it = mvc_node->begin();
           it != mvc_node->end(); ++it)
      {
        const uint cell_index = it->attribute("cell_index").as_uint();
        const uint local_entity = it->attribute("local_entity").as_uint();
        const int value = it->attribute("value").as_int();
        mesh_value_collection.set_value(cell_index, local_entity, value);
      }
    }
    else if (type == "double")
    {
      for (pugi::xml_node_iterator it = mvc_node->begin();
           it != mvc_node->end(); ++it)
      {
        const uint cell_index = it->attribute("cell_index").as_uint();
        const uint local_entity = it->attribute("local_entity").as_uint();
        const double value = it->attribute("value").as_double();
        mesh_value_collection.set_value(cell_index, local_entity, value);
      }
    }
    else if (type == "bool")
    {
      for (pugi::xml_node_iterator it = mvc_node->begin();
           it != mvc_node->end(); ++it)
      {
        const uint cell_index = it->attribute("cell_index").as_uint();
        const uint local_entity = it->attribute("local_entity").as_uint();
        const bool value = it->attribute("value").as_bool();
        mesh_value_collection.set_value(cell_index, local_entity, value);
      }
    }
    else
    {
      dolfin_error("XMLValueCollection.h",
                   "read mesh value collection from XML file",
                   "Unhandled value type \"%s\"", type.c_str());
    }
  }
  //---------------------------------------------------------------------------
  template<typename T>
  void XMLMeshValueCollection::write(const MeshValueCollection<T>& mesh_value_collection,
                                     const std::string type,
                                     pugi::xml_node xml_node)
  {
    not_working_in_parallel("Writing XML MeshValueCollection");

    // Add mesh function node and attributes
    pugi::xml_node mf_node = xml_node.append_child("mesh_value_collection");
    mf_node.append_attribute("type") = type.c_str();
    mf_node.append_attribute("dim") = mesh_value_collection.dim();
    mf_node.append_attribute("size") = mesh_value_collection.size();

    // Add data
    const std::map<std::pair<uint, uint>, T>&
      values = mesh_value_collection.values();
    typename std::map<std::pair<uint, uint>, T>::const_iterator it;
    for (it = values.begin(); it != values.end(); ++it)
    {
      pugi::xml_node entity_node = mf_node.append_child("value");
      entity_node.append_attribute("cell_index") = it->first.first;
      entity_node.append_attribute("local_entity") = it->first.second;
      entity_node.append_attribute("value") = it->second;
    }
  }
  //---------------------------------------------------------------------------

}
#endif
