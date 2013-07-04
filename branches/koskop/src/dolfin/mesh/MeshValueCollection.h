// Copyright (C) 2011 Anders Logg and Garth N. Wells
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
// First added:  2006-08-30
// Last changed: 2011-11-15

#ifndef __MESH_VALUE_COLLECTION_H
#define __MESH_VALUE_COLLECTION_H


#include <map>
#include <utility>
#include <boost/shared_ptr.hpp>
#include <dolfin/common/MPI.h>
#include <dolfin/common/Variable.h>
#include <dolfin/log/log.h>
#include "Cell.h"
#include "LocalMeshValueCollection.h"
#include "Mesh.h"
#include "MeshEntity.h"
#include "MeshFunction.h"
#include "MeshPartitioning.h"

namespace dolfin
{

  /// The MeshValueCollection class can be used to store data
  /// associated with a subset of the entities of a mesh of a given
  /// topological dimension. It differs from the MeshFunction class in
  /// two ways. First, data does not need to be associated with all
  /// entities (only a subset). Second, data is associated with
  /// entities through the corresponding cell index and local entity
  /// number (relative to the cell), not by global entity index, which
  /// means that data may be stored robustly to file.

  template <typename T>
  class MeshValueCollection : public Variable
  {
  public:

    /// Create empty mesh value collection
    ///
    MeshValueCollection();

    /// Create empty mesh value collection of given dimension
    ///
    /// *Arguments*
    ///     dim (uint)
    ///         The mesh entity dimension for the mesh value collection.
    explicit MeshValueCollection(uint dim);

    /// Create a mesh value collection from a MeshFunction
    ///
    /// *Arguments*
    ///     mesh_function (_MeshFunction_ <T>)
    ///         The mesh function for creating a MeshValueCollection.
    explicit MeshValueCollection(const MeshFunction<T>& mesh_function);

    /// Create a mesh value collection from a file.
    ///
    /// *Arguments*
    ///     mesh (Mesh)
    ///         A mesh associated with the collection. The mesh is used to
    ///         map collection values to the appropriate process.
    ///     filename (std::string)
    ///         The XML file name.
    ///     dim (uint)
    ///         The mesh entity dimension for the mesh value collection.
    MeshValueCollection(const Mesh& mesh, const std::string filename, uint dim);

    /// Destructor
    ~MeshValueCollection()
    {}

    /// Assignment operator
    ///
    /// *Arguments*
    ///     mesh_function (_MeshFunction_)
    ///         A _MeshFunction_ object used to construct a MeshValueCollection.
    MeshValueCollection<T>& operator=(const MeshFunction<T>& mesh_function);

    /// Assignment operator
    ///
    /// *Arguments*
    ///     mesh_value_collection (_MeshValueCollection_)
    ///         A _MeshValueCollection_ object used to construct a MeshValueCollection.
    MeshValueCollection<T>& operator=(const MeshValueCollection<T>& mesh_value_collection);

    /// Set the topological dimension
    ///
    /// *Arguments*
    ///     dim (uint)
    ///         The mesh entity dimension for the mesh value collection.
    void set_dim(uint dim);

    /// Return topological dimension
    ///
    /// *Returns*
    ///     uint
    ///         The dimension.
    uint dim() const;

    /// Return size (number of entities in subset)
    ///
    /// *Returns*
    ///     uint
    ///         The size.
    uint size() const;

    /// Set marker value for given entity defined by a cell index and
    /// a local entity index
    ///
    /// *Arguments*
    ///     cell_index (uint)
    ///         The index of the cell.
    ///     local_entity (uint)
    ///         The local index of the entity relative to the cell.
    ///     marker_value (T)
    ///         The value of the marker.
    ///
    /// *Returns*
    ///     bool
    ///         True is a new value is inserted, false if overwriting
    ///         an existing value.
    bool set_value(uint cell_index, uint local_entity, const T& value);

    /// Set value for given entity index
    ///
    /// *Arguments*
    ///     entity_index (uint)
    ///         Index of the entity.
    ///     value (T).
    ///         The value of the marker.
    ///     mesh (_Mesh_)
    ///         The mesh.
    ///
    /// *Returns*
    ///     bool
    ///         True is a new value is inserted, false if overwriting
    ///         an existing value.
    bool set_value(uint entity_index, const T& value, const Mesh& mesh);

    /// Get marker value for given entity defined by a cell index and
    /// a local entity index
    ///
    /// *Arguments*
    ///     cell_index (uint)
    ///         The index of the cell.
    ///     local_entity (uint)
    ///         The local index of the entity relative to the cell.
    ///
    /// *Returns*
    ///     marker_value (T)
    ///         The value of the marker.
    T get_value(uint cell_index, uint local_entity);

    /// Get all values
    ///
    /// *Returns*
    ///     std::map<std::pair<uint, uint>, T>
    ///         A map from positions to values.
    std::map<std::pair<uint, uint>, T>& values();

    /// Get all values (const version)
    ///
    /// *Returns*
    ///     std::map<std::pair<uint, uint>, T>
    ///         A map from positions to values.
    const std::map<std::pair<uint, uint>, T>& values() const;

    /// Clear all values
    void clear();

    /// Return informal string representation (pretty-print)
    ///
    /// *Arguments*
    ///     verbose (bool)
    ///         Flag to turn on additional output.
    ///
    /// *Returns*
    ///     std::string
    ///         An informal representation.
    std::string str(bool verbose) const;

  private:

    // The values
    std::map<std::pair<uint, uint>, T> _values;

    /// Topological dimension
    uint _dim;

  };

  //---------------------------------------------------------------------------
  // Implementation of MeshValueCollection
  //---------------------------------------------------------------------------
  template <typename T>
  MeshValueCollection<T>::MeshValueCollection()
    : Variable("m", "unnamed MeshValueCollection"), _dim(0)
  {
    // Do nothing
  }
  //---------------------------------------------------------------------------
  template <typename T>
  MeshValueCollection<T>::MeshValueCollection(uint dim)
    : Variable("m", "unnamed MeshValueCollection"), _dim(dim)
  {
    // Do nothing
  }
  //---------------------------------------------------------------------------
  template <typename T>
  MeshValueCollection<T>::MeshValueCollection(const MeshFunction<T>& mesh_function)
    : Variable("m", "unnamed MeshValueCollection"), _dim(mesh_function.dim())
  {
    const Mesh& mesh = mesh_function.mesh();
    const uint D = mesh.topology().dim();

    // FIXME: Use iterators

    // Handle cells as a special case
    if (D == _dim)
    {
      for (uint cell_index = 0; cell_index < mesh_function.size(); ++cell_index)
      {
        const std::pair<uint, uint> key(cell_index, 0);
        _values.insert(std::make_pair(key, mesh_function[cell_index]));
      }
    }
    else
    {
      mesh.init(_dim, D);
      const MeshConnectivity& connectivity = mesh.topology()(_dim, D);
      dolfin_assert(connectivity.size() > 0);
      for (uint entity_index = 0; entity_index < mesh_function.size(); ++entity_index)
      {
        // Find the cell
        dolfin_assert(connectivity.size(entity_index) > 0);
        const MeshEntity entity(mesh, _dim, entity_index);
        for (uint i = 0; i < entity.num_entities(D) ; ++i)
        {
          // Create cell
          const Cell cell(mesh, connectivity(entity_index)[i]);

          // Find the local entity index
          const uint local_entity = cell.index(entity);

          // Insert into map
          const std::pair<uint, uint> key(cell.index(), local_entity);
          _values.insert(std::make_pair(key, mesh_function[entity_index]));
        }
      }
    }
  }
  //---------------------------------------------------------------------------
  template <typename T>
  MeshValueCollection<T>::MeshValueCollection(const Mesh& mesh,
    const std::string filename, uint dim)
  : Variable("m", "unnamed MeshValueCollection"), _dim(dim)
  {
    if (MPI::num_processes() == 1)
    {
      File file(filename);
      file >> *this;
    }
    else
    {
      // Read file on process 0
      MeshValueCollection<T> tmp_collection(dim);
      if (MPI::process_number() == 0)
      {
        File file(filename);
        file >> tmp_collection;
      }

      // Create local data and build value collection
      LocalMeshValueCollection<T> local_data(tmp_collection, dim);

      // Build mesh value collection
      MeshPartitioning::build_distributed_value_collection(*this, local_data,
                                                           mesh);
    }
  }
  //---------------------------------------------------------------------------
  template <typename T>
  MeshValueCollection<T>& MeshValueCollection<T>::operator=(const MeshFunction<T>& mesh_function)
  {
    _dim = mesh_function.dim();

    const Mesh& mesh = mesh_function.mesh();
    const uint D = mesh.topology().dim();

    // FIXME: Use iterators

    // Handle cells as a special case
    if (D == _dim)
    {
      for (uint cell_index = 0; cell_index < mesh_function.size(); ++cell_index)
      {
        const std::pair<uint, uint> key(cell_index, 0);
        _values.insert(std::make_pair(key, mesh_function[cell_index]));
      }
    }
    else
    {
      mesh.init(_dim, D);
      const MeshConnectivity& connectivity = mesh.topology()(_dim, D);
      dolfin_assert(connectivity.size() > 0);
      for (uint entity_index = 0; entity_index < mesh_function.size(); ++entity_index)
      {
        // Find the cell
        dolfin_assert(connectivity.size(entity_index) > 0);
        const MeshEntity entity(mesh, _dim, entity_index);
        for (uint i = 0; i < entity.num_entities(D) ; ++i)
        {
          // Create cell
          const Cell cell(mesh, connectivity(entity_index)[i]);

          // Find the local entity index
          const uint local_entity = cell.index(entity);

          // Insert into map
          const std::pair<uint, uint> key(cell.index(), local_entity);
          _values.insert(std::make_pair(key, mesh_function[entity_index]));
        }
      }
    }

    return *this;
  }
  //---------------------------------------------------------------------------
  template <typename T>
  MeshValueCollection<T>& MeshValueCollection<T>::operator=(const MeshValueCollection<T>& mesh_value_collection)
  {
    _dim = mesh_value_collection.dim();
    _values = mesh_value_collection.values();

    return *this;
  }
  //---------------------------------------------------------------------------
  template <typename T>
  void MeshValueCollection<T>::set_dim(uint dim)
  {
    _dim = dim;
  }
  //---------------------------------------------------------------------------
  template <typename T>
  uint MeshValueCollection<T>::dim() const
  {
    return _dim;
  }
  //---------------------------------------------------------------------------
  template <typename T>
  uint MeshValueCollection<T>::size() const
  {
    return _values.size();
  }
  //---------------------------------------------------------------------------
  template <typename T>
  bool MeshValueCollection<T>::set_value(uint cell_index,
                                         uint local_entity,
                                         const T& value)
  {
    const std::pair<uint, uint> pos(std::make_pair(cell_index, local_entity));
    std::pair<typename std::map<std::pair<uint, uint>, T>::iterator, bool> it;
    it = _values.insert(std::make_pair(pos, value));
    return it.second;
  }
  //---------------------------------------------------------------------------
  template <typename T>
  bool MeshValueCollection<T>::set_value(uint entity_index,
                                         const T& value,
                                         const Mesh& mesh)
  {
    // Special case when d = D
    const uint D = mesh.topology().dim();
    if (_dim == D)
    {
      // Set local entity index to zero when we mark a cell
      const std::pair<uint, uint> pos(std::make_pair(entity_index, 0));
      std::pair<typename std::map<std::pair<uint, uint>, T>::iterator, bool> it;
      it = _values.insert(std::make_pair(pos, value));
      return it.second;
    }

    // Get mesh connectivity d --> D
    mesh.init(_dim, D);
    const MeshConnectivity& connectivity = mesh.topology()(_dim, D);

    // Find the cell
    dolfin_assert(connectivity.size() > 0);
    dolfin_assert(connectivity.size(entity_index) > 0);
    const MeshEntity entity(mesh, _dim, entity_index);
    const Cell cell(mesh, connectivity(entity_index)[0]); // choose first

    // Find the local entity index
    const uint local_entity = cell.index(entity);

    // Add value
    const std::pair<uint, uint> pos(std::make_pair(cell.index(), local_entity));
    std::pair<typename std::map<std::pair<uint, uint>, T>::iterator, bool> it;
    it = _values.insert(std::make_pair(pos, value));
    return it.second;
  }
  //---------------------------------------------------------------------------
  template <typename T>
  T MeshValueCollection<T>::get_value(uint cell_index,
				      uint local_entity)
  {
    const std::pair<uint, uint> pos(std::make_pair(cell_index, local_entity));
    const typename std::map<std::pair<uint, uint>, T>::const_iterator it =
      _values.find(pos);

    if (it == _values.end())
      dolfin_error("MeshValueCollection.h",
                   "extract value",
                   "No value stored for cell index: %d and local index: %d",
                   cell_index, local_entity);

    return it->second;
  }
  //---------------------------------------------------------------------------
  template <typename T>
  std::map<std::pair<uint, uint>, T>& MeshValueCollection<T>::values()
  {
    return _values;
  }
  //---------------------------------------------------------------------------
  template <typename T>
  const std::map<std::pair<uint, uint>, T>& MeshValueCollection<T>::values() const
  {
    return _values;
  }
  //---------------------------------------------------------------------------
  template <typename T>
  void MeshValueCollection<T>::clear()
  {
    _values.clear();
  }
  //---------------------------------------------------------------------------
  template <typename T>
  std::string MeshValueCollection<T>::str(bool verbose) const
  {
    std::stringstream s;

    if (verbose)
    {
      s << str(false) << std::endl << std::endl;
      warning("Verbose output of MeshValueCollection must be implemented manually.");
    }
    else
      s << "<MeshValueCollection of topological dimension " << dim()
        << " containing " << size() << " values>";

    return s.str();
  }
  //---------------------------------------------------------------------------

}

#endif
