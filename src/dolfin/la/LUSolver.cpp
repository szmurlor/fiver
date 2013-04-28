// Copyright (C) 2010 Garth N. Wells
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
// Modified by Anders Logg 2011
//
// First added:  2010-07-11
// Last changed: 2011-10-20

#include <dolfin/parameter/GlobalParameters.h>
#include <dolfin/common/NoDeleter.h>
#include <dolfin/common/Timer.h>
#include "DefaultFactory.h"
#include "LinearSolver.h"
#include "LUSolver.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
LUSolver::LUSolver(std::string method)
{
  // Initialize solver
  init(method);
}
//-----------------------------------------------------------------------------
LUSolver::LUSolver(boost::shared_ptr<const GenericMatrix> A,
                   std::string method)
{
  // Initialize solver
  init(method);

  // Set operator
  set_operator(A);
}
//-----------------------------------------------------------------------------
LUSolver::~LUSolver()
{
  // Do nothing
}
//-----------------------------------------------------------------------------
void LUSolver::set_operator(const boost::shared_ptr<const GenericMatrix> A)
{
  dolfin_assert(solver);
  solver->parameters.update(parameters);
  solver->set_operator(A);
}
//-----------------------------------------------------------------------------
dolfin::uint LUSolver::solve(GenericVector& x, const GenericVector& b)
{
  dolfin_assert(solver);

  Timer timer("LU solver");
  solver->parameters.update(parameters);
  return solver->solve(x, b);
}
//-----------------------------------------------------------------------------
dolfin::uint LUSolver::solve(const GenericMatrix& A, GenericVector& x,
                             const GenericVector& b)
{
  dolfin_assert(solver);

  Timer timer("LU solver");
  solver->parameters.update(parameters);
  return solver->solve(A, x, b);
}
//-----------------------------------------------------------------------------
void LUSolver::init(std::string method)
{
  // Get default linear algebra factory
  DefaultFactory factory;

  // Get list of available methods
  std::vector<std::pair<std::string, std::string> >
    methods = factory.lu_solver_methods();

  // Check that method is available
  if (!LinearSolver::in_list(method, methods))
  {
    dolfin_error("LUSolver.cpp",
                 "solve linear system using LU factorization",
                 "Unknown LU method \"%s\". "
                 "Use list_lu_solver_methods() to list available LU methods",
                 method.c_str());
  }

  // Set default parameters
  parameters = dolfin::parameters("lu_solver");

  // Initialize solver
  solver.reset(factory.create_lu_solver(method));
  solver->parameters.update(parameters);
}
//-----------------------------------------------------------------------------
