/*
 * p1d_solver.hpp
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

#ifndef P1D_SOLVER
#define P1D_SOLVER

#include "p1d_common.hpp"
#include "p1d_structs.hpp"
#include <Eigen/SparseCore>

namespace poisson1d {

class Solver
{
    public:
        Solver(const Real* A, const Real* b, size_t n);
        ~Solver();

        Solution* get_solution_alloc();

    private:
        typedef Eigen::SparseMatrix<Real, Eigen::RowMajor> SparseMat;
        typedef Eigen::Matrix<Real, Eigen::Dynamic, 1> Vec;

        void load_matrix_array(const Real* matrix_ptr);
        void load_rhs_array(const Real* rhs_ptr);

        void solve(Real* x_ptr);

        SparseMat A;
        Vec b;
        size_t n;
};

} //namespace poisson1d

#endif //P1D_SOLVER
