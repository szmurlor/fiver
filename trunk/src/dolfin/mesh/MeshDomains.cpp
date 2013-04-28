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
// First added:  2011-08-29
// Last changed: 2011-10-05

#include <limits>

#include <dolfin/common/MPI.h>
#include <dolfin/log/log.h>
#include "MeshFunction.h"
#include "MeshValueCollection.h"
#include "MeshDomains.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
MeshDomains::MeshDomains()
{
  // Do nothing
}
//-----------------------------------------------------------------------------
MeshDomains::~MeshDomains()
{
  // Do nothing
}
//-----------------------------------------------------------------------------
dolfin::uint MeshDomains::dim() const
{
  if (_markers.size() > 0)
    return _markers.size() - 1;
  else
    return 0;
}
//-----------------------------------------------------------------------------
dolfin::uint MeshDomains::num_marked(uint dim) const
{
  dolfin_assert(dim < _markers.size());
  dolfin_assert(_markers[dim]);
  return _markers[dim]->size();
}
//-----------------------------------------------------------------------------
bool MeshDomains::is_empty() const
{
  uint size = 0;
  for (uint i = 0; i < _markers.size(); i++)
  {
    dolfin_assert(_markers[i]);
    size += _markers[i]->size();
  }
  return size == 0;
}
//-----------------------------------------------------------------------------
MeshValueCollection<unsigned int>& MeshDomains::markers(uint dim)
{
  dolfin_assert(dim < _markers.size());
  dolfin_assert(_markers[dim]);
  return *_markers[dim];
}
//-----------------------------------------------------------------------------
const MeshValueCollection<unsigned int>& MeshDomains::markers(uint dim) const
{
  dolfin_assert(dim < _markers.size());
  dolfin_assert(_markers[dim]);
  return *_markers[dim];
}
//-----------------------------------------------------------------------------
boost::shared_ptr<MeshValueCollection<unsigned int> >
MeshDomains::markers_shared_ptr(uint dim)
{
  dolfin_assert(dim < _markers.size());
  dolfin_assert(_markers[dim]);
  return _markers[dim];
}
//-----------------------------------------------------------------------------
boost::shared_ptr<const MeshValueCollection<unsigned int> >
MeshDomains::markers_shared_ptr(uint dim) const
{
  dolfin_assert(dim < _markers.size());
  dolfin_assert(_markers[dim]);
  return _markers[dim];
}
//-----------------------------------------------------------------------------
boost::shared_ptr<const MeshFunction<dolfin::uint> >
MeshDomains::cell_domains(const Mesh& mesh) const
{
  // Check if data already exists
  if (_cell_domains)
    return _cell_domains;

  // Check if any markers have been set
  const uint D = mesh.topology().dim();
  dolfin_assert(_markers.size() == D + 1);
  if (_markers[D]->size() == 0)
    return _cell_domains;

  // Compute cell domains
  _cell_domains = boost::shared_ptr<MeshFunction<uint> >(new MeshFunction<uint>());
  _cell_domains->init(mesh, D);
  init_domains(*_cell_domains);

  return _cell_domains;
}
//-----------------------------------------------------------------------------
boost::shared_ptr<const MeshFunction<dolfin::uint> >
MeshDomains::facet_domains(const Mesh& mesh) const
{
  // Check if data already exists
  if (_facet_domains)
    return _facet_domains;

  // Check if any markers have been set
  const uint D = mesh.topology().dim();
  dolfin_assert(_markers.size() == D + 1);
  if (_markers[D - 1]->size() == 0)
    return _facet_domains;

  // Compute facet domains
  _facet_domains = boost::shared_ptr<MeshFunction<uint> >(new MeshFunction<uint>());
  _facet_domains->init(mesh, D - 1);
  init_domains(*_facet_domains);

  return _facet_domains;
}
//-----------------------------------------------------------------------------
void MeshDomains::init(uint dim)
{
  // Clear old data
  clear();

  // Add markers for each topological dimension
  for (uint d = 0; d <= dim; d++)
  {
    boost::shared_ptr<MeshValueCollection<uint> >
      m(new MeshValueCollection<uint>(d));
    _markers.push_back(m);
  }
}
//-----------------------------------------------------------------------------
void MeshDomains::clear()
{
  _markers.clear();
}
//-----------------------------------------------------------------------------
void MeshDomains::init_domains(MeshFunction<uint>& mesh_function) const
{
  // Get mesh
  const Mesh& mesh = mesh_function.mesh();
  const uint d = mesh_function.dim();
  const uint D = mesh.topology().dim();

  // Get mesh connectivity D --> d
  dolfin_assert(d <= D);
  const MeshConnectivity& connectivity = mesh.topology()(D, d);
  dolfin_assert(D == d || connectivity.size() > 0);

  // Set all values of mesh function to maximum uint value
  mesh_function.set_all(std::numeric_limits<unsigned int>::max());

  // Iterate over all values
  const std::map<std::pair<uint, uint>, uint> values = _markers[d]->values();
  std::map<std::pair<uint, uint>, uint>::const_iterator it;
  for (it = values.begin(); it != values.end(); ++it)
  {
    // Get marker data
    const uint cell_index = it->first.first;
    const uint local_entity = it->first.second;
    const uint value = it->second;

    // Get global entity index. Note that we ignore the local entity
    // index when the function is defined over cells.
    uint entity_index(0);
    if (d == D)
      entity_index = cell_index;
    else
      entity_index = connectivity(cell_index)[local_entity];

    // Set value for entity
    mesh_function[entity_index] = value;
  }
}
//---------------------------------------------------------------------------
