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

DistributedAssembler::DistributedAssembler(Real a_, Real b_, size_t n_, std::string rhs)
:a(a_), b(b_), n(n_), rhs_func(rhs)
{
    //rhs_ptr = new Real[n];
    //Real dx = (b-a)/n;

    //for(UInt i(0); i < n; ++i)
    //{
        //rhs_ptr[i] = a+i*dx;
    //}

    //matrix_ptr = new Real[n*3];
}


DistributedAssembler::~DistributedAssembler()
{
    //delete[] rhs_ptr;
    //delete[] matrix_ptr;
}


void DistributedAssembler::assemble_rhs(Real* rhs_ptr) const
{
    //Initialize the expression parser
    mu::Parser p;
    Real x;
    p.DefineVar("x", &x);
    p.SetExpr(rhs_func);

    Real dx = (b-a)/n;

    for (std::size_t i(0); i < n; ++i)
    {
        x = a+i*dx;
        rhs_ptr[i] = p.Eval();
    }
}

void DistributedAssembler::assemble_matrix(Real* matrix_ptr) const
{
    Real dx = (b-a)/n;
    Real coef1 = -1/dx;
    Real coef2 = 2/dx;

    // Very simple "(-1)-2-(-1)-style matrix for now because regular mesh
    matrix_ptr[0] = coef2;
    matrix_ptr[1] = coef1;
    matrix_ptr[3*n-4] = coef1;
    matrix_ptr[3*n-3] = coef2;

    for (std::size_t i(2); i < 3*n-4; i+=3)
    {
        matrix_ptr[i] = coef1;
        matrix_ptr[i+1] = coef2;
        matrix_ptr[i+2] = coef1;
    }
}

} //namespace poisson1d
