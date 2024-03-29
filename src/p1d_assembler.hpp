/*
 * p1d_assembler.hpp
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

#ifndef P1D_ASSEMBLER
#define P1D_ASSEMBLER

#include <string>
#include "p1d_common.hpp"
#include "p1d_mesh.hpp"
#include "p1d_structs.hpp"

namespace poisson1d {

class DistributedAssembler
{
    public:
        DistributedAssembler(const Job& job);
        ~DistributedAssembler();

        JobResult* get_job_result_alloc();

    private:
        void assemble_rhs(Real* rhs_ptr) const;
        Real* assemble_rhs_alloc() const;

        void assemble_matrix(Real* matrix_ptr) const;
        Real* assemble_matrix_alloc() const;

        size_t get_matrix_nnz() const;
        size_t get_rhs_size() const;

        const Job* job_ptr;
        const Mesh* mesh_ptr;
        std::string rhs_func;
        Real a;
        Real b;
        Real fa;
        Real fb;
        size_t n;
        size_t nnz;
        size_t rhs_size;
};

} //namespace poisson1d

#endif //P1D_ASSEMBLER
