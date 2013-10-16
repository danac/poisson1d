/*
 * p1d_mesh.cpp
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

namespace poisson1d {

Mesh::Mesh(Real a_, Real b_, Real n_, MeshGlobalPosition position_)
: a(a_), b(b_), n(n_), position(position_)
{}

Mesh::Mesh()
: a(0.), b(0.), n(0), position(_undefined)
{}

size_t Mesh::get_num_nodes() const
{
    return n;
}

Real Mesh::get_lower_bound() const
{
    return a;
}

Real Mesh::get_upper_bound() const
{
    return b;
}

MeshGlobalPosition Mesh::get_global_position() const
{
    return position;
}

Mesh::const_iterator Mesh::begin() const {
    return const_iterator(this, 0);
}

Mesh::const_iterator Mesh::end() const {
    return const_iterator(this, n);
}

void Mesh::pack(std::ostream& buffer) const
{
    buffer << a << b << n << static_cast<int>(position);
}

void Mesh::unpack(std::istream& buffer)
{
    buffer >> a;
    buffer >> b;
    buffer >> n;
    int position_int(0);
    buffer >> position_int;
    position = static_cast<MeshGlobalPosition>(position_int);
}

} //namespace poisson1d
