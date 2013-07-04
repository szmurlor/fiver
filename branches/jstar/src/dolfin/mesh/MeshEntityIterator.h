// Copyright (C) 2006-2008 Anders Logg
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
// Modified by Andre Massing, 2009.
//
// First added:  2006-05-09
// Last changed: 2011-11-08

#ifndef __MESH_ENTITY_ITERATOR_H
#define __MESH_ENTITY_ITERATOR_H

#include <common/types.h>
#include "Mesh.h"
#include "MeshEntity.h"

namespace dolfin
{

  /// MeshEntityIterator provides a common iterator for mesh entities
  /// over meshes, boundaries and incidence relations. The basic use
  /// is illustrated below.
  ///
  /// *Example*
  ///
  ///     The following example shows how to iterate over all mesh entities
  ///     of a mesh of topological dimension dim:
  ///
  ///     .. code-block:: c++
  ///
  ///         for (MeshEntityIterator e(mesh, dim); !e.end(); ++e)
  ///         {
  ///           e->foo();
  ///         }
  ///
  ///     The following example shows how to iterate over mesh entities of
  ///     topological dimension dim connected (incident) to some mesh entity f:
  ///
  ///     .. code-block:: c++
  ///
  ///         for (MeshEntityIterator e(f, dim); !e.end(); ++e)
  ///         {
  ///           e->foo();
  ///         }
  ///
  /// In addition to the general iterator, a set of specific named iterators
  /// are provided for entities of type _Vertex_, _Edge_, _Face_, _Facet_
  /// and _Cell_. These iterators are defined along with their respective
  /// classes.

  class MeshEntityIterator
  {
  public:

    /// Default constructor
    MeshEntityIterator() : _pos(0), pos_end(0), index(0) {}

    /// Create iterator for mesh entities over given topological dimension
    MeshEntityIterator(const Mesh& mesh, uint dim)
      : entity(), _pos(0), pos_end(mesh.size(dim)), index(0)
    {
      // Check if mesh is empty
      if (mesh.num_vertices() == 0)
        return;

      // Initialize mesh entity
      entity.init(mesh, dim, 0);

      // Update end position if entities need to be generated first
      if (pos_end == 0)
        pos_end = mesh.init(dim);
    }

    /// Create iterator for entities of given dimension connected to given entity
    MeshEntityIterator(const MeshEntity& entity, uint dim)
      : entity(entity.mesh(), dim, 0), _pos(0), index(0)
    {
      // Get connectivity
      const MeshConnectivity& c = entity.mesh().topology()(entity.dim(), dim);

      // Compute connectivity if empty
      if (c.size() == 0)
        entity.mesh().init(entity.dim(), dim);

      // Get size and index map
      if (c.size() == 0)
      {
        pos_end = 0;
        index = 0;
      }
      else
      {
        pos_end = c.size(entity.index());
        index = c(entity.index());
      }
    }

    /// Copy constructor
    MeshEntityIterator(const MeshEntityIterator& it)
      : entity(it.entity), _pos(it._pos), pos_end(it.pos_end), index(it.index) {}

    /// Destructor
    virtual ~MeshEntityIterator() {}

    /// Step to next mesh entity (prefix increment)
    MeshEntityIterator& operator++()
    {
      ++_pos;
      return *this;
    }

    /// Step to the previous mesh entity (prefix decrease)
    MeshEntityIterator& operator--()
    {
      --_pos;
      return *this;
    }

    /// Return current position
    uint pos() const
    { return _pos; }

    /// Comparison operator.
    bool operator==(const MeshEntityIterator & it) const
    {
      // Use const_cast to use operator* inside comparison, which automatically
      // updates the entity index corresponding to pos *before* comparison (since
      // update of entity delays until request for entity)

      return ((const_cast<MeshEntityIterator *>(this))->operator*()
            == (const_cast<MeshEntityIterator *>(&it))->operator*()
            && _pos == it._pos && index == it.index);
    }

    /// Comparison operator
    bool operator!=(const MeshEntityIterator & it) const
    { return !operator==(it); }

    /// Dereference operator
    MeshEntity& operator*()
    { return *operator->(); }

    /// Member access operator
    MeshEntity* operator->()
    { entity._index = (index ? index[_pos] : _pos); return &entity; }

    /// Random access operator.
    MeshEntity& operator[] (uint pos)
    { _pos = pos; return *operator->();}

    /// Check if iterator has reached the end
    bool end() const
    { return _pos >= pos_end; }

    /// Provide a safeguard iterator pointing beyond the end of an iteration
    /// process, either iterating over the mesh /or incident entities. Added to
    /// be bit more like STL iterators, since many algorithms rely on a kind of
    /// beyond iterator.
    MeshEntityIterator end_iterator()
    {
      MeshEntityIterator
      sg(*this);
      sg.set_end();
      return sg;
    }

    // Note: Not a subclass of Variable for efficiency!
    // Commented out to avoid warning about shadowing str() for MeshEntity
    // Return informal string representation (pretty-print)
    // std::string str(bool verbose) const;

  private:

    /// Set pos to end position. To create a kind of mesh.end() iterator.
    void set_end()
    { _pos = pos_end; }

    // Mesh entity
    MeshEntity entity;

    // Current position
    uint _pos;

    // End position
    uint pos_end;

    // Mapping from pos to index (if any)
    const uint* index;

  };

}

#endif
