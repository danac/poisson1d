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
#include "p1d_helper_functions.hpp"
#include "muParser.h"
#include <cassert>

namespace poisson1d {

DistributedAssembler::DistributedAssembler(const Job & job)
: job_ptr(&job),
  mesh_ptr(NULL),
  rhs_func(""),
  a(0.0),
  b(0.0),
  fa(0.0),
  fb(0.0),
  n(0),
  nnz(0),
  rhs_size(0)
{
    const Problem* problem_ptr = &job_ptr->get_problem();
    mesh_ptr = &problem_ptr->get_mesh();
    a = mesh_ptr->get_a();
    b = mesh_ptr->get_b();
    n = mesh_ptr->get_n();
    fa = problem_ptr->get_fa();
    fb = problem_ptr->get_fb();
    rhs_func = problem_ptr->get_rhs_func();
    MeshGlobalPosition position = mesh_ptr->get_global_position();
    assert(position != _undefined);

    nnz = utils::get_matrix_nnz(n, position);

    if(position == _full)
    {
        rhs_size = n;
    }
    else if(position == _left)
    {
        rhs_size = n-1;
    }
    else if(position == _right)
    {
        rhs_size = n-1;
    }
    else
    {
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

JobResult* DistributedAssembler::get_job_result_alloc(size_t rank)
{
    Real* result_rhs_ptr = assemble_rhs_alloc();
    Real* result_matrix_ptr = assemble_matrix_alloc();
    return new JobResult(result_matrix_ptr, result_rhs_ptr, rank, nnz, n, true);
}

} //namespace poisson1d
