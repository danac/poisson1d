/*
 * p1d_helper_functions.cpp
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

#include "p1d_helper_functions.hpp"

namespace poisson1d {
namespace utils {

size_t get_job_nnz(size_t job_id, size_t n, size_t num_jobs)
{
    size_t job_nnz(0);
    size_t job_rows = get_job_rows(job_id, n, num_jobs);

    if(job_id == 0 || job_id == num_jobs-1)
    {
        job_nnz = (job_rows-1)*3+1;
    }
    else if(num_jobs == 1)
    {
        job_nnz = get_matrix_nnz(n);
    }
    else
    {
        job_nnz = job_rows*3;
    }
    return job_nnz;
}

size_t get_job_rows(size_t job_id, size_t n, size_t num_jobs)
{
    size_t num_rows = n/num_jobs;
    size_t remainder = n % num_jobs;

    if(remainder != 0 && job_id < remainder)
    {
        num_rows += 1;
    }

    return num_rows;
}

size_t get_matrix_nnz(size_t n, MeshGlobalPosition position)
{
    size_t nnz(0);
    if(position == _full)
    {
        nnz = 3*(n-2)+2; // n-2 rows with 3 coefs and 1 coef on both first and last rows
    }
    else if(position == _left)
    {
        nnz = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
    }
    else if(position == _right)
    {
        nnz = 3*(n-2)+1; // n-2 rows with 3 coefs because the last node is a ghost
    }
    else if(position == _middle)
    {
        nnz = 3*(n-2); // n-2 rows with 3 coefs beacuse both ends are ghosts
    }
    else
    {
        throw;
    }

    return nnz;
}

void dealloc_hook (void* data, void* hint)
{
    delete[] static_cast<Real*>(data);
}

} //namespace utils
} //namespace poisson1d

