// Copyright (C) 2006-2010 Garth N. Wells
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
// Modified by Anders Logg 2006-2011
// Modified by Kent-Andre Mardal 2008
// Modified by Ola Skavhaug 2008
// Modified by Martin Sandve Alnes 2009
// Modified by Johan Hake 2009-2010
//
// First added:  2006-04-25
// Last changed: 2011-11-11

#ifndef __GENERIC_VECTOR_H
#define __GENERIC_VECTOR_H

#include <algorithm>
#include <utility>
#include <vector>
#include <dolfin/log/log.h>
#include "GenericSparsityPattern.h"
#include "GenericTensor.h"

namespace dolfin
{
  template<typename T> class Array;

  /// This class defines a common interface for vectors.

  class GenericVector : public GenericTensor
  {
  public:

    /// Destructor
    virtual ~GenericVector() {}

    //--- Implementation of the GenericTensor interface ---

    /// Resize tensor with given dimensions
    virtual void resize(uint rank, const uint* dims)
    { dolfin_assert(rank == 1); resize(dims[0]); }

    /// Initialize zero tensor using sparsity pattern
    virtual void init(const GenericSparsityPattern& sparsity_pattern)
    { resize(sparsity_pattern.local_range(0)); zero(); }

    /// Return copy of tensor
    virtual GenericVector* copy() const = 0;

    /// Return tensor rank (number of dimensions)
    virtual uint rank() const
    { return 1; }

    /// Return size of given dimension
    virtual uint size(uint dim) const
    { dolfin_assert(dim == 0); return size(); }

    /// Return local ownership range
    virtual std::pair<uint, uint> local_range(uint dim) const
    { dolfin_assert(dim == 0); return local_range(); }

    /// Get block of values
    virtual void get(double* block, const uint* num_rows,
                     const uint * const * rows) const
    { get_local(block, num_rows[0], rows[0]); }

    /// Set block of values
    virtual void set(const double* block, const uint* num_rows,
                     const uint * const * rows)
    { set(block, num_rows[0], rows[0]); }

    /// Add block of values
    virtual void add(const double* block, const uint* num_rows,
                     const uint * const * rows)
    { add(block, num_rows[0], rows[0]); }

    /// Add block of values
    virtual void add(const double* block, const std::vector<const std::vector<uint>* >& rows)
    { add(block, rows[0]->size(), &(*rows[0])[0]); }

    /// Add block of values
    virtual void add(const double* block, const std::vector<std::vector<uint> >& rows)
    { add(block, rows[0].size(), &(rows[0])[0]); }

    /// Set all entries to zero and keep any sparse structure
    virtual void zero() = 0;

    /// Finalize assembly of tensor
    virtual void apply(std::string mode) = 0;

    /// Return informal string representation (pretty-print)
    virtual std::string str(bool verbose) const = 0;

    //--- Vector interface ---

    /// Resize vector to global size N
    virtual void resize(uint N) = 0;

    /// Resize vector with given ownership range
    virtual void resize(std::pair<uint, uint> range) = 0;

    /// Resize vector with given ownership range and with ghost values
    virtual void resize(std::pair<uint, uint> range,
                        const std::vector<uint>& ghost_indices) = 0;

    /// Return global size of vector
    virtual uint size() const = 0;

    /// Return local size of vector
    virtual uint local_size() const = 0;

    /// Return local ownership range of a vector
    virtual std::pair<uint, uint> local_range() const = 0;

    /// Determine whether global vector index is owned by this process
    virtual bool owns_index(uint i) const = 0;

    /// Get block of values (values may live on any process)
    virtual void get(double* block, uint m, const uint* rows) const
    {
      warning("GenericVector::get is redirected to GenericVector::get_local. Use GenericVector::gather for get off-process entries. GenericVector::get will be removed.");
      get_local(block, m, rows);
    }

    /// Get block of values (values must all live on the local process)
    virtual void get_local(double* block, uint m, const uint* rows) const = 0;

    /// Set block of values
    virtual void set(const double* block, uint m, const uint* rows) = 0;

    /// Add block of values
    virtual void add(const double* block, uint m, const uint* rows) = 0;

    /// Get all values on local process
    virtual void get_local(Array<double>& values) const = 0;

    /// Set all values on local process
    virtual void set_local(const Array<double>& values) = 0;

    /// Add values to each entry on local process
    virtual void add_local(const Array<double>& values) = 0;

    /// Gather entries into local vector x
    virtual void gather(GenericVector& x, const Array<uint>& indices) const = 0;

    /// Gather entries into Array x
    virtual void gather(Array<double>& x, const Array<uint>& indices) const = 0;

    /// Gather all entries into Array x on process 0
    virtual void gather_on_zero(Array<double>& x) const = 0;

    /// Add multiple of given vector (AXPY operation)
    virtual void axpy(double a, const GenericVector& x) = 0;

    /// Replace all entries in the vector by their absolute values
    virtual void abs() = 0;

    /// Return inner product with given vector
    virtual double inner(const GenericVector& x) const = 0;

    /// Return norm of vector
    virtual double norm(std::string norm_type) const = 0;

    /// Return minimum value of vector
    virtual double min() const = 0;

    /// Return maximum value of vector
    virtual double max() const = 0;

    /// Return sum of vector
    virtual double sum() const = 0;

    /// Return sum of selected rows in vector. Repeated entries are only summed once.
    virtual double sum(const Array<uint>& rows) const = 0;

    /// Multiply vector by given number
    virtual const GenericVector& operator*= (double a) = 0;

    /// Multiply vector by another vector pointwise
    virtual const GenericVector& operator*= (const GenericVector& x) = 0;

    /// Divide vector by given number
    virtual const GenericVector& operator/= (double a) = 0;

    /// Add given vector
    virtual const GenericVector& operator+= (const GenericVector& x) = 0;

    /// Subtract given vector
    virtual const GenericVector& operator-= (const GenericVector& x) = 0;

    /// Assignment operator
    virtual const GenericVector& operator= (const GenericVector& x) = 0;

    /// Assignment operator
    virtual const GenericVector& operator= (double a) = 0;

    /// Return pointer to underlying data (const version)
    virtual const double* data() const
    {
      dolfin_error("GenericVector.h",
                   "return pointer to underlying vector data",
                   "Not implemented by current linear algebra backend");
      return 0;
    }

    /// Return pointer to underlying data
    virtual double* data()
    {
      dolfin_error("GenericVector.h",
                   "return pointer to underlying vector data",
                   "Not implemented by current linear algebra backend");
      return 0;
    }

    /// Update ghost values
    virtual void update_ghost_values()
    {
      dolfin_error("GenericVector.h",
                   "update ghost values",
                   "Not implemented by current linear algebra backend");
    }

    //--- Convenience functions ---

    /// Get value of given entry
    virtual double operator[] (uint i) const
    { double value(0); get_local(&value, 1, &i); return value; }

    /// Get value of given entry
    virtual double getitem(uint i) const
    { double value(0); get_local(&value, 1, &i); return value; }

    /// Set given entry to value. apply("insert") should be called before using
    /// using the object.
    virtual void setitem(uint i, double value)
    { set(&value, 1, &i); }

  };

}

#endif
