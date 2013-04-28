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
// Last changed: 2011-09-27

#ifndef __MESH_DOMAINS_H
#define __MESH_DOMAINS_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <dolfin/common/types.h>

namespace dolfin
{

  // Forward declarations
  class Mesh;
  template <typename T> class MeshFunction;
  template <typename T> class MeshValueCollection;

  /// The class _MeshDomains_ stores the division of a _Mesh_ into
  /// subdomains. For each topological dimension 0 <= d <= D, where D
  /// is the topological dimension of the _Mesh_, a set of integer
  /// markers are stored for a subset of the entities of dimension d,
  /// indicating for each entity in the subset the number of the
  /// subdomain. It should be noted that the subset does not need to
  /// contain all entities of any given dimension; entities not
  /// contained in the subset are "unmarked".

  class MeshDomains
  {
  public:

    /// Create empty mesh domains
    MeshDomains();

    /// Destructor
    ~MeshDomains();

    /// Return maximal topological dimension of stored markers
    uint dim() const;

    /// Return number of marked entities of given dimension
    uint num_marked(uint dim) const;

    /// Check whether domain data is empty
    bool is_empty() const;

    /// Get subdomain markers for given dimension
    MeshValueCollection<unsigned int>& markers(uint dim);

    /// Get subdomain markers for given dimension (const version)
    const MeshValueCollection<unsigned int>& markers(uint dim) const;

    /// Get subdomain markers for given dimension (shared pointer version)
    boost::shared_ptr<MeshValueCollection<unsigned int> >
    markers_shared_ptr(uint dim);

    /// Get subdomain markers for given dimension (const shared pointer version)
    boost::shared_ptr<const MeshValueCollection<unsigned int> >
    markers_shared_ptr(uint dim) const;

    /// Get cell domains. This function computes the mesh function
    /// corresponding to markers of dimension D. The mesh function is
    /// cached for later access and will be computed on the first call
    /// to this function.
    boost::shared_ptr<const MeshFunction<unsigned int> >
    cell_domains(const Mesh& mesh) const;

    /// Get facet domains. This function computes the mesh function
    /// corresponding to markers of dimension D-1. The mesh function
    /// is cached for later access and will be computed on the first
    /// call to this function.
    boost::shared_ptr<const MeshFunction<unsigned int> >
    facet_domains(const Mesh& mesh) const;

    /// Initialize mesh domains for given topological dimension
    void init(uint dim);

    /// Clear all data
    void clear();

  private:

    // Initialize mesh function corresponding to markers
    void init_domains(MeshFunction<uint>& mesh_function) const;

    // Subdomain markers
    std::vector<boost::shared_ptr<MeshValueCollection<uint> > > _markers;

    // Mesh function for cell domains
    mutable boost::shared_ptr<MeshFunction<uint> > _cell_domains;

    // Mesh function for facet domains (exterior or interior)
    mutable boost::shared_ptr<MeshFunction<uint> > _facet_domains;

  };

}

#endif
