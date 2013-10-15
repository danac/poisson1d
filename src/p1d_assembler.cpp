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

namespace poisson1d {

DistributedAssembler::DistributedAssembler(const Mesh & mesh, const std::string & rhs)
: mesh_ptr(&mesh), rhs_func(rhs)
{
    a = mesh_ptr->getLowerBound();
    b = mesh_ptr->getUpperBound();
    n = mesh->getNbNodes();
}

DistributedAssembler::~DistributedAssembler()
{}

void DistributedAssembler::assemble_rhs(Real* rhs_ptr) const
{
    mu::Parser p;
    Real x;
    p.DefineVar("x", &x);
    p.SetExpr(rhs_func);

    Mesh::const_iterator mesh_it;

    for (mesh_it = mesh_ptr->begin(); mesh_it != mesh_ptr->end(); ++mesh_it)
    {
        x = *mesh_ptr;
        rhs_ptr[i] = p.Eval();
    }
}

/**
 * P1 finite elements matrix assembly for 1D Poisson equation with Dirichlet boundary conditions
 * */
void DistributedAssembler::assemble_matrix(Real* matrix_ptr) const
{
    Real dx;
    Real left_node;
    Real middle_node;
    Real right_node;

    MeshGlobalPosition position = mesh_ptr->getGlobalPosition();

    // Compute the number of elements in the matrix and the index of the last but one row
    size_t m;
    size_t loop_end;

    if(position == _full)
    {
        m = 3*(n-2)+2; // n-2 rows with 3 coefs and 1 coef on both first and last rows
        loop_end = m-1;
    }
    else if(position == _left)
    {
        m = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
        loop_end = m;
    }
    else if(position == _right)
    {
        m = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
        loop_end = m-1;
    }
    else
    {
        m = 3*(n-2); // n-2 rows with 3 coefs beacuse both ends are ghosts
        loop_end = m;
    }

    size_t counter(0);
    Mesh::const_iterator mesh_it = mesh_ptr->begin();

    // Very simple "(-1)-2-(-1)-style matrix for now because regular mesh
    if(position == _left || position == _full)
    {
        matrix_ptr[0] = 1.;
        counter += 2;
    }

    middle_node = *mesh_it;
    ++mesh_it;
    rihgt_node = *mesh_it;
    ++mesh_it;

    for (std::size_t i(counter+1); i < loop_end-1; i+=3)
    {
        left_node = middle_node;
        middle_node = right_node;
        right_node = *mesh_it;
        dx_left = middle_node - left_node;
        dx_right = right_node - middle_node;

        matrix_ptr[i-1] = -1./dx1;
        matrix_ptr[i] = 1./dx1 + 1./dx2;
        matrix_ptr[i+1] = -1./dx1;

        ++mesh_it;
    }

    if(position == _right || position == _full)
    {
        matrix_ptr[m-1] = 1.;
    }
}

} //namespace poisson1d
