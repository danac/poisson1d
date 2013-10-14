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

using namespace poisson1d;
using namespace std;

int main(int argc, char* argv[])
{

    Real a(-1.0);
    Real b(1.0);
    size_t n(100);
    string rhs_func("-x*x+1");

    DistributedAssembler assembler(a, b, n, rhs_func);

    Real* rhs_ptr = new Real[n];
    Real* matrix_ptr = new Real[3*n-2];

    assembler.assemble_rhs(rhs_ptr);
    assembler.assemble_matrix(matrix_ptr);

    Real dx = (b-a)/n;

    cout << "Testing rhs..." << endl;
    for (size_t i(0); i < n; ++i)
    {
        Real expected_value = -(a+i*dx)*(a+i*dx)+1;
        cout << " rhs_ptr[" << i << "] = " << rhs_ptr[i] << " expected " << expected_value << endl;
        assert(rhs_ptr[i] == expected_value);
    }

    cout << "Testing matrix..." << endl;

    Real expected_coef1 = -1/dx;
    Real expected_coef2 = 2/dx;

    size_t count(0);

    cout << " Row " << count << ": ";
    assert(matrix_ptr[0] == expected_coef2);
    cout << matrix_ptr[0] << " ";

    assert(matrix_ptr[1] == expected_coef1);
    cout << matrix_ptr[1] << endl;

    ++count;

    for (size_t i(2); i < 3*n-4; i+=3)
    {
        cout << " Row " << count << ": ";
        assert(matrix_ptr[i] == expected_coef1);
        cout << matrix_ptr[i] << " ";

        assert(matrix_ptr[i+1] == expected_coef2);
        cout << matrix_ptr[i+1] << " ";

        assert(matrix_ptr[i+2] == expected_coef1);
        cout << matrix_ptr[i+2] << endl;

        ++count;
    }

    cout << " Row " << count << ": ";
    assert(matrix_ptr[3*n-4] == expected_coef1);
    cout << matrix_ptr[3*n-4] << " ";

    assert(matrix_ptr[3*n-3] == expected_coef2);
    cout << matrix_ptr[3*n-3] << endl;

    delete[] rhs_ptr;
    delete[] matrix_ptr;

    return 0;
}
