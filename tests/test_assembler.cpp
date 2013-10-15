/*
 * test_assembler.cpp
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
#include <cassert>
#include <iostream>
#include <limits>
#include <cmath>
#include <cstdio>

using namespace poisson1d;
using namespace std;

int main(int argc, char* argv[])
{
    Real a(0.0);
    Real b(1.0);
    Real fa(10);
    Real fb(2);
    size_t n(10);
    string rhs_func("-x*x+1");
    Real dx = (b-a)/n;
    MeshGlobalPosition position = _right;
    size_t rhs_clip(0);
    size_t matrix_loop_end(0);
    size_t m(0);

    Real epsilon = std::numeric_limits<Real>::epsilon() * 100;
    printf("Admitted epsilon: %.2e\n", epsilon);

    if(position == _full)
    {
        m = 3*(n-2)+2; // n-2 rows with 3 coefs and 1 coef on both first and last rows
        matrix_loop_end = m-1;
        rhs_clip = 1;
    }
    else if(position == _left)
    {
        m = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
        matrix_loop_end = m;
        rhs_clip = 1;
    }
    else if(position == _right)
    {
        m = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
        matrix_loop_end = m-1;
        rhs_clip = 2;
    }
    else
    {
        m = 3*(n-2); // n-2 rows with 3 coefs beacuse both ends are ghosts
        matrix_loop_end = m;
        rhs_clip = 2;
    }

    Mesh mesh(a, b, n, position);

    DistributedAssembler assembler(mesh, rhs_func, fa, fb);

    Real* rhs_ptr = new Real[n];
    Real* matrix_ptr = new Real[m];

    cout << "Testing rhs..." << endl;
    assembler.assemble_rhs(rhs_ptr);

    Real expected_value(0);
    size_t start(0);
    if(position == _full || position == _left)
    {
        expected_value = fa;
        Real diff = std::abs(rhs_ptr[0] - expected_value);
        printf(" rhs_ptr[0]=%.2f error %.2e\n", rhs_ptr[0], diff);
        ++start;
    }

    for (size_t i(start); i < n-rhs_clip; ++i)
    {
        Real offset(0);
        if(position == _middle || position == _right)
        {
            offset=1;
        }
        expected_value = -(a+(i+offset)*dx)*(a+(i+offset)*dx)+1;
        Real diff = std::abs(rhs_ptr[i] - expected_value);
        printf(" rhs_ptr[%lu]=%.2f exp %.2f error %.2e\n", i, rhs_ptr[i], expected_value, diff);
        assert(diff <= epsilon);
    }

    if(position == _full || position == _right)
    {
        expected_value = fb;
        size_t last_index = n-rhs_clip;
        Real diff = std::abs(rhs_ptr[last_index] - expected_value);
        printf(" rhs_ptr[%lu]=%.2f error %.2e\n", last_index, rhs_ptr[last_index], diff);
    }

    cout << "Testing matrix..." << endl;
    assembler.assemble_matrix(matrix_ptr);

    Real expected_coef1 = -1./dx;
    Real expected_coef2 = 2./dx;
    Real diff;
    size_t count(0);

    if(position == _full || position == _left)
    {
        diff = std::abs(matrix_ptr[0] - 1.0);
        printf(" Row %lu: %.2f (error %.2e)\n", count, matrix_ptr[0], diff);
        assert(std::abs(matrix_ptr[0] - 1.0) <= epsilon);
        ++count;
    }

    for (size_t i(count); i < matrix_loop_end; i+=3)
    {
        diff = std::abs(matrix_ptr[i] - expected_coef1);
        printf(" Row %lu: %.2f (error %.2e) ", count, matrix_ptr[i], diff);
        assert(diff <= epsilon);

        diff = std::abs(matrix_ptr[i+1] - expected_coef2);
        printf("| %.2f (error %.2e) ", matrix_ptr[i+1], diff);
        assert(diff <= epsilon);

        diff = std::abs(matrix_ptr[i+2] - expected_coef1);
        printf("| %.2f (error %.2e)\n", matrix_ptr[i+2], diff);
        assert(diff <= epsilon);

        ++count;
    }

    if(position == _right || position == _full)
    {
        diff = std::abs(matrix_ptr[m-1] - 1.);
        printf(" Row %lu: %.2f (error %.2e)\n", count, matrix_ptr[m-1], diff);
        assert(diff <= epsilon);
    }

    delete[] rhs_ptr;
    delete[] matrix_ptr;

    return 0;
}
