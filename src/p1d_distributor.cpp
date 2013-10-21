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

Distributor::Distributor(const Problem& problem, size_t port, const std::string& bind_host)
: problem_ptr(&problem), context(1), socket(context, ZMQ_PUSH)
{
    std::stringstream sstream;
    sstream << "tcp://" << bind_host << ":" << port;
    std::string bind_address = sstream.str();
    socket.bind(bind_address.c_str());
}

void Distributor::distribute()
{
    Partitioner partitioner(*problem_ptr);
    for (size_t job_id(0); job_id < problem_ptr->get_num_jobs(); ++job_id) {
        Job* job = partitioner.get_job_alloc(job_id);
        size_t job_size = job->get_packed_size();
        Byte* buffer = new Byte[job_size];
        job->pack(buffer);

        zmq::message_t msg(buffer, job_size, utils::dealloc_hook);
        socket.send(msg);
        delete job;
    }
}

} //namespace poisson1d
