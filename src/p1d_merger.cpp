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

namespace poisson1d {

Merger::Merger(size_t n_, size_t num_jobs_)
: full_matrix_ptr(NULL), full_rhs_ptr(NULL), n(n_), num_jobs(num_jobs_)
{
    size_t nnz = get_matrix_nnz();

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
    for(size_t i(0); i < get_job_nnz(job_id); ++i)
    {
        target_ptr[i] = matrix_ptr[i];
    }
}

void Merger::merge_rhs(const Real* rhs_ptr, size_t job_id)
{
    size_t offset = get_job_rows_offset(job_id);
    Real* target_ptr = full_rhs_ptr+offset;
    for(size_t i(0); i < get_job_rows(job_id); ++i)
    {
        target_ptr[i] = rhs_ptr[i];
    }
}
size_t Merger::get_matrix_nnz() const
{
    return 3*(n-2)+2;
}

const Real* Merger::get_matrix_ptr() const
{
    return full_matrix_ptr;
}

const Real* Merger::get_rhs_ptr() const
{
    return full_rhs_ptr;
}


size_t Merger::get_job_rows(size_t job_id) const
{
    size_t num_rows = (size_t)(n/num_jobs);

    if(job_id == num_jobs-1)
    {
        num_rows += 1;
    }

    return num_rows;
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
            offset += get_job_rows(i);
        }
    }
    return offset;
}


size_t Merger::get_job_nnz(size_t job_id) const
{
    size_t job_nnz(0);
    size_t job_rows = get_job_rows(job_id);

    if(job_id == 0 || job_id == num_jobs-1)
    {
        job_nnz = (job_rows-1)*3+1;
    }
    else if(num_jobs == 1)
    {
        job_nnz = get_matrix_nnz();
    }
    else
    {
        job_nnz = job_rows*3;
    }
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
            offset += get_job_nnz(i);
        }
    }
    return offset;
}

} //namespace poisson1d
