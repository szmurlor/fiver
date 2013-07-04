// Copyright (C) 2011 Garth N. Wells
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
// First added:  2011-09-17
// Last changed: 2011-11-14

#include "dolfin/common/MPI.h"
#include "dolfin/log/log.h"
#include "dolfin/mesh/Mesh.h"
#include "ParallelData.h"
#include "MeshFunction.h"

#include "MeshDistributed.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
std::map<dolfin::uint, std::set<std::pair<dolfin::uint, dolfin::uint> > >
MeshDistributed::off_process_indices(const std::vector<uint>& entity_indices,
                                     uint dim, const Mesh& mesh)
{
  if (dim == 0)
    warning("MeshDistributed::host_processes has not been tested for vertices.");

  // Mesh topology dim
  const uint D = mesh.topology().dim();

  // Check that entity is a vertex or a cell
  if (dim != 0 && dim != D)
  {
    dolfin_error("MeshDistributed.cpp",
                 "compute off-process indices",
                 "This version of MeshDistributed::host_processes is only for vertices or cells");
  }

  // Check that global numbers have been computed.
  if (!mesh.parallel_data().have_global_entity_indices(dim))
  {
    dolfin_error("MeshDistributed.cpp",
                 "compute off-process indices",
                 "Global mesh entity numbers have not been computed");
  }

  // Check that global numbers have been computed.
  if (!mesh.parallel_data().have_global_entity_indices(D))
  {
    dolfin_error("MeshDistributed.cpp",
                 "compute off-process indices",
                 "Global mesh entity numbers have not been computed");
  }

  // Get global cell entity indices on this process
  const std::vector<uint> global_entity_indices
      = mesh.parallel_data().global_entity_indices_as_vector(dim);

  dolfin_assert(global_entity_indices.size() == mesh.num_cells());

  // Prepare map to hold process numbers
  std::map<uint, std::set<std::pair<uint, uint> > > processes;

  // FIXME: work on optimising below code

  // List of indices to send
  std::vector<uint> my_entities = entity_indices;

  // Remove local cells from my_entities to reduce communication
  if (dim == D)
  {
    std::vector<uint>::iterator it;
    for (uint i = 0; i < global_entity_indices.size(); ++i)
    {
      const uint global_index = global_entity_indices[i];
      it = std::find(my_entities.begin(), my_entities.end(), global_index);
      if (it != my_entities.end())
        my_entities.erase(it);
    }
  }

  /*
  std::vector<uint> my_entities;
  if (dim == D)
  {
    for (uint i = 0; i < entity_indices.size(); ++i)
    {
      const uint global_index = entity_indices[i];
      std::vector<uint>::const_iterator it;
      it = std::find(global_entity_indices.begin(), global_entity_indices.end(), global_index);
      if (it == global_entity_indices.end())
        my_entities.push_back(global_index);
    }
  }
  else
    my_entities = entity_indices;
  */

  // FIXME: handle case when my_entities.size() == 0
  //dolfin_assert(my_entities.size() > 0);

  // Prepare data structures for send/receive
  const uint num_proc = MPI::num_processes();
  const uint proc_num = MPI::process_number();
  const uint max_recv = MPI::max(my_entities.size());
  std::vector<uint> off_process_entities(max_recv);

  // Send and receive data
  for (uint k = 1; k < MPI::num_processes(); ++k)
  {
    const uint src  = (proc_num - k + num_proc) % num_proc;
    const uint dest = (proc_num + k) % num_proc;

    // Send/receive list of non-owned entities (global indices)
    //const uint recv_enitity_count = MPI::send_recv(&my_entities[0],
    //                                    my_entities.size(), dest,
    //                                    &off_process_entities[0],
    //                                    off_process_entities.size(), src);

    MPI::send_recv(my_entities, dest, off_process_entities, src);

    const uint recv_enitity_count = off_process_entities.size();

    // Check if this process owns received entities, and if so
    // store local index
    std::vector<uint> my_hosted_entities;
    for (uint j = 0; j < recv_enitity_count; ++j)
    {
      // Check if this process hosts 'received_entity'
      const uint received_entity = off_process_entities[j];
      std::vector<uint>::const_iterator it;
      it = std::find(global_entity_indices.begin(), global_entity_indices.end(), received_entity);
      if (it != global_entity_indices.end())
      {
        const uint local_index = std::distance(global_entity_indices.begin(), it);
        my_hosted_entities.push_back(received_entity);
        my_hosted_entities.push_back(local_index);
      }
    }

    // Send/receive hosted cells
    const uint max_recv_host_proc = MPI::max(my_hosted_entities.size());
    std::vector<uint> host_processes(max_recv_host_proc);
    MPI::send_recv(my_hosted_entities, src, host_processes, dest);

    const uint recv_hostproc_count = host_processes.size();
    /*
    const uint recv_hostproc_count = MPI::send_recv(&my_hosted_entities[0],
                                            my_hosted_entities.size(), src,
                                            &host_processes[0],
                                            host_processes.size(), dest);
    */

    for (uint j = 0; j < recv_hostproc_count; j += 2)
    {
      const uint global_index = host_processes[j];
      const uint local_index  = host_processes[j + 1];
      processes[global_index].insert(std::make_pair(dest, local_index));
    }

    // FIXME: Do later for efficiency
    // Remove entries from entities (from my_entities) that cannot
    // reside on more processes (i.e., cells)
  }

  // Sanity check
  const std::set<uint> test_set(my_entities.begin(), my_entities.end());
  const uint number_expected = test_set.size();
  if (number_expected != processes.size())
  {
    dolfin_error("MeshDistributed.cpp",
                 "compute off-process indices",
                 "Sanity check failed");
  }

  return processes;
}
//-----------------------------------------------------------------------------
