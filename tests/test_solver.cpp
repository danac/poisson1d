/*
 * test_solver.cpp
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
#include "p1d_assembler.hpp"
#include <cassert>
#include <iostream>
#include <cmath>
#include <limits>

using namespace poisson1d;
using namespace std;


void dump_matrix_ptr(const Real* matrix_ptr, size_t n)
{
    int count(0);
    cout << " Row " << count << ": ";
    cout << matrix_ptr[0] << " ";
    cout << matrix_ptr[1] << endl;
    ++count;

    for (size_t i(2); i < 3*n-4; i+=3)
    {
        cout << " Row " << count << ": ";
        cout << matrix_ptr[i] << " ";
        cout << matrix_ptr[i+1] << " ";
        cout << matrix_ptr[i+2] << endl;
        ++count;
    }

    cout << " Row " << count << ": ";
    cout << matrix_ptr[3*n-4] << " ";
    cout << matrix_ptr[3*n-3] << endl;
}

void dump_vector_ptr(const Real* vector_ptr, size_t n)
{
    for (size_t i(0); i < n; ++i)
    {
        cout << " " << vector_ptr[i];
    }
    cout << endl;
}

int main(int argc, char* argv[])
{

    size_t n(3);
    Real a(0), b(1);
    Real dx((b-a)/n);
    Real ref_value[3] = {1./2, 2./3, 1./2};

    // n-by-n 1D poisson matrix has n-2 non-zeros
    Real* matrix_ptr = new Real[3*n-2];
    Real* rhs_ptr = new Real[n];
    Real* x_ptr = new Real[n];

    DistributedAssembler assembler(a, b, n, "1");
    assembler.assemble_rhs(rhs_ptr);
    assembler.assemble_matrix(matrix_ptr);

    cout << "Matrix: " << endl;
    dump_matrix_ptr(matrix_ptr, n);

    cout << "RHS: " << endl;
    dump_vector_ptr(rhs_ptr, n);

    Solver solver(matrix_ptr, rhs_ptr, n);
    solver.solve(x_ptr);

    cout << "Solution vector: " << endl;
    for(size_t i(0); i<n; ++i)
    {
        cout << " " << x_ptr[i] << " (reference is " << ref_value[i] << ")" << endl;
        assert(std::abs(x_ptr[i] - ref_value[i]) < std::numeric_limits<Real>::epsilon());
    }
    cout << endl;

    delete[] matrix_ptr;
    delete[] rhs_ptr;
    delete[] x_ptr;

    return 0;
}

