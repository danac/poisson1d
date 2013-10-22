/*
 * ventilator.cpp
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

#include <zmq.hpp>
#include <unistd.h>
#include <string>

#include "p1d_structs.hpp"
#include "p1d_mesh.hpp"
#include "p1d_distributor.hpp"

using namespace poisson1d;

int main (int argc, char *argv[])
{
    Real a(0.0);
    Real b(13.013);
    Real fa(1);
    Real fb(1);
    size_t n(5);
    std::string rhs_func("1");
    size_t num_jobs(2);

    Mesh mesh(a, b, n);
    Problem problem(mesh, fa, fb, rhs_func, num_jobs);

    Distributor distributor(problem, 13000, 13100);

    distributor.distribute();
    sleep(1);
    return 0;
}
