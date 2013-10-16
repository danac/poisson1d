/*
 * p1d_partitioner.cpp
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

#include "p1d_partitioner.hpp"
#include <cassert>

namespace poisson1d {

Partitioner::Partitioner(const Mesh& mesh, size_t num_jobs_)
: full_mesh_ptr(&mesh), num_jobs(num_jobs_)
{}

Partitioner::~Partitioner()
{}

Mesh* Partitioner::get_partitioned_mesh_alloc(size_t rank) const
{
    assert(rank < num_jobs);

    size_t n = full_mesh_ptr->get_num_nodes();
    Real a = full_mesh_ptr->get_lower_bound();
    Real b = full_mesh_ptr->get_upper_bound();

    Mesh::const_iterator mesh_it = full_mesh_ptr->begin();

    size_t n2 = n/num_jobs;
    Real a2(0);
    Real b2(0);

    MeshGlobalPosition position(_middle);

    if(rank == 0)
    {
        if(num_jobs == 1)
        {
            position = _full;
            a2 = a;
            b2 = b;
        }
        else
        {
            // We include a ghost node at the right end
            a2 = a;
            b2 = mesh_it[(rank+1)*n2];
            position = _left;

        }
    }
    else if(rank == num_jobs-1)
    {
        if(n % num_jobs)
        {
            n2 += 1;
        }

        // We include a ghost node at the right end
        a2 = mesh_it[rank*n2];
        b2 = b;
        position = _right;
    }
    else
    {
        // We include ghost nodes at each end
        a2 = mesh_it[rank*n2-1];
        b2 = mesh_it[(rank+1)*n2];
    }

    return new Mesh(a2, b2, n2, position);
}


} //namespace poisson1d
