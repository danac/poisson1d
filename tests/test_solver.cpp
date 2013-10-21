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
    std::string rhs_func("1");
    size_t num_jobs(1);
    size_t rank(0);

    Mesh mesh(a, b, n, _full);
    Problem problem(mesh, fa, fb, rhs_func, num_jobs);
    Job job(problem, rank);
    DistributedAssembler assembler(job);

    JobResult* job_result = assembler.get_job_result_alloc();
    Real* matrix_ptr = job_result->get_matrix_ptr();
    Real* rhs_ptr = job_result->get_rhs_ptr();

    cout << "Matrix: " << endl;
    dump_matrix_ptr(matrix_ptr, n);

    cout << "RHS: " << endl;
    dump_vector_ptr(rhs_ptr, n);

    Solver solver(matrix_ptr, rhs_ptr, n);
    Solution* solution = solver.get_solution_alloc();
    const Real* x_ptr = solution->get_x_ptr();
    cout << "Solution vector: " << endl;
    for(size_t i(0); i<n; ++i)
    {
        Real diff = std::abs(x_ptr[i] - ref_value[i]);
        printf(" %.8f (error %.8e)\n", x_ptr[i], diff);
        assert(diff < epsilon);
    }
    cout << endl;

    delete solution;
    delete job_result;

    return 0;
}

