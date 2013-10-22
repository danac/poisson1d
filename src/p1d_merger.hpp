/*
 * p1d_merger.hpp
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

#ifndef P1D_MERGER
#define P1D_MERGER

#include "p1d_common.hpp"
#include "p1d_structs.hpp"

namespace poisson1d {

class Merger
{
    public:
        Merger(size_t n, size_t num_jobs);
        ~Merger();

        void merge_job_result(const JobResult& job_result);
        const Real* get_matrix_ptr() const;
        const Real* get_rhs_ptr() const;

        void merge_matrix(const Real* matrix_ptr, size_t job_id);
        void merge_rhs(const Real* matrix_ptr, size_t job_id);

    private:
        size_t get_job_nnz_offset(size_t job_id) const;
        size_t get_job_rows_offset(size_t job_id) const;

        Real* full_matrix_ptr;
        Real* full_rhs_ptr;

        size_t n;
        size_t num_jobs;
};

} //namespace poisson1d

#endif
