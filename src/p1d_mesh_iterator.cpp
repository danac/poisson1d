/*
 * p1d_mesh_iterator.cpp
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

#include "p1d_mesh_iterator.hpp"
#include "p1d_mesh.hpp"

namespace poisson1d {

MeshConstIterator::MeshConstIterator()
: mesh_ptr(NULL), index(0), node(0)
{}

MeshConstIterator::MeshConstIterator(const MeshConstIterator & other)
: mesh_ptr(other.mesh_ptr), index(other.index), node(0.)
{
    compute_node();
}

MeshConstIterator::MeshConstIterator(const Mesh* mesh, size_t index_)
: mesh_ptr(mesh), index(index_), node(0.)
{
    compute_node();
}

void MeshConstIterator::increment()
{
    size_t n = mesh_ptr->get_num_nodes();
    if(index != n)
    {
        index += 1;
    }
    compute_node();
}

void MeshConstIterator::decrement()
{
    if(index != 0)
    {
        index -= 1;
    }
    compute_node();
}

void MeshConstIterator::advance(size_t steps)
{
    size_t n = mesh_ptr->get_num_nodes();
    if(index <= n-steps)
    {
        index += steps;
    }
    else
    {
        index = n;
    }
    compute_node();
}

ptrdiff_t MeshConstIterator::distance_to(MeshConstIterator const& other) const
{
    return other.index-index;
}

bool MeshConstIterator::equal(MeshConstIterator const& other) const
{
    return index == other.index;
}

const Real& MeshConstIterator::dereference() const
{
    return node;
}

void MeshConstIterator::compute_node()
{
    Real a = mesh_ptr->get_lower_bound();
    Real b = mesh_ptr->get_upper_bound();
    Real n = mesh_ptr->get_num_nodes();
    Real dx = (b - a) / (n - 1);
    node = index * dx;
}

} //namespace poisson1d
