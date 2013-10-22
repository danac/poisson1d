/*
 * p1d_gatherer.cpp
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

#include "p1d_gatherer.hpp"
#include "p1d_merger.hpp"
#include "p1d_solver.hpp"

#include <iostream>
#include <sstream>

namespace poisson1d {

Gatherer::Gatherer(size_t input_port,
                   size_t control_port,
                   const std::string& bind_host,
                   const std::string& control_host)
: context(1), input_socket(context, ZMQ_PULL), control_socket(context, ZMQ_PUB)
{
    std::stringstream sstream;
    sstream << "tcp://" << bind_host << ":" << input_port;
    std::string bind_address = sstream.str();
    input_socket.bind(bind_address.c_str());

    std::stringstream control_sstream;
    control_sstream << "tcp://" << control_host << ":" << control_port;
    std::string control_bind_address = control_sstream.str();
    control_socket.bind(control_bind_address.c_str());
}

void Gatherer::gather()
{
    zmq::message_t start_msg;
    zmq::message_t start_msg2;

    input_socket.recv(&start_msg);
    size_t* input_buffer = static_cast<size_t*>(start_msg.data());
    size_t num_jobs = *input_buffer;

    input_socket.recv(&start_msg2);
    input_buffer = static_cast<size_t*>(start_msg2.data());
    size_t n = *input_buffer;

    std::cout << "Received problem info: " << num_jobs << " " << n << std::endl;

    Merger merger(n, num_jobs);
    for(size_t i(0); i < num_jobs; ++i)
    {
        zmq::message_t msg;
        input_socket.recv(&msg);

        Byte* input_buffer = static_cast<Byte*>(msg.data());
        JobResult job_result;
        job_result.unpack(input_buffer);

        merger.merge_job_result(job_result);
        std::cout << "Received a job result: " << i << std::endl;
    }

    zmq::message_t kill_msg(5);
    memcpy(kill_msg.data(), "KILL", 5);
    control_socket.send(kill_msg);
    std::cout << "Published kill signal to workers" << std::endl;

    const Real* matrix_ptr = merger.get_matrix_ptr();
    const Real* rhs_ptr = merger.get_rhs_ptr();

    Solver solver(matrix_ptr, rhs_ptr, n);
    Solution* solution = solver.get_solution_alloc();

    delete solution;
}


} //namespace poisson1d
