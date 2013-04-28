// Copyright (C) 2007-2011 Garth N. Wells
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
// Modified by Anders Logg, 2007-2009.
//
// First added:  2007-03-13
// Last changed: 2011-01-02

#ifndef __SPARSITY_PATTERN_H
#define __SPARSITY_PATTERN_H

#include <set>
#include <utility>
#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include "dolfin/common/Set.h"
#include "dolfin/common/types.h"
#include "GenericSparsityPattern.h"

namespace dolfin
{

  /// This class implements the GenericSparsityPattern interface.
  /// It is used by most linear algebra backends.

  class SparsityPattern : public GenericSparsityPattern
  {

    // Set type used for the rows of the sparsity pattern
    //typedef dolfin::Set<uint> set_type;
    //typedef std::set<uint> set_type;
    typedef boost::unordered_set<dolfin::uint> set_type;

  public:

    /// Create empty sparsity pattern
    SparsityPattern();

    /// Initialize sparsity pattern for a generic tensor
    void init(const std::vector<uint>& dims,
              const std::vector<std::pair<uint, uint> >& ownership_range,
              const std::vector<const boost::unordered_map<uint, uint>* > off_process_owner);

    /// Insert non-zero entries
    void insert(const std::vector<const std::vector<uint>* >& entries);

    /// Return rank
    uint rank() const;

    /// Return global size for dimension i
    uint size(uint i) const;

    /// Return local range for dimension dim
    std::pair<uint, uint> local_range(uint dim) const;

    /// Return total number of nonzeros in local_range for dimension 0
    uint num_nonzeros() const;

    /// Fill array with number of nonzeros for diagonal block in local_range for dimension 0
    /// For matrices, fill array with number of nonzeros per local row for diagonal block
    void num_nonzeros_diagonal(std::vector<uint>& num_nonzeros) const;

    /// Fill array with number of nonzeros for off-diagonal block in local_range for dimension 0
    /// For matrices, fill array with number of nonzeros per local row for off-diagonal block
    void num_nonzeros_off_diagonal(std::vector<uint>& num_nonzeros) const;

    /// Finalize sparsity pattern
    void apply();

    /// Return informal string representation (pretty-print)
    std::string str() const;

    /// Return underlying sparsity pattern (diagonal). Options are
    /// 'sorted' and 'unsorted'.
    std::vector<std::vector<uint> > diagonal_pattern(Type type) const;

    /// Return underlying sparsity pattern (off-diagional). Options are
    /// 'sorted' and 'unsorted'.
    std::vector<std::vector<uint> > off_diagonal_pattern(Type type) const;

  private:

    // Print some useful information
    void info_statistics() const;

    // Shape of tensor
    std::vector<uint> shape;

    // Local range
    uint row_range_min;
    uint row_range_max;
    uint col_range_min;
    uint col_range_max;

    // Sparsity patterns for diagonal and off-diagonal blocks
    std::vector<set_type> diagonal;
    std::vector<set_type> off_diagonal;

    // Sparsity pattern for non-local entries stored as [i, j, i, j, ...]
    std::vector<uint> non_local;

    // Ownership range for each dimension
    std::vector<std::pair<uint, uint> > ownership_range;

    // Map from non-local vertex owning process index
    std::vector<boost::unordered_map<uint, uint> > off_process_owner;

  };

}
#endif
