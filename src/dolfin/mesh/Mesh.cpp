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
//#include <ale/ALE.h>
//#include <common/Timer.h>
#include <common/utils.h>
#include <io/File.h>
//#include <log/log.h>
//#include <common/MPI.h>
//#include "BoundaryMesh.h"
#include "Cell.h"
//#include "LocalMeshData.h"
//#include "MeshColoring.h"
#include "MeshData.h"
#include "MeshFunction.h"
//#include "MeshOrdering.h"
//#include "MeshPartitioning.h"
//#include "MeshRenumbering.h"
//#include "MeshSmoothing.h"
//#include "ParallelData.h"
#include "TopologyComputation.h"
#include "Vertex.h"
#include "Mesh.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
Mesh::Mesh() : //Variable("mesh", "DOLFIN mesh"),
//               Hierarchical<Mesh>(*this),
               _data(*this),
//               _parallel_data(new ParallelData(*this)),
               _cell_type(0),
//               _intersection_operator(*this),
               _ordered(false)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
Mesh::Mesh(const Mesh& mesh) : //Variable("mesh", "DOLFIN mesh"),
//                               Hierarchical<Mesh>(*this),
                               _data(*this),
//                               _parallel_data(new ParallelData(*this)),
                               _cell_type(0),
//                               _intersection_operator(*this),
                               _ordered(false)
{
  *this = mesh;
}
//-----------------------------------------------------------------------------
Mesh::Mesh(std::string filename) : //Variable("mesh", "DOLFIN mesh"),
//                                   Hierarchical<Mesh>(*this),
                                   _data(*this),
//                                   _parallel_data(new ParallelData(*this)),
                                   _cell_type(0),
//                                   _intersection_operator(*this),
                                   _ordered(false)
{
  File file(filename);
  file >> *this;
}
//-----------------------------------------------------------------------------
//Mesh::Mesh(LocalMeshData& local_mesh_data)
//                                 : Variable("mesh", "DOLFIN mesh"),
//                                   Hierarchical<Mesh>(*this),
//                                   _data(*this),
//                                   _parallel_data(new ParallelData(*this)),
//                                   _cell_type(0),
//                                   _intersection_operator(*this),
//                                   _ordered(false)
//{
//  MeshPartitioning::build_distributed_mesh(*this, local_mesh_data);
//}
//-----------------------------------------------------------------------------
Mesh::~Mesh()
{
  clear();
}
//-----------------------------------------------------------------------------
const Mesh& Mesh::operator=(const Mesh& mesh)
{
  // Clear all data
  clear();

  // Assign data
  _topology = mesh._topology;
  _geometry = mesh._geometry;
//  _domains = mesh._domains;
  _data = mesh._data;
//  _parallel_data.reset(new ParallelData(*mesh._parallel_data));
  if (mesh._cell_type)
    _cell_type = CellType::create(mesh._cell_type->cell_type());

  // Rename
//  rename(mesh.name(), mesh.label());

  // Call assignment operator for base class
//  Hierarchical<Mesh>::operator=(mesh);

  return *this;
}
//-----------------------------------------------------------------------------
MeshData& Mesh::data()
{
  return _data;
}
//-----------------------------------------------------------------------------
const MeshData& Mesh::data() const
{
  return _data;
}
//-----------------------------------------------------------------------------
//ParallelData& Mesh::parallel_data()
//{
//  dolfin_assert(_parallel_data);
//  return *_parallel_data;
//}
//-----------------------------------------------------------------------------
//const ParallelData& Mesh::parallel_data() const
//{
//  dolfin_assert(_parallel_data);
//  return *_parallel_data;
//}
//-----------------------------------------------------------------------------
dolfin::uint Mesh::init(uint dim) const
{
  // This function is obviously not const since it may potentially compute
  // new connectivity. However, in a sense all connectivity of a mesh always
  // exists, it just hasn't been computed yet. The const_cast is also needed
  // to allow iterators over a const Mesh to create new connectivity.

  // Skip if mesh is empty
  if (num_cells() == 0)
  {
    printf("Mesh is empty, unable to create entities of dimension %d.", dim);
    return 0;
  }

  // Skip if already computed
  if (_topology.size(dim) > 0)
    return _topology.size(dim);

  // Skip vertices and cells (should always exist)
  if (dim == 0 || dim == _topology.dim())
    return _topology.size(dim);

  // Check that mesh is ordered
  if (!ordered())
  {
    printf("Mesh.cpp: initialize mesh entities. Mesh is not ordered according to the UFC numbering convention. Consider calling mesh.order()");
  }

  // Compute connectivity
  Mesh* mesh = const_cast<Mesh*>(this);
  TopologyComputation::compute_entities(*mesh, dim);

  // Order mesh if necessary
  if (!ordered())
    mesh->order();

  return _topology.size(dim);
}
//-----------------------------------------------------------------------------
void Mesh::init(uint d0, uint d1) const
{
  // This function is obviously not const since it may potentially compute
  // new connectivity. However, in a sense all connectivity of a mesh always
  // exists, it just hasn't been computed yet. The const_cast is also needed
  // to allow iterators over a const Mesh to create new connectivity.

  // Skip if mesh is empty
  if (num_cells() == 0)
  {
    printf("Mesh is empty, unable to create connectivity %d --> %d.", d0, d1);
    return;
  }

  // Skip if already computed
  if (_topology(d0, d1).size() > 0)
    return;

  // Check that mesh is ordered
  if (!ordered())
  {
    printf("Mesh.cpp: initialize mesh connectivity. Mesh is not ordered according to the UFC numbering convention. Consider calling mesh.order()");
  }

  // Compute connectivity
  Mesh* mesh = const_cast<Mesh*>(this);
  TopologyComputation::compute_connectivity(*mesh, d0, d1);

  // Order mesh if necessary
  if (!ordered())
    mesh->order();
}
//-----------------------------------------------------------------------------
void Mesh::init() const
{
  // Compute all entities
  for (uint d = 0; d <= topology().dim(); d++)
    init(d);

  // Compute all connectivity
  for (uint d0 = 0; d0 <= topology().dim(); d0++)
    for (uint d1 = 0; d1 <= topology().dim(); d1++)
      init(d0, d1);
}
//-----------------------------------------------------------------------------
void Mesh::clear()
{
  _topology.clear();
  _geometry.clear();
  _data.clear();
  delete _cell_type;
  _cell_type = 0;
//  _intersection_operator.clear();
  _ordered = false;
}
//-----------------------------------------------------------------------------
void Mesh::clean()
{
  const uint D = topology().dim();
  for (uint d0 = 0; d0 <= D; d0++)
  {
    for (uint d1 = 0; d1 <= D; d1++)
    {
      if (!(d0 == D && d1 == 0))
        _topology.clear(d0, d1);
    }
  }
}
//-----------------------------------------------------------------------------
void Mesh::order()
{
  // Order mesh
//  MeshOrdering::order(*this);

  // Remember that the mesh has been ordered
  _ordered = true;
}
//-----------------------------------------------------------------------------
bool Mesh::ordered() const
{
  // Don't check if we know (or think we know) that the mesh is ordered
  if (_ordered)
    return true;

//  _ordered = MeshOrdering::ordered(*this);
  return _ordered;
}
//-----------------------------------------------------------------------------
//dolfin::Mesh Mesh::renumber_by_color() const
//{
//  std::vector<uint> coloring_type;
//  const uint D = topology().dim();
//  coloring_type.push_back(D); coloring_type.push_back(0); coloring_type.push_back(D);
//  return MeshRenumbering::renumber_by_color(*this, coloring_type);
//}
//-----------------------------------------------------------------------------
void Mesh::move(BoundaryMesh& boundary)
{
//  ALE::move(*this, boundary);
}
//-----------------------------------------------------------------------------
void Mesh::move(Mesh& mesh)
{
//  ALE::move(*this, mesh);
}
//-----------------------------------------------------------------------------
//void Mesh::move(const Function& displacement)
//{
//  ALE::move(*this, displacement);
//}
//-----------------------------------------------------------------------------
void Mesh::smooth(uint num_iterations)
{
//  MeshSmoothing::smooth(*this, num_iterations);
}
//-----------------------------------------------------------------------------
void Mesh::smooth_boundary(uint num_iterations, bool harmonic_smoothing)
{
//  MeshSmoothing::smooth_boundary(*this, num_iterations, harmonic_smoothing);
}
//-----------------------------------------------------------------------------
//void Mesh::snap_boundary(const SubDomain& sub_domain, bool harmonic_smoothing)
//{
//  MeshSmoothing::snap_boundary(*this, sub_domain, harmonic_smoothing);
//}
//-----------------------------------------------------------------------------
//const dolfin::MeshFunction<dolfin::uint>&
//Mesh::color(std::string coloring_type) const
//{
//  // Define graph type
//  const uint dim = MeshColoring::type_to_dim(coloring_type, *this);
//  std::vector<uint> _coloring_type;
//  _coloring_type.push_back(topology().dim());
//  _coloring_type.push_back(dim);
//  _coloring_type.push_back(topology().dim());
//
//  return color(_coloring_type);
//}
//-----------------------------------------------------------------------------
//const dolfin::MeshFunction<dolfin::uint>&
//Mesh::color(std::vector<uint> coloring_type) const
//{
//  // Find color data
//  std::map<const std::vector<uint>, std::pair<MeshFunction<uint>,
//           std::vector<std::vector<uint> > > >::const_iterator coloring_data;
//  coloring_data = this->parallel_data().coloring.find(coloring_type);
//
//  if (coloring_data != this->parallel_data().coloring.end())
//  {
//    dolfin_debug("Mesh has already been colored, not coloring again.");
//    return coloring_data->second.first;
//  }
//
//  // We do the same const-cast trick here as in the init() functions
//  // since we are not really changing the mesh, just attaching some
//  // auxiliary data to it.
//  Mesh* _mesh = const_cast<Mesh*>(this);
//  return MeshColoring::color(*_mesh, coloring_type);
//}
//-----------------------------------------------------------------------------
void Mesh::intersected_cells(const Point& point,
                             std::set<uint>& cells) const
{
//  _intersection_operator.all_intersected_entities(point, cells);
}
//-----------------------------------------------------------------------------
void Mesh::intersected_cells(const std::vector<Point>& points,
                             std::set<uint>& cells) const
{
//  _intersection_operator.all_intersected_entities(points, cells);
}
//-----------------------------------------------------------------------------
void Mesh::intersected_cells(const MeshEntity & entity,
                             std::vector<uint>& cells) const
{
//  _intersection_operator.all_intersected_entities(entity, cells);
}
//-----------------------------------------------------------------------------
void Mesh::intersected_cells(const std::vector<MeshEntity>& entities,
                             std::set<uint>& cells) const
{
//  _intersection_operator.all_intersected_entities(entities, cells);
}
//-----------------------------------------------------------------------------
void Mesh::intersected_cells(const Mesh& another_mesh,
                             std::set<uint>& cells) const
{
//  _intersection_operator.all_intersected_entities(another_mesh, cells);
}
//-----------------------------------------------------------------------------
int Mesh::intersected_cell(const Point& point) const
{
//  return _intersection_operator.any_intersected_entity(point);
	return 0;
}
//-----------------------------------------------------------------------------
//Point Mesh::closest_point(const Point& point) const
//{
//  return _intersection_operator.closest_point(point);
//}
//-----------------------------------------------------------------------------
//dolfin::uint Mesh::closest_cell(const Point & point) const
//{
//  return _intersection_operator.closest_cell(point);
//}
//-----------------------------------------------------------------------------
//std::pair<Point,dolfin::uint>
//Mesh::closest_point_and_cell(const Point & point) const
//{
//  return _intersection_operator.closest_point_and_cell(point);
//}
//-----------------------------------------------------------------------------
double Mesh::distance(const Point& point) const
{
	return 0;
//  return _intersection_operator.distance(point);
}
//-----------------------------------------------------------------------------
//IntersectionOperator& Mesh::intersection_operator()
//{
//  return _intersection_operator;
//}
//-----------------------------------------------------------------------------
//const IntersectionOperator& Mesh::intersection_operator() const
//{
//  return _intersection_operator;
//}
//-----------------------------------------------------------------------------
double Mesh::hmin() const
{
  CellIterator cell(*this);
  double h = cell->diameter();
  for (; !cell.end(); ++cell)
    h = std::min(h, cell->diameter());

  return h;
}
//-----------------------------------------------------------------------------
double Mesh::hmax() const
{
  CellIterator cell(*this);
  double h = cell->diameter();
  for (; !cell.end(); ++cell)
    h = std::max(h, cell->diameter());

  return h;
}
//-----------------------------------------------------------------------------
std::string Mesh::str(bool verbose) const
{
  std::stringstream s;

  if (verbose)
  {
    s << str(false) << std::endl << std::endl;

    s << indent(_geometry.str(true));
    s << indent(_topology.str(true));
    s << indent(_data.str(true));
  }
  else
  {
    std::string cell_type("undefined cell type");
    if (_cell_type)
      cell_type = _cell_type->description(true);
    s << "<Mesh of topological dimension "
      << topology().dim() << " ("
      << cell_type << ") with "
      << num_vertices() << " vertices and "
      << num_cells() << " cells, "
      << (_ordered ? "ordered" : "unordered") << ">";
  }

  return s.str();
}
//-----------------------------------------------------------------------------
