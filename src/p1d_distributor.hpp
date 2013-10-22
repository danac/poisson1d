/*
 * p1d_distributor.hpp
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

#ifndef P1D_DISTRIBUTOR
#define P1D_DISTRIBUTOR

#include "p1d_common.hpp"
#include "p1d_structs.hpp"
#include <zmq.hpp>
#include <string>

namespace poisson1d {

class Distributor
{
    public:
        Distributor(const Problem& problem,
                    size_t port,
                    size_t sink_port,
                    const std::string& bind_host= "*",
                    const std::string& sink_host = "localhost");

        void distribute();

    private:
        const Problem* problem_ptr;

        zmq::context_t context;
        zmq::socket_t socket;
        zmq::socket_t sink_socket;

};

} //namespace poisson1d

#endif
