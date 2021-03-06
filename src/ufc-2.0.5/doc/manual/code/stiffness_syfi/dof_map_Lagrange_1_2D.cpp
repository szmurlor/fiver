//
// This code complies with UFC version 1.0, and is generated with SyFi version 0.4.0.
//
// http://www.fenics.org/syfi/
// http://www.fenics.org/ufc/
//


#include <stdexcept>
#include <math.h>
#include <ufc.h>
#include <pycc/Functions/Ptv.h>
#include <pycc/Functions/Ptv_tools.h>
#include <pycc/Functions/Dof_Ptv.h>
#include <pycc/Functions/OrderedPtvSet.h>
#include <pycc/Functions/Dof_OrderedPtvSet.h>
#include "dof_map_Lagrange_1_2D.h"


namespace pycc
{

/// Constructor
dof_map_Lagrange_1_2D::dof_map_Lagrange_1_2D() : ufc::dof_map()
{
  loc2glob = 0;
}

/// Destructor
dof_map_Lagrange_1_2D::~dof_map_Lagrange_1_2D()
{
  if(loc2glob) delete [] loc2glob;
}

/// Return a string identifying the dof map
const char* dof_map_Lagrange_1_2D::signature() const
{
  return "dof_map_Lagrange_1_2D // generated by SyFi";
}

/// Return true iff mesh entities of topological dimension d are needed
bool dof_map_Lagrange_1_2D::needs_mesh_entities(unsigned int d) const
{
  switch(d)
  {
  case 0: return true; // vertices
  case 1: return true; // edges
  case 2: return true; // faces
  case 3: return false; // volumes
  }
  return false; // strange unsupported case or error
}

/// Initialize dof map for mesh (return true iff init_cell() is needed)
bool dof_map_Lagrange_1_2D::init_mesh(const ufc::mesh& m)
{
  int top_dim = 2;
  num_elements = m.num_entities[top_dim];
  return true;
}

/// Initialize dof map for given cell
void dof_map_Lagrange_1_2D::init_cell(const ufc::mesh& m,
                              const ufc::cell& c)
{
  // coordinates
  double x0 = c.coordinates[0][0]; double y0 = c.coordinates[0][1];
  double x1 = c.coordinates[1][0]; double y1 = c.coordinates[1][1];
  double x2 = c.coordinates[2][0]; double y2 = c.coordinates[2][1];
  
  // affine map
  double G00 = x1 - x0;
  double G01 = x2 - x0;
  
  double G10 = y1 - y0;
  double G11 = y2 - y0;
  
  int element = c.entity_indices[2][0];
  
  double dof0[2] = { x0, y0 };
  Ptv pdof0(2, dof0);
  dof.insert_dof(element, 0, pdof0);
  
  double dof1[2] = { G00+x0, y0+G10 };
  Ptv pdof1(2, dof1);
  dof.insert_dof(element, 1, pdof1);
  
  double dof2[2] = { x0+G01, G11+y0 };
  Ptv pdof2(2, dof2);
  dof.insert_dof(element, 2, pdof2);
}

/// Finish initialization of dof map for cells
void dof_map_Lagrange_1_2D::init_cell_finalize()
{
  loc2glob = new unsigned int[num_elements * local_dimension()];
  dof.build_loc2dof(num_elements, local_dimension(), reinterpret_cast<int*>(loc2glob));
}

/// Return the dimension of the global finite element function space
unsigned int dof_map_Lagrange_1_2D::global_dimension() const
{
  return dof.size();
}

/// Return the dimension of the local finite element function space
unsigned int dof_map_Lagrange_1_2D::local_dimension() const
{
  return 3;
}

/// Return the number of dofs on each cell facet
unsigned int dof_map_Lagrange_1_2D::num_facet_dofs() const
{
  return 2;
}

/// Tabulate the local-to-global mapping of dofs on a cell
void dof_map_Lagrange_1_2D::tabulate_dofs(unsigned int* dofs,
                                  const ufc::mesh& m,
                                  const ufc::cell& c) const
{
  const unsigned int *from_dofs = loc2glob + (3 * c.entity_indices[2][0]);
  memcpy(dofs, from_dofs, sizeof(unsigned int)*3);
}

/// Tabulate the local-to-local mapping from facet dofs to cell dofs
void dof_map_Lagrange_1_2D::tabulate_facet_dofs(unsigned int* dofs,
                                        unsigned int facet) const
{
  switch(facet)
  {
  case 0:
    dofs[0] = 1;
    dofs[1] = 2;
    break;
  case 1:
    dofs[0] = 0;
    dofs[1] = 2;
    break;
  case 2:
    dofs[0] = 0;
    dofs[1] = 1;
    break;
  default:
    throw std::runtime_error("Invalid facet number.");
  }
}

/// Tabulate the coordinates of all dofs on a cell
void dof_map_Lagrange_1_2D::tabulate_coordinates(double** coordinates,
                                         const ufc::cell& c) const
{
  // coordinates
  double x0 = c.coordinates[0][0]; double y0 = c.coordinates[0][1];
  double x1 = c.coordinates[1][0]; double y1 = c.coordinates[1][1];
  double x2 = c.coordinates[2][0]; double y2 = c.coordinates[2][1];
  
  // affine map
  double G00 = x1 - x0;
  double G01 = x2 - x0;
  
  double G10 = y1 - y0;
  double G11 = y2 - y0;
  
  
  coordinates[0][0] = x0;
  coordinates[0][1] = y0;
  coordinates[1][0] = G00+x0;
  coordinates[1][1] = y0+G10;
  coordinates[2][0] = x0+G01;
  coordinates[2][1] = G11+y0;
}

/// Return the number of sub dof maps (for a mixed element)
unsigned int dof_map_Lagrange_1_2D::num_sub_dof_maps() const
{
  return 1;
}

/// Create a new dof_map for sub dof map i (for a mixed element)
ufc::dof_map* dof_map_Lagrange_1_2D::create_sub_dof_map(unsigned int i) const
{
  return new dof_map_Lagrange_1_2D();
}



} // namespace
