/*
 * p1d_worker.hpp
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

#ifndef P1D_WORKER
#define P1D_WORKER

#include "p1d_common.hpp"
#include "p1d_structs.hpp"
#include <zmq.hpp>

namespace poisson1d {

class Worker
{
    public:
        Worker(size_t input_port,
               size_t output_port,
               size_t control_port,
               const std::string& input_host = "localhost",
               const std::string& output_host = "localhost",
               const std::string& control_host = "localhost");
        void work();

    private:
        zmq::context_t context;
        zmq::socket_t input_socket;
        zmq::socket_t output_socket;
        zmq::socket_t control_socket;
};

} //namespace poisson1d

#endif

