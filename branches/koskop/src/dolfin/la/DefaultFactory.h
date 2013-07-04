// Copyright (C) 2008-2011 Anders Logg
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
// First added:  2008-05-17
// Last changed: 2011-10-19

#ifndef __DEFAULT_FACTORY_H
#define __DEFAULT_FACTORY_H

#include <string>
#include "LinearAlgebraFactory.h"

namespace dolfin
{

  class DefaultFactory : public LinearAlgebraFactory
  {

    class GenericLinearSolver;

    public:

    /// Constructor
    DefaultFactory() {}

    /// Destructor
    virtual ~DefaultFactory() {}

    /// Create empty matrix
    virtual dolfin::GenericMatrix* create_matrix() const;

    /// Create empty vector (global)
    virtual dolfin::GenericVector* create_vector() const;

    /// Create empty vector (local)
    virtual dolfin::GenericVector* create_local_vector() const;

    /// Create empty sparsity pattern
    virtual dolfin::GenericSparsityPattern* create_pattern() const;

    /// Create LU solver
    virtual dolfin::GenericLinearSolver* create_lu_solver(std::string method) const;

    /// Create Krylov solver
    virtual dolfin::GenericLinearSolver*
    create_krylov_solver(std::string method, std::string preconditioner) const;

    /// Return a list of available LU solver methods
    std::vector<std::pair<std::string, std::string> >
    lu_solver_methods() const;

    /// Return a list of available Krylov solver methods
    std::vector<std::pair<std::string, std::string> >
    krylov_solver_methods() const;

    /// Return a list of available preconditioners
    std::vector<std::pair<std::string, std::string> >
    krylov_solver_preconditioners() const;

    /// Return instance of default backend
    static LinearAlgebraFactory& factory();

  };

}

#endif
