// Copyright (C) 2008 Anders Logg
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
// Modified by Garth N. Wells, 2010
//
// First added:  2008-07-16
// Last changed: 2011-03-17

//#include <dolfin/ale/ALE.h>
#include <dolfin/common/Array.h>
#include <dolfin/common/constants.h>
#include "Mesh.h"
#include "BoundaryMesh.h"
#include "Vertex.h"
#include "Cell.h"
#include "Facet.h"
#include "MeshData.h"
#include "SubDomain.h"
#include "MeshSmoothing.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
void MeshSmoothing::smooth(Mesh& mesh, uint num_iterations)
{
  log(PROGRESS, "Smoothing mesh");

  // Make sure we have cell-facet connectivity
  mesh.init(mesh.topology().dim(), mesh.topology().dim() - 1);

  // Make sure the mesh is ordered
  mesh.order();

  // Mark vertices on the boundary so we may skip them
  BoundaryMesh boundary(mesh);
  const MeshFunction<unsigned int> vertex_map = boundary.vertex_map();
  MeshFunction<bool> on_boundary(mesh, 0);
  on_boundary = false;
  if (boundary.num_vertices() > 0)
  {
    for (VertexIterator v(boundary); !v.end(); ++v)
      on_boundary[vertex_map[*v]] = true;
  }

  // Iterate over all vertices
  const uint d = mesh.geometry().dim();
  std::vector<double> xx(d);
  for (uint iteration = 0; iteration < num_iterations; iteration++)
  {
    for (VertexIterator v(mesh); !v.end(); ++v)
    {
      // Skip vertices on the boundary
      if (on_boundary[*v])
        continue;

      // Get coordinates of vertex
      double* x = mesh.geometry().x(v->index());
      const Point p = v->point();

      // Compute center of mass of neighboring vertices
      for (uint i = 0; i < d; i++) xx[i] = 0.0;
      uint num_neighbors = 0;
      for (VertexIterator vn(*v); !vn.end(); ++vn)
      {
        // Skip the vertex itself
        if (v->index() == vn->index())
          continue;
        num_neighbors += 1;

        // Compute center of mass
        const double* xn = vn->x();
        for (uint i = 0; i < d; i++)
          xx[i] += xn[i];
      }
      for (uint i = 0; i < d; i++)
        xx[i] /= static_cast<double>(num_neighbors);

      // Compute closest distance to boundary of star
      double rmin = 0.0;
      for (CellIterator c(*v); !c.end(); ++c)
      {
        // Get local number of vertex relative to facet
        const uint local_vertex = c->index(*v);

        // Get normal of corresponding facet
        Point n = c->normal(local_vertex);

        // Get first vertex in facet
        Facet f(mesh, c->entities(mesh.topology().dim() - 1)[local_vertex]);
        VertexIterator fv(f);

        // Compute length of projection of v - fv onto normal
        const double r = std::abs(n.dot(p - fv->point()));
        if (rmin == 0.0)
          rmin = r;
        else
          rmin = std::min(rmin, r);
      }

      // Move vertex at most a distance rmin / 2
      double r = 0.0;
      for (uint i = 0; i < d; i++)
      {
        const double dx = xx[i] - x[i];
        r += dx*dx;
      }
      r = std::sqrt(r);
      if (r < DOLFIN_EPS)
        continue;
      rmin = std::min(0.5*rmin, r);
      for (uint i = 0; i < d; i++)
        x[i] += rmin*(xx[i] - x[i])/r;
    }
  }

  if (num_iterations > 1)
    log(PROGRESS, "Mesh smoothing repeated %d times.", num_iterations);
}
//-----------------------------------------------------------------------------
void MeshSmoothing::smooth_boundary(Mesh& mesh,
                                    uint num_iterations,
                                    bool harmonic_smoothing)
{
  cout << "Smoothing boundary of mesh: " << mesh << endl;

  // Extract boundary of mesh
  BoundaryMesh boundary(mesh);

  // Smooth boundary
  smooth(boundary, num_iterations);

  // Move interior vertices
  move_interior_vertices(mesh, boundary, harmonic_smoothing);
}
//-----------------------------------------------------------------------------
void MeshSmoothing::snap_boundary(Mesh& mesh,
                                  const SubDomain& sub_domain,
                                  bool harmonic_smoothing)
{
  cout << "Snapping boundary of mesh: " << mesh << endl;

  // Extract boundary of mesh
  BoundaryMesh boundary(mesh);

  const uint dim = mesh.geometry().dim();
  Array<double> x;

  // Smooth boundary
  MeshGeometry& geometry = boundary.geometry();
  for (uint i = 0; i < boundary.num_vertices(); i++)
  {
    x.update(dim, geometry.x(i));
    sub_domain.snap(x);
  }

  // Move interior vertices
  move_interior_vertices(mesh, boundary, harmonic_smoothing);
}
//-----------------------------------------------------------------------------
void MeshSmoothing::move_interior_vertices(Mesh& mesh,
                                           BoundaryMesh& boundary,
                                           bool harmonic_smoothing)
{
  // Select smoothing of interior vertices
  if (harmonic_smoothing)
      ;//    ALE::move(mesh, boundary);
  else
  {
    // Use vertex map to update boundary coordinates of original mesh
    const MeshFunction<unsigned int>& vertex_map = boundary.vertex_map();
    const uint d = mesh.geometry().dim();
    for (VertexIterator v(boundary); !v.end(); ++v)
    {
      const double* xb = v->x();
      double* xm = mesh.geometry().x(vertex_map[*v]);
      for (uint i = 0; i < d; i++)
        xm[i] = xb[i];
    }
  }
}
//-----------------------------------------------------------------------------
