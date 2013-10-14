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

DistributedAssembler::DistributedAssembler(Real a_, Real b_, UInt n_, std::string rhs)
:a(a_), b(b_), n(n_), rhs_func(rhs)
{
    rhs_ptr = new Real[n];
    Real dx = (b-a)/n;

    for(UInt i(0); i < n; ++i)
    {
        rhs_ptr[i] = a+i*dx;
    }

    matrix_ptr = new Real[n*3];
}


DistributedAssembler::~DistributedAssembler()
{
    delete[] rhs_ptr;
    delete[] matrix_ptr;
}


void DistributedAssembler::assemble_rhs()
{
    //Initialize the expression parser
    mu::Parser p;
    Real x;
    p.DefineVar("x", &x);
    p.SetExpr(rhs_func);

    Real dx = (b-a)/n;

    for (std::size_t i(0); i < n; ++i)
    {
        x = rhs_ptr[i];
        rhs_ptr[i] = p.Eval();
    }
}

const Real* DistributedAssembler::get_rhs_ptr() const
{
    return rhs_ptr;
}

void DistributedAssembler::assemble_matrix()
{
    Real dx = (b-a)/n;
    Real coef1 = -1/dx;
    Real coef2 = 2/dx;

    //Very simple "(-1)-2-(-1)-style matrix for now
    for (std::size_t i(0); i < 3*n; i+=3)
    {
        matrix_ptr[i] = coef1;
        matrix_ptr[i+1] = coef2;
        matrix_ptr[i+2] = coef1;
    }
}

const Real* DistributedAssembler::get_matrix_ptr() const
{
    return matrix_ptr;
}

} //namespace poisson1d
