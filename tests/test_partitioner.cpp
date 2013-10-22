/*
 * test_partitioner.cpp
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

#include "p1d_partitioner.hpp"
#include <cassert>
#include <cstdio>
#include <limits>
#include <cmath>

using namespace poisson1d;
using namespace std;

int main(int argc, char* argv[])
{
    Real epsilon = std::numeric_limits<Real>::epsilon() * 100;
    printf("Admitted epsilon: %.2e\n", epsilon);

    Real a(0), b(0.8);
    Real fa(13.13), fb(14.14);
    size_t n(11);
    Real dx = (b-a) / (n-1);
    std::string rhs_func("x");
    size_t num_jobs(3);
    size_t num_rows[num_jobs];

    for(size_t i(0); i < num_jobs; ++i)
    {
        num_rows[i] = n/num_jobs;
        if((n % num_jobs) != 0 && i < (n % num_jobs))
        {
            num_rows[i] += 1;
        }
    }

    MeshGlobalPosition position(_full);

    Mesh full_mesh(a, b, n, position);
    Problem problem(full_mesh, fa, fb, rhs_func, num_jobs);
    Partitioner partitioner(problem);
    const Job* jobs[num_jobs];
    const Mesh* partial_meshes_ptr[num_jobs];

    for(size_t i(0); i < num_jobs; ++i)
    {
        jobs[i] = partitioner.get_job_alloc(i);
        partial_meshes_ptr[i] = &jobs[i]->get_problem().get_mesh();
    }

    assert(partial_meshes_ptr[0]->get_num_nodes() == num_rows[0]+1);
    assert(partial_meshes_ptr[1]->get_num_nodes() == num_rows[1]+2);
    assert(partial_meshes_ptr[2]->get_num_nodes() == num_rows[2]+1);

    assert(std::abs(partial_meshes_ptr[0]->get_upper_bound() - (num_rows[0])*dx) <= epsilon);
    assert(std::abs(partial_meshes_ptr[1]->get_upper_bound() - (num_rows[1]+num_rows[0])*dx) <= epsilon);
    assert(std::abs(partial_meshes_ptr[2]->get_upper_bound() - b) <= epsilon);

    assert(std::abs(partial_meshes_ptr[0]->get_lower_bound() - a) <= epsilon);
    assert(std::abs(partial_meshes_ptr[1]->get_lower_bound() - (num_rows[0]-1)*dx) <= epsilon);
    assert(std::abs(partial_meshes_ptr[2]->get_lower_bound() - (num_rows[1]+num_rows[0]-1)*dx) <= epsilon);

    for(size_t i(0); i < num_jobs; ++i)
    {
        delete jobs[i];
    }
    return 0;
}
