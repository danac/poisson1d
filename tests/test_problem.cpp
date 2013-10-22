/*
 * test_problem.cpp
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

using namespace poisson1d;
using namespace std;

int main(int argc, char* argv[])
{
    Real a(13.13);
    Real b(1.0);
    size_t n(10);
    Real fa(10.0);
    Real fb(3.14);
    std::string rhs_func("x*3-2*x^2");
    size_t num_jobs(3);
    MeshGlobalPosition position = _full;

    Mesh mesh(a, b, n, position);
    Problem problem(mesh, fa, fb, rhs_func, num_jobs);

    size_t problem_size = problem.get_packed_size();

    Byte* buffer = new Byte[problem_size];
    Byte* test_ptr = problem.pack(buffer);
    assert(test_ptr == buffer + problem_size);

    Problem new_problem;

    new_problem.unpack(buffer);

    assert(new_problem == problem);

    delete[] buffer;

    return 0;
}
