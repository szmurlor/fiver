// Copyright (C) 2009-2011 Ola Skavhaug and Garth N. Wells
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
// First added:  2009-03-03
// Last changed: 2011-09-27

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "pugixml.hpp"

#include <dolfin/common/Array.h>
#include <dolfin/common/types.h>
#include <dolfin/common/constants.h>
#include <dolfin/common/MPI.h>
#include <dolfin/common/NoDeleter.h>
#include <dolfin/function/Function.h>
#include <dolfin/la/GenericVector.h>
#include <dolfin/log/log.h>
#include <dolfin/mesh/LocalMeshData.h>
#include <dolfin/mesh/Mesh.h>
#include <dolfin/mesh/MeshPartitioning.h>
#include "XMLFunctionData.h"
//#include "XMLFunctionPlotData.h"
//#include "XMLLocalMeshSAX.h"
#include "XMLMesh.h"
#include "XMLMeshFunction.h"
//#include "XMLMeshValueCollection.h"
//#include "XMLParameters.h"
//#include "XMLVector.h"
#include "XMLFile.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
XMLFile::XMLFile(const std::string filename)
  : GenericFile(filename, "XML")
{
  // Do nothing
}
//-----------------------------------------------------------------------------
XMLFile::XMLFile(std::ostream& s)
  : GenericFile("", "XML"), outstream(&s, NoDeleter())
{
  // Do nothing
}
//-----------------------------------------------------------------------------
XMLFile::~XMLFile()
{
  // Do nothing
}
//-----------------------------------------------------------------------------
void XMLFile::operator>> (Mesh& input_mesh)
{
//  if (MPI::num_processes() == 1)
//  {
    // Create XML doc and get DOLFIN node
    pugi::xml_document xml_doc;
    load_xml_doc(xml_doc);
    const pugi::xml_node dolfin_node = get_dolfin_xml_node(xml_doc);

    // Read mesh
    XMLMesh::read(input_mesh, dolfin_node);
//  }
//  else
//  {
//    // Read local mesh data
//    LocalMeshData local_mesh_data;
//    XMLLocalMeshSAX xml_object(local_mesh_data, filename);
//    xml_object.read();

//    // Partition and build mesh
//    MeshPartitioning::build_distributed_mesh(input_mesh, local_mesh_data);
//  }
}
//-----------------------------------------------------------------------------
void XMLFile::operator<< (const Mesh& output_mesh)
{
  not_working_in_parallel("Mesh XML output in parallel not yet supported.");

  pugi::xml_document doc;
  pugi::xml_node node = write_dolfin(doc);
  XMLMesh::write(output_mesh, node);
  save_xml_doc(doc);
}
//-----------------------------------------------------------------------------
//void XMLFile::operator>> (LocalMeshData& input_data)
//{
//  XMLLocalMeshSAX xml_object(input_data, filename);
//  xml_object.read();
//}
//-----------------------------------------------------------------------------
//void XMLFile::operator<< (const LocalMeshData& output_data)
//{
//  dolfin_error("XMLFile.cpp",
//               "write local mesh data to XML file",
//               "Not implemented");
//}
//-----------------------------------------------------------------------------
//void XMLFile::operator>> (GenericVector& input)
//{
//  pugi::xml_document xml_doc;
//  pugi::xml_node dolfin_node(0);

//  // Read vector size
//  uint size = 0;
//  if (MPI::process_number() == 0)
//  {
//    load_xml_doc(xml_doc);
//    dolfin_node = get_dolfin_xml_node(xml_doc);
//    size = XMLVector::read_size(dolfin_node);
//  }
//  MPI::broadcast(size);

//  // Resize if necessary
//  const uint input_vector_size = input.size();
//  const uint num_proc = MPI::num_processes();
//  if (num_proc > 1 && input_vector_size != size)
//    warning("Resizing parallel vector. Default partitioning will be used. To control distribution, initialize vector size before reading from file.");
//  if (input.size() != size)
//    input.resize(size);

//  // Read vector on root process
//  if (MPI::process_number() == 0)
//  {
//    dolfin_assert(dolfin_node);
//    XMLVector::read(input, dolfin_node);
//  }

//  // Finalise
//  input.apply("insert");
//}
//-----------------------------------------------------------------------------
//void XMLFile::read_vector(Array<double>& input, Array<uint>& indices)
//{
//  // Create XML doc and get DOLFIN node
//  pugi::xml_document xml_doc;
//  load_xml_doc(xml_doc);
//  const pugi::xml_node dolfin_node = get_dolfin_xml_node(xml_doc);

//  // Read parameters
//  XMLVector::read(input, indices, dolfin_node);
//}
//-----------------------------------------------------------------------------
//void XMLFile::operator<< (const GenericVector& output)
//{
//  // Open file on process 0 for distributed objects and on all processes
//  // for local objects
//  if (MPI::process_number() == 0)
//  {
//    pugi::xml_document doc;
//    pugi::xml_node node = write_dolfin(doc);
//    XMLVector::write(output, node, true);
//    save_xml_doc(doc);
//  }
//  else
//  {
//    pugi::xml_node node(0);
//    XMLVector::write(output, node, false);
//  }
//}
//-----------------------------------------------------------------------------
//void XMLFile::operator>> (Parameters& input)
//{
//  // Create XML doc and get DOLFIN node
//  pugi::xml_document xml_doc;
//  load_xml_doc(xml_doc);
//  const pugi::xml_node dolfin_node = get_dolfin_xml_node(xml_doc);

//  // Read parameters
//  XMLParameters::read(input, dolfin_node);
//}
////-----------------------------------------------------------------------------
//void XMLFile::operator<< (const Parameters& output)
//{
//  if (MPI::process_number() == 0)
//  {
//    pugi::xml_document doc;
//    pugi::xml_node node = write_dolfin(doc);
//    XMLParameters::write(output, node);
//    save_xml_doc(doc);
//  }
//}
//-----------------------------------------------------------------------------
void XMLFile::operator>>(Function& input)
{
  // Create XML doc and get DOLFIN node
  pugi::xml_document xml_doc;
  pugi::xml_node dolfin_node(0);
  if (MPI::process_number() == 0)
  {
    load_xml_doc(xml_doc);
    dolfin_node = get_dolfin_xml_node(xml_doc);
  }

  // Read data
  XMLFunctionData::read(input, dolfin_node);
}
//-----------------------------------------------------------------------------
void XMLFile::operator<< (const Function& output)
{
  if (MPI::process_number() == 0)
  {
    pugi::xml_document doc;
    pugi::xml_node node = write_dolfin(doc);
    XMLFunctionData::write(output, node);
    save_xml_doc(doc);
  }
  else
  {
    pugi::xml_node node(0);
    XMLFunctionData::write(output, node);
  }
}
//-----------------------------------------------------------------------------
//void XMLFile::operator>> (FunctionPlotData& input)
//{
//  // Create XML doc and get DOLFIN node
//  pugi::xml_document xml_doc;
//  load_xml_doc(xml_doc);
//  const pugi::xml_node dolfin_node = get_dolfin_xml_node(xml_doc);

//  // Read plot data
//  XMLFunctionPlotData::read(input, dolfin_node);
//}
////-----------------------------------------------------------------------------
//void XMLFile::operator<< (const FunctionPlotData& output)
//{
//  not_working_in_parallel("FunctionPlotData XML output in parallel not yet supported.");

//  pugi::xml_document doc;
//  pugi::xml_node node = write_dolfin(doc);
//  XMLFunctionPlotData::write(output, node);
//  save_xml_doc(doc);
//}
//-----------------------------------------------------------------------------
template<typename T> void XMLFile::read_mesh_function(MeshFunction<T>& t,
                                                  const std::string type) const
{
  pugi::xml_document xml_doc;
  load_xml_doc(xml_doc);
  const pugi::xml_node dolfin_node = get_dolfin_xml_node(xml_doc);
  XMLMeshFunction::read(t, type, dolfin_node);
}
//-----------------------------------------------------------------------------
template<typename T> void XMLFile::write_mesh_function(const MeshFunction<T>& t,
                                                       const std::string type)
{
  not_working_in_parallel("MeshFunction XML output in parallel not yet supported.");

  pugi::xml_document xml_doc;
  pugi::xml_node node = write_dolfin(xml_doc);
  XMLMeshFunction::write(t, type, node, false);
  save_xml_doc(xml_doc);
}
//-----------------------------------------------------------------------------
//template<typename T>
//void XMLFile::read_mesh_value_collection(MeshValueCollection<T>& t,
//                                         const std::string type) const
//{
//  pugi::xml_document xml_doc;
//  load_xml_doc(xml_doc);
//  const pugi::xml_node dolfin_node = get_dolfin_xml_node(xml_doc);
//  XMLMeshValueCollection::read(t, type, dolfin_node);
//}
////-----------------------------------------------------------------------------
//template<typename T>
//void XMLFile::write_mesh_value_collection(const MeshValueCollection<T>& t,
//                                          const std::string type)
//{
//  not_working_in_parallel("MeshValueCollection XML output in parallel not yet supported.");

//  pugi::xml_document xml_doc;
//  pugi::xml_node node = write_dolfin(xml_doc);
//  XMLMeshValueCollection::write(t, type, node);
//  save_xml_doc(xml_doc);
//}
//-----------------------------------------------------------------------------
void XMLFile::load_xml_doc(pugi::xml_document& xml_doc) const
{
  // Create XML parser result
  pugi::xml_parse_result result;

  // Get file path and extension
  const boost::filesystem::path path(filename);
  const std::string extension = boost::filesystem::extension(path);

  // FIXME: Check that file exists
  if (!boost::filesystem::is_regular_file(filename))
    dolfin_error("XMLFile.cpp",
                 "read data from XML file",
                 "Unable to open file \"%s\"", filename.c_str());

  // Load xml file (unzip if necessary) into parser
  if (extension == ".gz")
  {
    // Decompress file
    std::ifstream file(filename.c_str(), std::ios_base::in|std::ios_base::binary);
    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::gzip_decompressor());
    in.push(file);

    // FIXME: Is this the best way to do it?
    std::stringstream dst;
    boost::iostreams::copy(in, dst);

    // Load data
    result = xml_doc.load(dst);
  }
  else
    result = xml_doc.load_file(filename.c_str());

  if (!result)
    dolfin_error("XMLFile.cpp",
                 "read data from XML file",
                 "Error while parsing XML");
}
//-----------------------------------------------------------------------------
// Pragma to avoid Boost.iostreams error with strict compiler flags
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Woverflow"
#endif
void XMLFile::save_xml_doc(const pugi::xml_document& xml_doc) const
{
  if (outstream)
    xml_doc.save(*outstream, "  ");
  else
  {
    // Compress if filename has extension '.gz'
    const boost::filesystem::path path(filename);
    const std::string extension = boost::filesystem::extension(path);
    if (extension == ".gz")
    {
      std::stringstream xml_stream;
      xml_doc.save(xml_stream, "  ");

      std::ofstream file(filename.c_str(), std::ios_base::out | std::ios_base::binary);
      boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
      out.push(boost::iostreams::gzip_compressor());
      out.push(file);
      boost::iostreams::copy(xml_stream, out);
    }
    else
      xml_doc.save_file(filename.c_str(), "  ");
  }
}
//-----------------------------------------------------------------------------
const pugi::xml_node XMLFile::get_dolfin_xml_node(pugi::xml_document& xml_doc) const
{
  // Check that we have a DOLFIN XML file
  const pugi::xml_node dolfin_node = xml_doc.child("dolfin");
  if (!dolfin_node)
    dolfin_error("XMLFile.cpp",
                 "read data from XML file",
                 "Not a DOLFIN XML file");
  return dolfin_node;
}
//-----------------------------------------------------------------------------
pugi::xml_node XMLFile::write_dolfin(pugi::xml_document& xml_doc)
{
  pugi::xml_node node = xml_doc.append_child("dolfin");
  node.append_attribute("xmlns:dolfin") = "http://fenicsproject.org";
  return node;
}
//-----------------------------------------------------------------------------
