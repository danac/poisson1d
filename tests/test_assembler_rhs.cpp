/*
 * test_assembler_rhs.cpp
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

void test_rhs_5_mesh(MeshGlobalPosition position)
{
    Real a(0.0);
    Real b(1.0);
    Real fa(10);
    Real fb(2);
    size_t n(5);
    string rhs_func("x");
    Real dx = (b-a)/(n-1);

    Real epsilon = std::numeric_limits<Real>::epsilon() * 100;
    printf("Admitted epsilon: %.2e\n", epsilon);

    Mesh mesh(a, b, n, position);

    DistributedAssembler assembler(mesh, rhs_func, fa, fb);

    size_t rhs_size(0);
    size_t rhs_loop_start(0);
    size_t rhs_end_clip(0);

    if(position == _full)
    {
        rhs_size = n;
        rhs_loop_start = 1;
        rhs_end_clip = 1;
    }
    else if(position == _left)
    {
        rhs_size = n-1;
        rhs_loop_start = 1;
    }
    else if(position == _right)
    {
        rhs_size = n-1;
        rhs_end_clip = 1;
    }
    else
    {
        rhs_size = n-2;
    }

    Real* rhs_ptr = new Real[rhs_size];

    cout << "Testing rhs..." << endl;

    assembler.assemble_rhs(rhs_ptr);

    if(position == _full || position == _left)
    {
        Real expected_value = fa;
        Real diff = std::abs(rhs_ptr[0] - expected_value);
        printf(" rhs_ptr[%d]=%.2f expected %.2f diff %.2e \n", 0, rhs_ptr[0], expected_value, diff);
        assert(diff <= epsilon);
    }

    for (size_t i(1); i < rhs_size - rhs_end_clip; ++i)
    {
        Real expected_value = a+i*dx;
        size_t index = i-1+rhs_loop_start;
        Real diff = std::abs(rhs_ptr[index] - expected_value);
        printf(" rhs_ptr[%lu]=%.2f expected %.2f diff %.2e \n", index, rhs_ptr[index], expected_value, diff);
        assert(diff <= epsilon);
    }

    if(position == _full || position == _right)
    {
        size_t index = rhs_size-1;
        Real expected_value = fb;
        Real diff = std::abs(rhs_ptr[index] - expected_value);
        printf(" rhs_ptr[%lu]=%.2f expected %.2f diff %.2e \n", index, rhs_ptr[index], expected_value, diff);
        assert(diff <= epsilon);
    }
    delete[] rhs_ptr;


}

int main(int argc, char* argv[])
{
    test_rhs_5_mesh(_left);
    test_rhs_5_mesh(_middle);
    test_rhs_5_mesh(_right);
    test_rhs_5_mesh(_full);

    return 0;
}
