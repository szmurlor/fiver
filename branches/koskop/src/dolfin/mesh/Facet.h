// Copyright (C) 2006-2010 Anders Logg
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
// Modified by Garth N. Wells, 2009-2011.
//
// First added:  2006-06-02
// Last changed: 2011-04-07

#ifndef __FACET_H
#define __FACET_H

#include <utility>
#include <dolfin/common/types.h>
#include "Cell.h"
#include "Mesh.h"
#include "MeshEntity.h"
#include "MeshEntityIterator.h"
#include "MeshFunction.h"

namespace dolfin
{

  /// A Facet is a MeshEntity of topological codimension 1.

  class Facet : public MeshEntity
  {
  public:

    /// Constructor
    Facet(const Mesh& mesh, uint index) : MeshEntity(mesh, mesh.topology().dim() - 1, index) {}

    /// Destructor
    ~Facet() {}

    /// Compute component i of the normal to the facet
    double normal(uint i) const;

    /// Compute normal to the facet
    Point normal() const;

    /// Return true if facet is an exterior facet (relative to global mesh,
    /// so this function will return false for facets on partition boundaries)
    /// Facet connectivity must be initialized before calling this function.
    bool exterior() const;

    // FIXME: This function should take care of the case where adjacent cells
    // FIXME: live on different processes

    /// Return adjacent cells. An optional argument that lists for
    /// each facet the index of the first cell may be given to specify
    /// the ordering of the two cells. If not specified, the ordering
    /// will depend on the (arbitrary) ordering of the mesh
    /// connectivity.
    std::pair<const Cell, const Cell>
    adjacent_cells(const MeshFunction<uint>* facet_orientation) const;

  };


  /// A FacetIterator is a MeshEntityIterator of topological codimension 1.

  class FacetIterator : public MeshEntityIterator
  {
  public:

    FacetIterator(const Mesh& mesh) : MeshEntityIterator(mesh, mesh.topology().dim() - 1) {}
    FacetIterator(const MeshEntity& entity) : MeshEntityIterator(entity, entity.mesh().topology().dim() - 1) {}

    const Facet& operator*() { return *operator->(); }
    const Facet* operator->() { return static_cast<Facet*>(MeshEntityIterator::operator->()); }

  };

  /// A FacetFunction is a MeshFunction of topological codimension 1.

  template <typename T> class FacetFunction : public MeshFunction<T>
  {
  public:

    FacetFunction(const Mesh& mesh)
      : MeshFunction<T>(mesh, mesh.topology().dim() - 1) {}

    FacetFunction(const Mesh& mesh, const T& value)
      : MeshFunction<T>(mesh, mesh.topology().dim() - 1, value) {}
  };

}

#endif
