/*
 * test_structs.cpp
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

#include "p1d_structs.hpp"
#include <cassert>
#include <string>
#include <limits>
#include <iostream>
#include <iomanip>

using namespace poisson1d;
using namespace std;

int main(int argc, char* argv[])
{
    size_t n(1000);

    Real* x_ptr = new Real[n];

    for(size_t i(0); i < n; ++i)
    {
        x_ptr[i] = (Real)i + 0.123465;
    }

    Solution solution(x_ptr, n);
    size_t solution_size = solution.get_packed_size();

    Byte* buffer = new Byte[solution_size];
    Byte* test_ptr = solution.pack(buffer);
    assert(test_ptr == buffer + solution_size);

    Solution new_solution;

    new_solution.unpack(buffer);

    assert(new_solution == solution);

    delete[] x_ptr;
    delete[] buffer;

    return 0;
}
