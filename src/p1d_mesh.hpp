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
#include <iterator>

namespace poisson1d {

enum MeshGlobalPosition {
    _left,
    _middle,
    _right,
    _full
};

class Mesh {

    public:
        Mesh(Real a, Real b, Real n, MeshGlobalPosition position);

        class const_iterator : std::iterator<std::bidirectional_iterator_tag, Real>
        {
            public:
                const_iterator();
                const_iterator(const const_iterator & other);
                const_iterator & operator=(const const_iterator & other);

                Real* operator->();
                const Real* operator->() const;

                Real& operator*();
                const Real& operator*() const;

                const_iterator & operator++();
                const_iterator operator++(int);

                const_iterator & operator--();
                const_iterator operator--(int);

                bool operator==(const const_iterator & other) const;
                bool operator!=(const const_iterator & other) const;

            private:
                const_iterator(const Mesh* mesh, size_t index);
                void compute_node();
                friend class Mesh;
                const Mesh* mesh_ptr;
                size_t index;
                Real node;
        };

        size_t getNbNodes() const;
        Real getLowerBound() const;
        Real getUpperBound() const;
        MeshGlobalPosition getGlobalPosition() const;
        const_iterator begin() const;
        const_iterator end() const;

    private:
        Real a;
        Real b;
        MeshGlobalPosition position;
        size_t n;
};

} //namespace poisson1d

#endif
