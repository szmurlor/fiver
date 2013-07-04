// Copyright (C) 2007-2011 Anders Logg
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
// First added:  2007-04-10
// Last changed: 2011-09-15

#ifndef __SUB_DOMAIN_H
#define __SUB_DOMAIN_H

#include <dolfin/common/types.h>

namespace dolfin
{

  // Forward declarations
  template <typename T> class MeshFunction;
  template <typename T> class MeshValueCollection;
  template <typename T> class Array;

  /// This class defines the interface for definition of subdomains.
  /// Alternatively, subdomains may be defined by a _Mesh_ and a
  /// _MeshFunction_ <uint> over the mesh.

  class SubDomain
  {
  public:

    /// Constructor
    SubDomain();

    /// Destructor
    virtual ~SubDomain();

    /// Return true for points inside the subdomain
    ///
    /// *Arguments*
    ///     x (_Array_ <double>)
    ///         The coordinates of the point.
    ///     on_boundary (bool)
    ///         True for points on the boundary.
    ///
    /// *Returns*
    ///     bool
    ///         True for points inside the subdomain.
    virtual bool inside(const Array<double>& x, bool on_boundary) const;

    /// Map coordinate x in domain H to coordinate y in domain G (used for
    /// periodic boundary conditions)
    ///
    /// *Arguments*
    ///     x (_Array_ <double>)
    ///         The coordinates in domain H.
    ///     unnamed (_Array_ <double>)
    ///         The coordinates in domain G.
    virtual void map(const Array<double>& x, Array<double>&) const;

    /// Snap coordinate to boundary of subdomain
    ///
    /// *Arguments*
    ///     x (_Array_ <double>)
    ///         The coordinates.
    virtual void snap(Array<double>& x) const {}

    //--- Marking of Mesh ---

    /// Set subdomain markers (uint) on cells for given subdomain number
    ///
    /// *Arguments*
    ///     mesh (_Mesh_)
    ///         The mesh to be marked.
    ///     sub_domain (unsigned int)
    ///         The subdomain number.
    void mark_cells(Mesh& mesh, unsigned int sub_domain) const;

    /// Set subdomain markers (uint) on facets for given subdomain number
    ///
    /// *Arguments*
    ///     mesh (_Mesh_)
    ///         The mesh to be marked.
    ///     sub_domain (unsigned int)
    ///         The subdomain number.
    void mark_facets(Mesh& mesh, unsigned int sub_domain) const;

    /// Set subdomain markers (uint) for given topological dimension
    /// and subdomain number
    ///
    /// *Arguments*
    ///     mesh (_Mesh_)
    ///         The mesh to be marked.
    ///     dim (unsigned int)
    ///         The topological dimension of entities to be marked.
    ///     sub_domain (unsigned int)
    ///         The subdomain number.
    void mark(Mesh& mesh, unsigned int dim, unsigned int sub_domain) const;

    //--- Marking of MeshFunction ---

    /// Set subdomain markers (uint) for given subdomain number
    ///
    /// *Arguments*
    ///     sub_domains (_MeshFunction_ <unsigned int>)
    ///         The subdomain markers.
    ///     sub_domain (unsigned int)
    ///         The subdomain number.
    void mark(MeshFunction<unsigned int>& sub_domains,
              unsigned int sub_domain) const;

    /// Set subdomain markers (int) for given subdomain number
    ///
    /// *Arguments*
    ///     sub_domains (_MeshFunction_ <int>)
    ///         The subdomain markers.
    ///     sub_domain (int)
    ///         The subdomain number.
    void mark(MeshFunction<int>& sub_domains, int sub_domain) const;

    /// Set subdomain markers (double) for given subdomain number
    ///
    /// *Arguments*
    ///     sub_domains (_MeshFunction_ <double>)
    ///         The subdomain markers.
    ///     sub_domain (double)
    ///         The subdomain number.
    void mark(MeshFunction<double>& sub_domains, double sub_domain) const;

    /// Set subdomain markers (bool) for given subdomain
    ///
    /// *Arguments*
    ///     sub_domains (_MeshFunction_ <bool>)
    ///         The subdomain markers.
    ///     sub_domain (bool)
    ///         The subdomain number.
    void mark(MeshFunction<bool>& sub_domains, bool sub_domain) const;

    //--- Marking of MeshValueCollection ---

    /// Set subdomain markers (uint) for given subdomain number
    ///
    /// *Arguments*
    ///     sub_domains (_MeshValueCollection_ <unsigned int>)
    ///         The subdomain markers.
    ///     sub_domain (unsigned int)
    ///         The subdomain number.
    ///     mesn (_Mesh_)
    ///         The mesh.
    void mark(MeshValueCollection<unsigned int>& sub_domains,
              unsigned int sub_domain,
              const Mesh& mesh) const;

    /// Set subdomain markers (int) for given subdomain number
    ///
    /// *Arguments*
    ///     sub_domains (_MeshValueCollection_ <int>)
    ///         The subdomain markers
    ///     sub_domain (int)
    ///         The subdomain number
    void mark(MeshValueCollection<int>& sub_domains,
              int sub_domain,
              const Mesh& mesh) const;

    /// Set subdomain markers (double) for given subdomain number
    ///
    /// *Arguments*
    ///     sub_domains (_MeshValueCollection_ <double>)
    ///         The subdomain markers.
    ///     sub_domain (double)
    ///         The subdomain number
    void mark(MeshValueCollection<double>& sub_domains,
              double sub_domain,
              const Mesh& mesh) const;

    /// Set subdomain markers (bool) for given subdomain
    ///
    /// *Arguments*
    ///     sub_domains (_MeshValueCollection_ <bool>)
    ///         The subdomain markers
    ///     sub_domain (bool)
    ///         The subdomain number
    void mark(MeshValueCollection<bool>& sub_domains,
              bool sub_domain,
              const Mesh& mesh) const;

    /// Return geometric dimension
    ///
    /// *Returns*
    ///     uint
    ///         The geometric dimension.
    uint geometric_dimension() const;

  private:

    /// Apply marker of type T (most likely an uint) to object of class
    /// S (most likely MeshFunction or MeshValueCollection)
    template<typename S, typename T>
    void apply_markers(S& sub_domains, T sub_domain, const Mesh& mesh) const;

    // Friends
    friend class DirichletBC;
    friend class PeriodicBC;

    // Geometric dimension, needed for SWIG interface, will be set before
    // calls to inside() and map()
    mutable uint _geometric_dimension;

  };

}

#endif
