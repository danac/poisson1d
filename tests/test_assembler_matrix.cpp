/*
 * test_assembler.cpp
 *
 * Copyright 2013 Dana Christen <dana.christen@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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
#include <cassert>
#include <iostream>
#include <limits>
#include <cmath>
#include <cstdio>

using namespace poisson1d;
using namespace std;

void test_matrix_5_mesh(MeshGlobalPosition position)
{
    Real a(0.0);
    Real b(1.0);
    Real fa(10);
    Real fb(2);
    size_t n(5);
    string rhs_func("x");
    size_t num_jobs(1);
    size_t rank(0);
    Real dx = (b-a)/(n-1);
    size_t matrix_loop_end(0);
    size_t m(0);

    Real epsilon = std::numeric_limits<Real>::epsilon() * 100;
    printf("Admitted epsilon: %.2e\n", epsilon);

    if(position == _full)
    {
        m = 3*(n-2)+2; // n-2 rows with 3 coefs and 1 coef on both first and last rows
        matrix_loop_end = m-1;
    }
    else if(position == _left)
    {
        m = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
        matrix_loop_end = m;
    }
    else if(position == _right)
    {
        m = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
        matrix_loop_end = m-1;
    }
    else
    {
        m = 3*(n-2); // n-2 rows with 3 coefs beacuse both ends are ghosts
        matrix_loop_end = m;
    }

    Mesh mesh(a, b, n, position);
    Problem problem(mesh, fa, fb, rhs_func, num_jobs);
    Job job(problem, rank);
    DistributedAssembler assembler(job);

    JobResult* job_result = assembler.get_job_result_alloc();
    cout << "Testing matrix..." << endl;
    Real* matrix_ptr = job_result->get_matrix_ptr();

    Real expected_coef1 = -1./dx;
    Real expected_coef2 = 2./dx;
    Real diff;
    size_t count(0);

    if(position == _full || position == _left)
    {
        diff = std::abs(matrix_ptr[0] - 1.0);
        printf(" Row %lu: %.18f (error %.2e)\n", count, matrix_ptr[0], diff);
        assert(std::abs(matrix_ptr[0] - 1.0) <= epsilon);
        ++count;
    }

    for (size_t i(count); i < matrix_loop_end; i+=3)
    {
        diff = std::abs(matrix_ptr[i] - expected_coef1);
        printf(" Row %lu: %.18f (error %.2e) ", count, matrix_ptr[i], diff);
        assert(diff <= epsilon);

        diff = std::abs(matrix_ptr[i+1] - expected_coef2);
        printf("| %.18f (error %.2e) ", matrix_ptr[i+1], diff);
        assert(diff <= epsilon);

        diff = std::abs(matrix_ptr[i+2] - expected_coef1);
        printf("| %.18f (error %.2e)\n", matrix_ptr[i+2], diff);
        assert(diff <= epsilon);

        ++count;
    }

    if(position == _right || position == _full)
    {
        diff = std::abs(matrix_ptr[m-1] - 1.);
        printf(" Row %lu: %.18f (error %.2e)\n", count, matrix_ptr[m-1], diff);
        assert(diff <= epsilon);
    }

    delete job_result;
}

int main(int argc, char* argv[])
{
    test_matrix_5_mesh(_left);
    test_matrix_5_mesh(_middle);
    test_matrix_5_mesh(_right);
    test_matrix_5_mesh(_full);

    return 0;
}
