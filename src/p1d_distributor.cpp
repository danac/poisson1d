/*
 * p1d_distributor.cpp
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

#include "p1d_distributor.hpp"
#include "p1d_partitioner.hpp"
#include "p1d_helper_functions.hpp"

#include <sstream>
#include <iostream>

namespace poisson1d {

Distributor::Distributor(const Problem& problem,
                         size_t port,
                         size_t sink_port,
                         const std::string& bind_host,
                         const std::string& sink_host)
: problem_ptr(&problem), context(1), socket(context, ZMQ_PUSH), sink_socket(context, ZMQ_PUSH)
{
    std::stringstream sstream;
    sstream << "tcp://" << bind_host << ":" << port;
    std::string bind_address = sstream.str();
    socket.bind(bind_address.c_str());

    std::stringstream sink_sstream;
    sink_sstream << "tcp://" << sink_host << ":" << sink_port;
    std::string sink_address = sink_sstream.str();
    sink_socket.connect(sink_address.c_str());
}

void Distributor::distribute()
{
    size_t num_jobs = problem_ptr->get_num_jobs();
    size_t n = problem_ptr->get_mesh().get_n();

    size_t start_msg_size = sizeof(num_jobs);
    zmq::message_t start_msg(start_msg_size);
    memcpy(start_msg.data(), &num_jobs, start_msg_size);
    sink_socket.send(start_msg);

    zmq::message_t start_msg2(start_msg_size);
    memcpy(start_msg2.data(), &n, start_msg_size);
    sink_socket.send(start_msg2);

    std::cout << "Sent problem info to sink!" << std::endl;

    Partitioner partitioner(*problem_ptr);
    for (size_t job_id(0); job_id < problem_ptr->get_num_jobs(); ++job_id) {
        Job* job = partitioner.get_job_alloc(job_id);
        size_t job_size = job->get_packed_size();
        Byte* buffer = new Byte[job_size];
        job->pack(buffer);

        zmq::message_t msg(buffer, job_size, utils::dealloc_hook);
        socket.send(msg);
        delete job;
        std::cout << "Sent one job: " << job_id << std::endl;
    }
}

} //namespace poisson1d
