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
#include <iostream>
#include <string>

namespace poisson1d {

Partitioner::Partitioner(const Problem& problem)
: problem_ptr(&problem)
{}

Partitioner::~Partitioner()
{}

Job* Partitioner::get_job_alloc(size_t rank) const
{

    const Mesh& full_mesh = problem_ptr->get_mesh();
    size_t num_jobs = problem_ptr->get_num_jobs();

    size_t n = full_mesh.get_num_nodes();
    Real a = full_mesh.get_lower_bound();
    Real b = full_mesh.get_upper_bound();
    Real fa = problem_ptr->get_fa();
    Real fb = problem_ptr->get_fb();
    std::string rhs_func = problem_ptr->get_rhs_func();

    assert(rank < num_jobs);

    Mesh::const_iterator mesh_it = full_mesh.begin();

    Real a2(0);
    Real b2(0);
    size_t n2(n/num_jobs);
    size_t remainder = n % num_jobs;
    MeshGlobalPosition position(_middle);

    size_t offset(0);

    if(remainder != 0 && rank < remainder)
    {
        n2 += 1;
        offset = n2 * rank;
    }
    else
    {
        offset = remainder * (n2 + 1) + (rank - remainder) * n2;
    }
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
            b2 = mesh_it[offset+n2];
            position = _left;
            n2 += 1; // One ghost
        }
    }
    else if(rank == num_jobs-1)
    {
        // We include a ghost node at the right end
        a2 = mesh_it[offset-1];
        b2 = b;
        position = _right;
        n2 += 1;
    }
    else
    {
        // We include ghost nodes at each end
        a2 = mesh_it[offset-1];
        b2 = mesh_it[offset+n2];
        n2 += 2;
    }

    //std::cout << "-OFFSET " << offset << "-";
    //std::cout << "-n2 " << n2 << "-";
    Mesh* new_mesh = new Mesh(a2, b2, n2, position);
    Problem* new_problem = new Problem(*new_mesh, fa, fb, rhs_func, num_jobs, true);
    Job* new_job = new Job(*new_problem, rank, true);

    return new_job;
}


} //namespace poisson1d
