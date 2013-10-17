/*
 * p1d_assembler.cpp
 *
 * Copyright 2013 Dana Christen <dana.christen@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include "p1d_assembler.hpp"
#include "muParser.h"
#include <cassert>

namespace poisson1d {

DistributedAssembler::DistributedAssembler(const Mesh & mesh, const std::string & rhs, Real fa_, Real fb_)
: mesh_ptr(&mesh),
  rhs_func(rhs),
  a(mesh.get_lower_bound()),
  b(mesh.get_upper_bound()),
  fa(fa_),
  fb(fb_),
  n(mesh.get_num_nodes()),
  nnz(0),
  rhs_size(0)
{
    MeshGlobalPosition position = mesh_ptr->get_global_position();
    assert(position != _undefined);

    if(position == _full)
    {
        nnz = 3*(n-2)+2; // n-2 rows with 3 coefs and 1 coef on both first and last rows
        rhs_size = n;
    }
    else if(position == _left)
    {
        nnz = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
        rhs_size = n-1;
    }
    else if(position == _right)
    {
        nnz = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
        rhs_size = n-1;
    }
    else
    {
        nnz = 3*(n-2); // n-2 rows with 3 coefs beacuse both ends are ghosts
        rhs_size = n-2;
    }
}

DistributedAssembler::~DistributedAssembler()
{}

void DistributedAssembler::assemble_rhs(Real* rhs_ptr) const
{
    MeshGlobalPosition position = mesh_ptr->get_global_position();

    size_t i(0);
    size_t rhs_size(0);

    if(position == _left)
    {
        rhs_size = n-1;
        rhs_ptr[0] = fa;
        ++i;
    }
    else if(position == _right)
    {
        rhs_size = n-1;
        rhs_ptr[rhs_size-1] = fb;
    }
    else if(position == _full)
    {
        rhs_size = n;
        rhs_ptr[0] = fa;
        ++i;
        rhs_ptr[rhs_size-1] = fb;
    }
    else
    {
        rhs_size = n-2;
    }

    mu::Parser p;
    Real x;
    p.DefineVar("x", &x);
    p.SetExpr(rhs_func);

    Mesh::const_iterator mesh_it = mesh_ptr->begin();
    Mesh::const_iterator mesh_end = mesh_ptr->end();

    // Skip nodes at both ends since they are either ghosts or enforced boundary values
    --mesh_end;
    ++mesh_it;

    for (; mesh_it != mesh_end; ++mesh_it, ++i)
    {
        x = *mesh_it;
        rhs_ptr[i] = p.Eval();
    }
}

Real* DistributedAssembler::assemble_rhs_alloc() const
{
    Real* rhs = new Real[rhs_size];
    assemble_rhs(rhs);
    return rhs;
}

/**
 * P1 finite elements matrix assembly for 1D Poisson equation with Dirichlet boundary conditions
 * */
void DistributedAssembler::assemble_matrix(Real* matrix_ptr) const
{
    MeshGlobalPosition position = mesh_ptr->get_global_position();

    size_t loop_end(0);

    if(position == _full)
    {
        loop_end = nnz-1;
    }
    else if(position == _right)
    {
        loop_end = nnz-1;
    }
    else
    {
        loop_end = nnz;
    }

    size_t counter(0);
    Mesh::const_iterator mesh_it = mesh_ptr->begin();

    if(position == _left || position == _full)
    {
        matrix_ptr[0] = 1.;
        counter += 1;
    }

    Real dx_left(0.);
    Real dx_right(0.);
    Real left_node(0.);
    Real middle_node(0.);
    Real right_node(0.);

    middle_node = *mesh_it;
    ++mesh_it;
    right_node = *mesh_it;
    ++mesh_it;

    for (std::size_t i(counter+1); i < loop_end-1; i+=3)
    {
        left_node = middle_node;
        middle_node = right_node;
        right_node = *mesh_it;
        dx_left = middle_node - left_node;
        dx_right = right_node - middle_node;

        matrix_ptr[i-1] = -1./dx_left;
        matrix_ptr[i] = 1./dx_left + 1./dx_right;
        matrix_ptr[i+1] = -1./dx_left;

        ++mesh_it;
    }

    if(position == _right || position == _full)
    {
        matrix_ptr[nnz-1] = 1.;
    }
}


Real* DistributedAssembler::assemble_matrix_alloc() const
{
    Real* matrix = new Real[nnz];
    assemble_matrix(matrix);
    return matrix;
}

size_t DistributedAssembler::get_matrix_nnz() const
{
    return nnz;
}

size_t DistributedAssembler::get_rhs_size() const
{
    return rhs_size;
}

} //namespace poisson1d
