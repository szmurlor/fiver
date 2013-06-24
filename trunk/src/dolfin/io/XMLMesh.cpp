// Copyright (C) 2011 Garth N. Wells
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
// Modified by Anders Logg 2011
//
// First added:  2002-12-06
// Last changed: 2011-11-14

#include <map>
#include <iomanip>
#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "pugixml.hpp"

#include "dolfin/common/Array.h"
#include "dolfin/common/MPI.h"
#include "dolfin/la/GenericVector.h"
#include "dolfin/mesh/Cell.h"
#include "dolfin/mesh/CellType.h"
#include "dolfin/mesh/Mesh.h"
#include "dolfin/mesh/MeshData.h"
#include "dolfin/mesh/MeshEditor.h"
#include "dolfin/mesh/Point.h"
#include "dolfin/mesh/Vertex.h"
#include "dolfin/mesh/MeshFunction.h"
#include "XMLMeshFunction.h"
#include "XMLMeshValueCollection.h"
#include "XMLMesh.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
void XMLMesh::read(Mesh& mesh, const pugi::xml_node xml_dolfin)
{
  // Get mesh node
  const pugi::xml_node mesh_node = xml_dolfin.child("mesh");
  if (!mesh_node)
  {
    dolfin_error("XMLMesh.cpp",
                 "read mesh from XML file",
                 "Not a DOLFIN XML Mesh file");
  }

  // Read mesh
  read_mesh(mesh, mesh_node);

  // Read mesh data (if any)
  read_data(mesh.data(), mesh_node);

  // Read mesh domains (if any)
  read_domains(mesh.domains(), mesh_node);
}
//-----------------------------------------------------------------------------
void XMLMesh::write(const Mesh& mesh, pugi::xml_node xml_node)
{
  // Add mesh node
  pugi::xml_node mesh_node = xml_node.append_child("mesh");

  // Write mesh
  write_mesh(mesh, mesh_node);

  // Write mesh data (if any)
  write_data(mesh.data(), mesh_node);

  // Write mesh markers (if any)
  write_domains(mesh.domains(), mesh_node);
}
//-----------------------------------------------------------------------------
void XMLMesh::read_mesh(Mesh& mesh, const pugi::xml_node mesh_node)
{
  // Get cell type and geometric dimension
  const std::string cell_type_str = mesh_node.attribute("celltype").value();
  const unsigned int gdim = mesh_node.attribute("dim").as_uint();

  // Get topological dimension
  boost::scoped_ptr<CellType> cell_type(CellType::create(cell_type_str));
  const unsigned int tdim = cell_type->dim();

  // Create mesh for editing
  MeshEditor editor;
  editor.open(mesh, cell_type_str, tdim, gdim);

  // Get vertices xml node
  pugi::xml_node xml_vertices = mesh_node.child("vertices");
  dolfin_assert(xml_vertices);

  // Get number of vertices and init editor
  const unsigned int num_vertices = xml_vertices.attribute("size").as_uint();
  editor.init_vertices(num_vertices);

  // Iterate over vertices and add to mesh
  Point p;
  for (pugi::xml_node_iterator it = xml_vertices.begin();
       it != xml_vertices.end(); ++it)
  {
    const unsigned int index = it->attribute("index").as_uint();
    p[0] = it->attribute("x").as_double();
    p[1] = it->attribute("y").as_double();
    p[2] = it->attribute("z").as_double();
    editor.add_vertex(index, p);
  }

  // Get cells node
  pugi::xml_node xml_cells = mesh_node.child("cells");
  dolfin_assert(xml_cells);

  // Get number of cells and init editor
  const unsigned int num_cells = xml_cells.attribute("size").as_uint();
  editor.init_cells(num_cells);

  // Create list of vertex index attribute names
  const unsigned int num_vertices_per_cell = cell_type->num_vertices(tdim);
  std::vector<std::string> v_str(num_vertices_per_cell);
  for (uint i = 0; i < num_vertices_per_cell; ++i)
    v_str[i] = "v" + boost::lexical_cast<std::string, unsigned int>(i);

  // Iterate over cells and add to mesh
  std::vector<unsigned int> v(num_vertices_per_cell);
  for (pugi::xml_node_iterator it = xml_cells.begin(); it != xml_cells.end(); ++it)
  {
    const unsigned int index = it->attribute("index").as_uint();
    for (unsigned int i = 0; i < num_vertices_per_cell; ++i)
      v[i] = it->attribute(v_str[i].c_str()).as_uint();
    editor.add_cell(index, v);
  }

  // Close mesh editor
  editor.close();
}
//-----------------------------------------------------------------------------
void XMLMesh::read_data(MeshData& data, const pugi::xml_node mesh_node)
{
  // Check if we have any mesh data
  const pugi::xml_node xml_data = mesh_node.child("data");
  if (!xml_data)
    return;

  // Iterate over data
  for (pugi::xml_node_iterator it = xml_data.begin(); it != xml_data.end(); ++it)
  {
    // Check that node is <data_entry>
    const std::string node_name = it->name();
    if (node_name != "data_entry")
    {
      dolfin_error("XMLMesh.cpp",
                   "read mesh data from XML file",
                   "Expecting XML node <data_entry> but got <%s>",
                   node_name.c_str());
    }

    // Get name of data set
    const std::string data_set_name = it->attribute("name").value();

    // Check that there is only one data set
    if (it->first_child().next_sibling())
    {
      dolfin_error("XMLMesh.cpp",
                   "read mesh data from XML file",
                   "XML file contains too many data sets");
    }

    // Get type of data set
    pugi::xml_node data_set = it->first_child();
    const std::string data_set_type = data_set.name();
    if (data_set_type == "array")
    {
      // Get type
      const std::string data_type = data_set.attribute("type").value();
      if (data_type == "uint")
      {
        // Get vector from MeshData
        boost::shared_ptr<std::vector<unsigned int> >
          array = data.array(data_set_name);
        if (!array)
          array = data.create_array(data_set_name);
        dolfin_assert(array);

        // Read vector
        read_array_uint(*array, data_set);
      }
      else
      {
        dolfin_error("XMLMesh.cpp",
                     "read mesh data from XML file",
                     "Only reading of MeshData uint arrays are supported at present");
      }
    }
    else if (data_set_type == "mesh_function")
    {
      // Get MeshFunction from MeshData
      const std::string data_type = data_set.attribute("type").value();
      boost::shared_ptr<MeshFunction<unsigned int> >
        mf = data.mesh_function(data_set_name);
      if (!mf)
        mf = data.create_mesh_function(data_set_name);
      dolfin_assert(mf);

      // Read  MeshFunction
      XMLMeshFunction::read(*mf, data_type, *it);
    }
    else if (data_set_type == "meshfunction")
    {
      dolfin_error("XMLMesh.cpp",
                   "read mesh data from XML file",
                   "The XML tag <meshfunction> has been changed to <mesh_function>");
    }
    else
    {
      dolfin_error("XMLMesh.cpp",
                   "read mesh data from XML file",
                   "Reading of MeshData \"%s\" is not yet supported",
                   data_set_type.c_str());
    }
  }
}
//-----------------------------------------------------------------------------
void XMLMesh::read_domains(MeshDomains& domains,
                           const pugi::xml_node mesh_node)
{
  // Check if we have any domains
  const pugi::xml_node xml_domains = mesh_node.child("domains");
  if (!xml_domains)
    return;

  // Iterate over data
  for (pugi::xml_node_iterator it = xml_domains.begin();
       it != xml_domains.end(); ++it)
  {
    // Check that node is <mesh_value_collection>
    const std::string node_name = it->name();
    if (node_name != "mesh_value_collection")
    {
      dolfin_error("XMLMesh.cpp",
                   "read mesh domains from XML file",
                   "Expecting XML node <mesh_value_collection> but got <%s>",
                   node_name.c_str());
    }

    // Get attributes
    const std::string type = it->attribute("type").value();
    const uint dim = it->attribute("dim").as_uint();

    // Check that the type is uint
    if (type != "uint")
    {
      dolfin_error("XMLMesh.cpp",
                   "read mesh domains from XML file",
                   "Mesh domains must be marked as uint, not %s",
                   type.c_str());
    }

    // Read MeshValueCollection
    XMLMeshValueCollection::read(domains.markers(dim), type, *it);
  }
}
//-----------------------------------------------------------------------------
void XMLMesh::read_array_uint(std::vector<unsigned int>& array,
                              const pugi::xml_node xml_array)
{
  // Check that we have an array
  const std::string name = xml_array.name();
  if (name != "array")
  {
    dolfin_error("XMLMesh.cpp",
                 "read mesh array data from XML file",
                 "Expecting an XML array node");
  }

  // Check type is unit
  const std::string type = xml_array.attribute("type").value();
  if (type != "uint")
  {
    dolfin_error("XMLMesh.cpp",
                 "read mesh array data from XML file",
                 "Expecting an XML array node");
  }

  // Get size and resize vector
  const unsigned int size = xml_array.attribute("size").as_uint();
  array.resize(size);

  // Iterate over array entries
  for (pugi::xml_node_iterator it = xml_array.begin(); it !=xml_array.end(); ++it)
  {
    const unsigned int index = it->attribute("index").as_uint();
    const double value = it->attribute("value").as_uint();
    dolfin_assert(index < size);
    array[index] = value;
  }
}
//-----------------------------------------------------------------------------
void XMLMesh::write_mesh(const Mesh& mesh, pugi::xml_node mesh_node)
{
  // Add mesh attributes
  const CellType::Type _cell_type = mesh.type().cell_type();
  const std::string cell_type = CellType::type2string(_cell_type);
  mesh_node.append_attribute("celltype") = cell_type.c_str();
  mesh_node.append_attribute("dim") = mesh.geometry().dim();

  // Add vertices node
  pugi::xml_node vertices_node = mesh_node.append_child("vertices");
  vertices_node.append_attribute("size") = mesh.num_vertices();

  // Write each vertex
  for (VertexIterator v(mesh); !v.end(); ++v)
  {
    pugi::xml_node vertex_node = vertices_node.append_child("vertex");
    vertex_node.append_attribute("index") = v->index();

    const Point p = v->point();
    switch (mesh.geometry().dim())
    {
      case 1:
        vertex_node.append_attribute("x") = p.x();
        break;
      case 2:
        vertex_node.append_attribute("x") = p.x();
        vertex_node.append_attribute("y") = p.y();
        break;
      case 3:
        vertex_node.append_attribute("x") = p.x();
        vertex_node.append_attribute("y") = p.y();
        vertex_node.append_attribute("z") = p.z();
        break;
      default:
        dolfin_error("XMLMesh.cpp",
                     "write mesh to XML file",
                     "The XML mesh file format only supports 1D, 2D and 3D meshes");
    }
  }

  // Add cells node
  pugi::xml_node cells_node = mesh_node.append_child("cells");
  cells_node.append_attribute("size") = mesh.num_cells();

  // Add each cell
  for (CellIterator c(mesh); !c.end(); ++c)
  {
    pugi::xml_node cell_node = cells_node.append_child(cell_type.c_str());
    cell_node.append_attribute("index") = c->index();

    const uint* vertices = c->entities(0);
    dolfin_assert(vertices);

    switch (_cell_type)
    {
    case CellType::interval:
      cell_node.append_attribute("v0") = vertices[0];
      cell_node.append_attribute("v1") = vertices[1];
      break;
    case CellType::triangle:
      cell_node.append_attribute("v0") = vertices[0];
      cell_node.append_attribute("v1") = vertices[1];
      cell_node.append_attribute("v2") = vertices[2];
      break;
    case CellType::tetrahedron:
      cell_node.append_attribute("v0") = vertices[0];
      cell_node.append_attribute("v1") = vertices[1];
      cell_node.append_attribute("v2") = vertices[2];
      cell_node.append_attribute("v3") = vertices[3];
      break;
    default:
      dolfin_error("XMLMesh.cpp",
                   "write mesh to XML file",
                   "Unknown cell type (%u)", _cell_type);
    }
  }
}
//-----------------------------------------------------------------------------
void XMLMesh::write_data(const MeshData& data, pugi::xml_node mesh_node)
{
  // Check if there is any data to write
  if (data.mesh_functions.size() + data.arrays.size() == 0)
    return;

  // Add mesh data node
  pugi::xml_node mesh_data_node = mesh_node.append_child("data");

  // Write mesh functions
  typedef std::map<std::string,
                   boost::shared_ptr<MeshFunction<uint> > >
    ::const_iterator mf_iterator;
  for (mf_iterator it = data.mesh_functions.begin();
       it != data.mesh_functions.end(); ++it)
  {
    std::string name = it->first;
    boost::shared_ptr<MeshFunction<unsigned int> > mf = it->second;
    dolfin_assert(mf);

    pugi::xml_node data_entry_node = mesh_data_node.append_child("data_entry");
    data_entry_node.append_attribute("name") = name.c_str();

    pugi::xml_node mf_node = data_entry_node.append_child("mesh_function");
    mf_node.append_attribute("type") = "uint";
    mf_node.append_attribute("dim") = mf->dim();
    mf_node.append_attribute("size") = mf->size();

    for (uint i = 0; i < mf->size(); i++)
    {
      pugi::xml_node entity_node = mf_node.append_child("entity");
      entity_node.append_attribute("index") = i;
      entity_node.append_attribute("value") = (*mf)[i];
    }
  }

  // Write arrays
  typedef std::map<std::string,
                   boost::shared_ptr<std::vector<uint> > >
    ::const_iterator array_iterator;
  for (array_iterator it = data.arrays.begin();
       it != data.arrays.end(); ++it)
  {
    std::string name = it->first;
    boost::shared_ptr<std::vector<unsigned int> > array = it->second;
    dolfin_assert(array);

    pugi::xml_node data_entry_node = mesh_data_node.append_child("data_entry");
    data_entry_node.append_attribute("name") = name.c_str();

    pugi::xml_node array_node = data_entry_node.append_child("array");
    array_node.append_attribute("type") = "uint";
    array_node.append_attribute("size") = static_cast<uint>(array->size());

    for (uint i = 0; i < array->size(); i++)
    {
      pugi::xml_node element_node = array_node.append_child("element");
      element_node.append_attribute("index") = i;
      element_node.append_attribute("value") = (*array)[i];
    }
  }
}
//-----------------------------------------------------------------------------
void XMLMesh::write_domains(const MeshDomains& domains,
                            pugi::xml_node mesh_node)
{
  // Check if there is any data to write
  if (domains.is_empty())
    return;

  // Add mesh domains node
  pugi::xml_node domains_node = mesh_node.append_child("domains");

  // Write mesh markers
  for (uint d = 0; d <= domains.dim(); d++)
  {
    if (domains.markers(d).size() > 0)
      XMLMeshValueCollection::write(domains.markers(d), "uint", domains_node);
  }
}
//-----------------------------------------------------------------------------
