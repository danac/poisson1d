/*
 * p1d_structs.cpp
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

#include "p1d_structs.hpp"
#include <cstring>

namespace poisson1d {

/*
 *  Problem
 */

Problem::Problem(Mesh& mesh, Real fa_, Real fb_, std::string rhs_func_, size_t num_jobs_, bool ownership)
: mesh_ptr(&mesh), fa(fa_), fb(fb_), rhs_func(rhs_func_), num_jobs(num_jobs_), data_ownership(ownership)
{}

Problem::Problem()
: mesh_ptr(NULL), fa(0), fb(0), rhs_func(""), num_jobs(0), data_ownership(true)
{
    mesh_ptr = new Mesh();
}

Problem::~Problem()
{
    if(data_ownership)
    {
        delete mesh_ptr;
    }
}

Byte* Problem::pack(Byte* buffer) const
{
    Byte* cursor = buffer;
    cursor = mesh_ptr->pack(cursor);
    cursor = write_to_buffer(fa, cursor);
    cursor = write_to_buffer(fb, cursor);
    cursor = write_to_buffer(rhs_func, cursor);
    cursor = write_to_buffer(num_jobs, cursor);
    return cursor;
}

const Byte* Problem::unpack(const Byte* buffer)
{
    const Byte* cursor = buffer;
    cursor = mesh_ptr->unpack(cursor);
    cursor = read_from_buffer(fa,cursor);
    cursor = read_from_buffer(fb,cursor);
    cursor = read_from_buffer(rhs_func,cursor);
    cursor = read_from_buffer(num_jobs, cursor);
    return cursor;
}

size_t Problem::get_packed_size() const
{
    size_t size(0);
    size += mesh_ptr->get_packed_size();
    size += sizeof(fa);
    size += sizeof(fb);
    size += rhs_func.size()+1; // The null-character is packed
    size += sizeof(num_jobs);
    return size;
}

bool Problem::operator==(const Problem& other) const
{
    bool identical = (*mesh_ptr == *(other.mesh_ptr) &&
                      fa == other.fa &&
                      fb == other.fb &&
                      rhs_func == other.rhs_func &&
                      num_jobs == other.num_jobs);

    return identical;
}

const Mesh& Problem::get_mesh() const
{
    return *mesh_ptr;
}

Real Problem::get_fa() const
{
    return fa;
}

Real Problem::get_fb() const
{
    return fb;
}

std::string Problem::get_rhs_func() const
{
    return rhs_func;
}
size_t Problem::get_num_jobs() const
{
    return num_jobs;
}

/*
 *  Job
 */

Job::Job(Problem& problem, size_t rank_, bool ownership)
: problem_ptr(&problem), rank(rank_), data_ownership(ownership)
{}

Job::Job()
: problem_ptr(NULL), rank(0), data_ownership(true)
{
    problem_ptr = new Problem();
}

Job::~Job()
{
    if(data_ownership)
    {
        delete problem_ptr;
    }
}

Byte* Job::pack(Byte* buffer) const
{
    Byte* cursor = buffer;
    cursor = problem_ptr->pack(cursor);
    cursor = write_to_buffer(rank, cursor);
    return cursor;
}

const Byte* Job::unpack(const Byte* buffer)
{
    const Byte* cursor = buffer;
    cursor = problem_ptr->unpack(cursor);
    cursor = read_from_buffer(rank, cursor);
    return cursor;
}

size_t Job::get_packed_size() const
{
    size_t size(0);
    size += problem_ptr->get_packed_size();
    size += sizeof(rank);
    return size;
}

bool Job::operator==(const Job& other) const
{
    bool identical = (*problem_ptr == *(other.problem_ptr) &&
                      rank == other.rank);

    return identical;
}

const Problem& Job::get_problem() const
{
    return *problem_ptr;
}

/*
 *  JobResult
 */

JobResult::JobResult(Real* matrix_ptr_, Real* rhs_ptr_, size_t rank_, size_t nnz_, size_t n_, bool ownership)
: rhs_ptr(rhs_ptr_), matrix_ptr(matrix_ptr_), nnz(nnz_), n(n_), rank(rank_), data_ownership(ownership)
{}

JobResult::JobResult()
: rhs_ptr(NULL), matrix_ptr(NULL), nnz(0), n(0), rank(0), data_ownership(true)
{}

JobResult::~JobResult()
{
    if(data_ownership)
    {
        delete[] rhs_ptr;
        delete[] matrix_ptr;
    }
}

size_t JobResult::get_packed_size() const
{
    size_t size(0);
    size += sizeof(nnz);
    size += sizeof(n);
    size += sizeof(rank);
    size += n*sizeof(Real);
    size += nnz*sizeof(Real);
    return size;
}

Byte* JobResult::pack(Byte* const buffer) const
{
    Byte* cursor = buffer;

    cursor = write_to_buffer(nnz, cursor);
    cursor = write_to_buffer(n, cursor);
    cursor = write_to_buffer(rank, cursor);

    size_t matrix_byte_size = nnz*sizeof(Real);
    memcpy(cursor, matrix_ptr, matrix_byte_size);
    cursor += matrix_byte_size;

    size_t rhs_byte_size = n*sizeof(Real);
    memcpy(cursor, rhs_ptr, rhs_byte_size);
    cursor += rhs_byte_size;

    return cursor;
}

const Byte* JobResult::unpack(const Byte* buffer)
{
    const Byte* cursor = buffer;

    cursor = read_from_buffer(nnz, cursor);
    cursor = read_from_buffer(n, cursor);
    cursor = read_from_buffer(rank, cursor);

    size_t matrix_byte_size = nnz*sizeof(Real);
    matrix_ptr = new Real[nnz];
    memcpy(matrix_ptr, cursor, matrix_byte_size);
    cursor += matrix_byte_size;

    size_t rhs_byte_size = n*sizeof(Real);
    rhs_ptr = new Real[n];
    memcpy(rhs_ptr, cursor, rhs_byte_size);
    cursor += rhs_byte_size;
    return cursor;
}

bool JobResult::operator==(const JobResult& other) const
{
    bool matrix_identical(true);
    for(size_t i(0); i < nnz && matrix_identical; ++i)
    {
        matrix_identical = (matrix_ptr[i] == other.matrix_ptr[i]);
    }

    bool rhs_identical(true);
    for(size_t i(0); i < n && rhs_identical; ++i)
    {
        rhs_identical = (rhs_ptr[i] == other.rhs_ptr[i]);
    }

    bool identical = (rhs_identical &&
                      matrix_identical &&
                      n == other.n &&
                      nnz == other.nnz &&
                      rank == other.rank);

    return identical;
}


Solution::Solution(Real* x_ptr_, size_t n_, bool ownership)
: x_ptr(x_ptr_), n(n_), data_ownership(ownership)
{}

Solution::Solution()
: x_ptr(NULL), n(0), data_ownership(true)
{}

Solution::~Solution()
{
    if(data_ownership)
    {
        delete[] x_ptr;
    }
}


Byte* Solution::pack(Byte* const buffer) const
{
    Byte* cursor = buffer;

    cursor = write_to_buffer(n, cursor);

    size_t x_byte_size = n*sizeof(Real);
    memcpy(cursor, x_ptr, x_byte_size);
    cursor += x_byte_size;

    return cursor;
}

const Byte* Solution::unpack(const Byte* buffer)
{
    const Byte* cursor = buffer;

    cursor = read_from_buffer(n, cursor);

    size_t x_byte_size = n*sizeof(Real);
    x_ptr = new Real[n];
    memcpy(x_ptr, cursor, x_byte_size);
    cursor += x_byte_size;

    return cursor;
}

size_t Solution::get_packed_size() const
{
    return sizeof(n) + n * sizeof(Real);
}

bool Solution::operator==(const Solution& other) const
{
    bool identical(true);
    for(size_t i(0); i < n && identical; ++i)
    {
        identical = (x_ptr[i] == other.x_ptr[i]);
    }
    return identical;
}

} //namespace poisson1d
