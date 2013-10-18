/*
 * p1d_helper_functions.hpp
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

#ifndef P1D_HELPER_FUNCTIONS
#define P1D_HELPER_FUNCTIONS

#include "p1d_common.hpp"
#include "p1d_mesh.hpp"

namespace poisson1d {

namespace utils {

size_t get_job_nnz(size_t job_id, size_t n, size_t num_jobs);
size_t get_job_rows(size_t job_id, size_t n, size_t num_jobs);
size_t get_full_matrix_nnz(size_t n);
size_t get_matrix_nnz(size_t n, MeshGlobalPosition position = _full);

} //namespace utils

} //namespace poisson1d
#endif
