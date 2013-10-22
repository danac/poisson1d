/*
 * p1d_mesh_iterator.hpp
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

#ifndef P1D_MESH_ITERATOR
#define P1D_MESH_ITERATOR

#include <boost/iterator/iterator_facade.hpp>
#include "p1d_common.hpp"

namespace poisson1d {

class Mesh;

class MeshConstIterator
  : public boost::iterator_facade<
      MeshConstIterator,
      const Real,
      boost::forward_traversal_tag>
{
    public:
        MeshConstIterator();
        MeshConstIterator(const MeshConstIterator & other);

    private:
        MeshConstIterator(const Mesh* mesh, size_t index_);

        void increment();

        void decrement();

        void advance(size_t steps);

        ptrdiff_t distance_to(MeshConstIterator const& other) const;

        bool equal(MeshConstIterator const& other) const;

        const Real& dereference() const;

        void compute_node();

        friend class boost::iterator_core_access;
        friend class Mesh;

        const Mesh* mesh_ptr;
        size_t index;
        Real node;
};

} //namespace poisson1d
#endif
