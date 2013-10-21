/*
 * p1d_merger.cpp
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

#include "p1d_merger.hpp"
#include "p1d_helper_functions.hpp"

namespace poisson1d {

using utils::get_job_rows;
using utils::get_job_nnz;
using utils::get_matrix_nnz;

Merger::Merger(size_t n_, size_t num_jobs_)
: full_matrix_ptr(NULL), full_rhs_ptr(NULL), n(n_), num_jobs(num_jobs_)
{
    size_t nnz = get_matrix_nnz(n);

    full_matrix_ptr = new Real[nnz];
    full_rhs_ptr = new Real[n];
}

Merger::~Merger()
{
    delete[] full_matrix_ptr;
    delete[] full_rhs_ptr;
}

void Merger::merge_matrix(const Real* matrix_ptr, size_t job_id)
{
    size_t offset = get_job_nnz_offset(job_id);
    Real* target_ptr = full_matrix_ptr+offset;
    for(size_t i(0); i < get_job_nnz(job_id, n, num_jobs); ++i)
    {
        target_ptr[i] = matrix_ptr[i];
    }
}

void Merger::merge_rhs(const Real* rhs_ptr, size_t job_id)
{
    size_t offset = get_job_rows_offset(job_id);
    //std::cout << "JOB " << job_id << " OFFSET " << offset << " JOB_ROWS " << get_job_rows(job_id) << std::endl;
    Real* target_ptr = full_rhs_ptr+offset;
    for(size_t i(0); i < get_job_rows(job_id, n, num_jobs); ++i)
    {
        target_ptr[i] = rhs_ptr[i];
    }
}

const Real* Merger::get_matrix_ptr() const
{
    return full_matrix_ptr;
}

const Real* Merger::get_rhs_ptr() const
{
    return full_rhs_ptr;
}

size_t Merger::get_job_rows_offset(size_t job_id) const
{
    size_t offset(0);

    if(job_id == 0)
    {
        offset = 0;
    }
    else
    {
        for(size_t i(0); i < job_id; ++i)
        {
            offset += get_job_rows(i, n, num_jobs);
        }
    }
    return offset;
}


size_t Merger::get_job_nnz_offset(size_t job_id) const
{
    size_t offset(0);

    if(job_id == 0)
    {
        offset = 0;
    }
    else
    {
        for(size_t i(0); i < job_id; ++i)
        {
            offset += get_job_nnz(i, n, num_jobs);
        }
    }
    return offset;
}


void Merger::merge_job_result(const JobResult& job_result)
{
    size_t rank = job_result.get_rank();
    merge_matrix(job_result.get_matrix_ptr(), rank);
    merge_rhs(job_result.get_rhs_ptr(), rank);
}

} //namespace poisson1d
