// Copyright (C) 2008-2011 Garth N. Wells
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
// First added:  2008-01-07
// Last changed: 2011-01-23

#ifndef __SUB_SYSTEMS_MANAGER_H
#define __SUB_SYSTEMS_MANAGER_H

namespace dolfin
{

  /// This is a singleton class which manages the initialisation and
  /// finalisation of various sub systems, such as MPI and PETSc.

  class SubSystemsManager
  {
  public:

    /// Initialise MPI
    static void init_mpi();

    /// Initialize PETSc without command-line arguments
    static void init_petsc();

    /// Initialize PETSc with command-line arguments. Note that PETSc
    /// command-line arguments may also be filtered and sent to PETSc
    /// by parameters.parse(argc, argv).
    static void init_petsc(int argc, char* argv[]);

    /// Finalize subsytems. This will be called by the destructor, but in
    /// special cases it may be necessary to call finalize() explicitly.
    static void finalize();

    /// Return true if DOLFIN intialised MPI (and is therefore responsible
    //  for finalization)
    static bool responsible_mpi();

    /// Return true if DOLFIN intialised PETSc (and is therefore responsible
    //  for finalization)
    static bool responsible_petsc();

    // Check if MPI has been initialised (returns true if MPI has been
    // initialised, even if it is later finalised)
    static bool mpi_initialized();

    // Check if MPI has been finalized (returns true if MPI has been
    // finalised)
    static bool mpi_finalized();

  private:

    // Constructor (private)
    SubSystemsManager();

    // Copy constructor (private)
    SubSystemsManager(const SubSystemsManager& sub_sys_manager);

    // Destructor
    ~SubSystemsManager();

    // Finalize MPI
    static void finalize_mpi();

    // Finalize PETSc
    static void finalize_petsc();

    // Singleton instance
    static SubSystemsManager& singleton();

    // State variables
    bool petsc_initialized;
    bool control_mpi;

  };

}

#endif
