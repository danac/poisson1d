/*
 * p1d_assembler.hpp
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

#ifndef P1D_ASSEMBLER
#define P1D_ASSEMBLER

#include <string>
#include "p1d_common.hpp"
#include "p1d_mesh.hpp"

namespace poisson1d {

class DistributedAssembler
{
    public:
        DistributedAssembler(const Mesh & mesh, const std::string & rhs);
        ~DistributedAssembler();

        void assemble_rhs(Real* rhs_ptr) const;

        void assemble_matrix(Real* matrix_ptr) const;

    private:
        const Mesh* mesh_ptr;
        std::string rhs_func;
        Real a;
        Real b;
        size_t n;
};

} //namespace poisson1d

#endif //P1D_ASSEMBLER
