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

namespace poisson1d {

Solver::Solver(const Real* A_, const Real* b_)
: A(A_), b(b_)
{}

Solver::~Solver()
{}

void Solver::solve(Real* x) const
{
    //Eigen::ConjugateGradient<EigenSpMat, Eigen::Upper> solver;
    //solver.compute(*A);
    //assert(solver.info() == Eigen::Success);
    //x = solver.solve(*b);
    //assert(solver.info() == Eigen::Success);
    //return x;
}

} //namespace poisson1d
