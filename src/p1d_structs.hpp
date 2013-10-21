/*
 * p1d_structs.hpp
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

#ifndef P1D_STRUCTS
#define P1D_STRUCTS

#include "p1d_common.hpp"
#include "p1d_mesh.hpp"
#include <string>

namespace poisson1d {

class Problem : public Packable {

    public:
        Problem(Mesh& mesh, Real fa, Real fb, std::string rhs_func, size_t num_jobs, bool ownership = false);
        Problem();
        ~Problem();

        virtual Byte* pack(Byte* const buffer) const;
        virtual const Byte* unpack(const Byte* buffer);
        virtual size_t get_packed_size() const;

        bool operator==(const Problem& other) const;

        const Mesh& get_mesh() const;
        Real get_fa() const;
        Real get_fb() const;
        std::string get_rhs_func() const;
        size_t get_num_jobs() const;

    private:
        Mesh* mesh_ptr;
        Real fa;
        Real fb;
        std::string rhs_func;
        size_t num_jobs;
        bool data_ownership;
};

class Job : public Packable {

    public:
        Job(Problem& problem, size_t rank, bool ownership = false);
        Job();
        ~Job();

        virtual Byte* pack(Byte* const buffer) const;
        virtual const Byte* unpack(const Byte* buffer);
        virtual size_t get_packed_size() const;

        bool operator==(const Job& other) const;

        const Problem& get_problem() const;
        size_t get_rank() const;

    private:
        Problem* problem_ptr;
        size_t rank;
        bool data_ownership;
};

class JobResult : Packable {

    public:
        JobResult(Real* matrix_ptr_, Real* rhs_ptr_, size_t rank, size_t nnz, size_t n, bool ownership = false);
        JobResult();
        ~JobResult();

        virtual Byte* pack(Byte* const buffer) const;
        virtual const Byte* unpack(const Byte* buffer);
        virtual size_t get_packed_size() const;

        bool operator==(const JobResult& other) const;

        Real* get_rhs_ptr() const;
        Real* get_matrix_ptr() const;
        size_t get_rank() const;
        size_t get_n() const;
        size_t get_nnz() const;


    private:
        Real* rhs_ptr;
        Real* matrix_ptr;
        size_t nnz;
        size_t n;
        size_t rank;
        bool data_ownership;
};

class Solution : public Packable {

    public:
        Solution(Real* solution_ptr, size_t n, bool ownership = false);
        Solution();
        ~Solution();

        virtual Byte* pack(Byte* const buffer) const;
        virtual const Byte* unpack(const Byte* buffer);
        virtual size_t get_packed_size() const;

        bool operator==(const Solution& other) const;

        const Real* get_x_ptr() const;

    private:
        Real* x_ptr;
        size_t n;
        bool data_ownership;
};

} //namespace poisson1d

#endif
