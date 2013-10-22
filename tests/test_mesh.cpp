/*
 * test_mesh.cpp
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

#include "p1d_mesh.hpp"
#include <cassert>
#include <cstdio>
#include <limits>
#include <cmath>

using namespace poisson1d;
using namespace std;

int main(int argc, char* argv[])
{
    Real a(0.0);
    Real b(1.0);
    size_t n(10);
    Real dx = (b-a)/(n-1);

    Real epsilon = std::numeric_limits<Real>::epsilon() * 100;

    Mesh mesh(a, b, n, _full);
    Mesh::const_iterator mesh_it;

    mesh_it = mesh.begin();
    size_t i(0);

    printf("Testing forward iteration... \n");
    for(mesh_it = mesh.begin(); mesh_it != mesh.end(); ++mesh_it, ++i)
    {
        Real expected_value = i*dx;
        Real diff = std::abs(*mesh_it - expected_value);
        printf(" i = %lu | *mesh_it = %.2f | expected %.2f | error %.2e \n", i, *mesh_it, expected_value, diff);
        assert(diff <= epsilon);
    }

    i = 0;
    printf("Testing backward iteration... \n");
    for(mesh_it = mesh.end()-1; mesh_it != mesh.begin(); --mesh_it, ++i)
    {
        Real expected_value = b-i*dx;
        Real diff = std::abs(*mesh_it - expected_value);
        printf(" i = %lu | *mesh_it = %.2f | expected %.2f | error %.2e \n", n-1-i, *mesh_it, expected_value, diff);
        assert(diff <= epsilon);
    }

    printf("Testing random iteration... \n");
    mesh_it = mesh.begin();
    for(size_t i(0); i < n-1; ++i)
    {
        Real expected_value = i*dx;
        Real diff = std::abs(mesh_it[i] - expected_value);
        printf(" i = %lu | *mesh_it = %.2f | expected %.2f | error %.2e \n", n-1-i, *mesh_it, expected_value, diff);
        assert(diff <= epsilon);

    }

    return 0;
}
