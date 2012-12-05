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
// First added:  2006-06-01
// Last changed: 2010-08-16

#ifndef __VERTEX_H
#define __VERTEX_H

#include "common/types.h"
#include "Mesh.h"
#include "MeshEntity.h"
#include "MeshEntityIterator.h"
#include "MeshFunction.h"
#include "Point.h"

namespace dolfin
{

  /// A Vertex is a MeshEntity of topological dimension 0.

  class Vertex : public MeshEntity
  {
  public:

    /// Create vertex on given mesh
    Vertex(const Mesh& mesh, uint index) : MeshEntity(mesh, 0, index) {}

    /// Create vertex from mesh entity
    Vertex(MeshEntity& entity) : MeshEntity(entity.mesh(), 0, entity.index()) {}

    /// Destructor
    ~Vertex() {}

    /// Return value of vertex coordinate i
    double x(uint i) const { return _mesh->geometry().x(_index, i); }

    /// Return vertex coordinates as a 3D point value
    Point point() const { return _mesh->geometry().point(_index); }

    /// Return array of vertex coordinates (const version)
    const double* x() const { return _mesh->geometry().x(_index); }

  };

  /// A VertexIterator is a MeshEntityIterator of topological dimension 0.

  class VertexIterator : public MeshEntityIterator
  {
  public:

    VertexIterator(const Mesh& mesh) : MeshEntityIterator(mesh, 0) {}
    VertexIterator(const MeshEntity& entity) : MeshEntityIterator(entity, 0) {}

    inline const Vertex& operator*() { return *operator->(); }
    inline const Vertex* operator->() { return static_cast<Vertex*>(MeshEntityIterator::operator->()); }

  };

  /// A VertexFunction is a MeshFunction of topological dimension 0.

  template <typename T> class VertexFunction : public MeshFunction<T>
  {
  public:

    VertexFunction(const Mesh& mesh) : MeshFunction<T>(mesh, 0) {}

    VertexFunction(const Mesh& mesh, const T& value)
        : MeshFunction<T>(mesh, 0, value) {}

  };

}

#endif
