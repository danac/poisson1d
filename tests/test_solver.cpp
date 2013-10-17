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
#include <cstdio>
#include <cmath>
#include <limits>

using namespace poisson1d;
using namespace std;


void dump_matrix_ptr(const Real* matrix_ptr, size_t n)
{
    size_t count(0);
    size_t m = 3*(n-2)+2;

    cout << " Row " << count << ": " << matrix_ptr[0] << endl;
    ++count;

    for (size_t i(1); i < m-1; i+=3)
    {
        cout << " Row " << count << ": ";
        cout << matrix_ptr[i] << " ";
        cout << matrix_ptr[i+1] << " ";
        cout << matrix_ptr[i+2] << endl;
        ++count;
    }
    cout << " Row " << count << ": " << matrix_ptr[m-1] << endl;
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
    Real fa(10), fb(2);
    Real ref_value[3] = {10., 25./4, 2.};
    Real epsilon = std::numeric_limits<Real>::epsilon();

    // n-by-n 1D poisson matrix has n-2 non-zeros
    Real* matrix_ptr = new Real[3*n-2];
    Real* rhs_ptr = new Real[n];
    Real* x_ptr = new Real[n];

    Mesh mesh(a, b, n, _full);
    DistributedAssembler assembler(mesh, "1", fa, fb);

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
        Real diff = std::abs(x_ptr[i] - ref_value[i]);
        printf(" %.8f (error %.8e)\n", x_ptr[i], diff);
        assert(diff < epsilon);
    }
    cout << endl;

    delete[] matrix_ptr;
    delete[] rhs_ptr;
    delete[] x_ptr;

    return 0;
}

