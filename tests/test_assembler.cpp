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

using namespace poisson1d;

int main(int argc, char* argv[])
{

    Real a(-1.0);
    Real b(1.0);
    UInt n(100);
    std::string rhs_func("-x*x+1");

    DistributedAssembler assembler(a, b, n, rhs_func);

    assembler.assemble_rhs();
    assembler.assemble_matrix();

    const Real* rhs_ptr = assembler.get_rhs_ptr();
    const Real* matrix_ptr = assembler.get_matrix_ptr();

    Real dx = (b-a)/n;

    for (std::size_t i(0); i < n; ++i)
    {
        Real expected_value = -(a+i*dx)*(a+i*dx)+1;
        assert(rhs_ptr[i] == expected_value);
    }

    for (std::size_t i(0); i < 3*n; i+=3)
    {
        Real expected_coef1 = -1/dx;
        Real expected_coef2 = 2/dx;

        assert(matrix_ptr[i] == expected_coef1);
        assert(matrix_ptr[i+1] == expected_coef2);
        assert(matrix_ptr[i+2] == expected_coef1);
    }

    return 0;
}
