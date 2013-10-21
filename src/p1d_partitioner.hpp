/*
 * p1d_partitioner.hpp
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

#ifndef P1D_PARTITIONER
#define P1D_PARTITIONER

#include "p1d_common.hpp"
#include "p1d_structs.hpp"
#include "p1d_mesh.hpp"

namespace poisson1d {

class Partitioner
{
    public:
        Partitioner(const Problem& problem);
        ~Partitioner();

        Job* get_job_alloc(size_t rank) const;

    private:
        const Problem* problem_ptr;
};

} //namespace poisson1d

#endif
