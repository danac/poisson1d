/*
 * p1d_mesh.hpp
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

#ifndef P1D_MESH
#define P1D_MESH

#include "p1d_common.hpp"
#include "p1d_mesh_iterator.hpp"
#include "p1d_packable.hpp"

namespace poisson1d {

enum MeshGlobalPosition {
    _left,
    _middle,
    _right,
    _full,
    _undefined
};

class MeshBaseIterator;

class Mesh : public Packable {

    public:
        Mesh(Real a, Real b, Real n, MeshGlobalPosition position);
        Mesh();

        typedef MeshConstIterator const_iterator;

        size_t get_num_nodes() const;
        Real get_lower_bound() const;
        Real get_upper_bound() const;
        MeshGlobalPosition get_global_position() const;
        const_iterator begin() const;
        const_iterator end() const;

        virtual void pack(std::ostream& buffer) const;
        virtual void unpack(std::istream& buffer);

    private:
        friend class MeshBaseIterator;
        Real a;
        Real b;
        MeshGlobalPosition position;
        size_t n;
};

} //namespace poisson1d

#endif
