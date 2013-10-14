/*
 * p1d_solver.cpp
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

#include "p1d_solver.hpp"
#include <cassert>
#include <Eigen/IterativeLinearSolvers>
#include <iostream>

namespace poisson1d {

Solver::Solver(const Real* matrix_ptr, const Real* rhs_ptr, size_t n_)
: A(n_,n_), b(n_), n(n_)
{
    load_matrix_array(matrix_ptr);
    load_rhs_array(rhs_ptr);
}

Solver::~Solver()
{}

void Solver::load_matrix_array(const Real* matrix_ptr)
{
    A.reserve(Eigen::VectorXi::Constant(n,3));
    A.insert(0,0) = matrix_ptr[0];
    A.insert(0,1) = matrix_ptr[1];
    for(std::size_t i(1); i < n-1; ++i)
    {
        A.insert(i,i-1) = matrix_ptr[i*3-1];
        A.insert(i,i) = matrix_ptr[i*3];
        A.insert(i,i+1) = matrix_ptr[i*3+1];
    }
    A.insert(n-1,n-2) = matrix_ptr[(n-1)*3-1];
    A.insert(n-1,n-1) = matrix_ptr[(n-1)*3];
    A.makeCompressed();
}

void Solver::load_rhs_array(const Real* rhs_ptr)
{
    for(std::size_t i(0); i < n; ++i)
    {
        b(i) = rhs_ptr[i];
    }
}

void Solver::solve(Real* x_ptr)
{
    Vec x(n);
    Eigen::ConjugateGradient<SparseMat> solver;
    solver.setMaxIterations(n*10);

    solver.compute(A);
    assert(solver.info() == Eigen::Success);

    x = solver.solve(b);
    //std::cout << "#iterations:     " << solver.iterations() << std::endl;
    //std::cout << "estimated error: " << solver.error()      << std::endl;
    //std::cout << solver.info() <<  " " << Eigen::Success;
    assert(solver.info() == Eigen::Success);

    for(std::size_t i(0); i < n; ++i)
    {
        x_ptr[i] = x(i);
    }
}

} //namespace poisson1d
