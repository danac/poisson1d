/*
 * test_job_result.cpp
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

#include <cassert>
#include <string>
#include <iostream>

#include "p1d_structs.hpp"
#include "p1d_assembler.hpp"
#include "p1d_mesh.hpp"

using namespace poisson1d;
using namespace std;

int main(int argc, char* argv[])
{
    Real a(-14.68);
    Real b(14567.0);
    Real fa(10.314);
    Real fb(-0.1);
    size_t n(500);
    string rhs_func("x*x-2");
    size_t num_jobs(1);
    size_t rank(0);

    MeshGlobalPosition position = _full;

    Mesh mesh(a, b, n, position);

    Problem problem(mesh, fa, fb, rhs_func, num_jobs);
    Job job(problem, rank);
    DistributedAssembler assembler(job);

    JobResult* job_result = assembler.get_job_result_alloc();

    size_t job_result_size = job_result->get_packed_size();
    Byte* buffer = new Byte[job_result_size];
    Byte* test_ptr(NULL);

    test_ptr = job_result->pack(buffer);
    assert(test_ptr == buffer + job_result_size);

    JobResult new_job_result;
    new_job_result.unpack(buffer);

    assert(new_job_result == *job_result);

    delete[] buffer;
    delete job_result;
    return 0;
}
