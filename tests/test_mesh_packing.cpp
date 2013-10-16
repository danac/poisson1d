/*
 * test_mesh_packing.cpp
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

#include "p1d_mesh.hpp"
#include <sstream>
#include <cassert>

using namespace poisson1d;
using namespace std;

int main(int argc, char* argv[])
{
    stringstream stream;

    Real a(0), b(0);
    size_t n(10);
    MeshGlobalPosition position(_full);

    Mesh mesh(a, b, n, position);
    mesh.pack(stream);

    Mesh unpacked_mesh;

    unpacked_mesh.unpack(stream);

    assert(mesh.get_lower_bound() == a);
    assert(mesh.get_upper_bound() == b);
    assert(mesh.get_num_nodes() == n);
    assert(mesh.get_global_position() == position);

    return 0;
}
