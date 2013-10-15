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

size_t Mesh::getNbNodes() const
{
    return n;
}

Real Mesh::getLowerBound() const
{
    return a;
}

Real Mesh::getUpperBound() const
{
    return b;
}

MeshGlobalPosition Mesh::getGlobalPosition() const
{
    return position;
}

Mesh::const_iterator Mesh::begin() const {
    return const_iterator(this, 0);
}

Mesh::const_iterator Mesh::end() const {
    return const_iterator(this, n);
}

Mesh::const_iterator::const_iterator()
: node(0), mesh_ptr(NULL), index(0)
{}

Mesh::const_iterator::const_iterator(const const_iterator & other)
: mesh_ptr(other.mesh_ptr), index(other.index), node(0.)
{
    compute_node();
}

Mesh::const_iterator & Mesh::const_iterator::operator=(const const_iterator & other)
{
    mesh_ptr = other.mesh_ptr;
    index = other.index;
    compute_node();
}

Mesh::const_iterator::const_iterator(const Mesh* mesh, size_t index_)
: mesh_ptr(mesh), index(index_), node(0.)
{
    compute_node();
}

Real* Mesh::const_iterator::operator->()
{
    return &node;
}
const Real* Mesh::const_iterator::operator->() const
{
    return &node;
}

Real& Mesh::const_iterator::operator*()
{
    return node;
}

const Real& Mesh::const_iterator::operator*() const
{
    return node;
}

Mesh::const_iterator & Mesh::const_iterator::operator++()
{
    size_t n = mesh_ptr->getNbNodes();
    if(index != n)
    {
        index += 1;
    }
    compute_node();
    return *this;
}

Mesh::const_iterator Mesh::const_iterator::operator++(int)
{
    const_iterator copy(*this);
    operator++();
    return copy;
}


Mesh::const_iterator & Mesh::const_iterator::operator--()
{
    if(index != 0)
    {
        index -= 1;
    }
    compute_node();
    return *this;
}

Mesh::const_iterator Mesh::const_iterator::operator--(int)
{
    const_iterator copy(*this);
    operator--();
    return copy;
}

bool Mesh::const_iterator::operator==(const const_iterator & other) const
{
    return index == other.index;
}

bool Mesh::const_iterator::operator!=(const const_iterator & other) const
{
    return index != other.index;
}

inline void Mesh::const_iterator::compute_node()
{
    Real a = mesh_ptr->getLowerBound();
    Real b = mesh_ptr->getUpperBound();
    Real n = mesh_ptr->getNbNodes();
    Real dx = (b-a)/n;
    node = index*dx;
}

} //namespace poisson1d
