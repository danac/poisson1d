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
#include <unistd.h>

namespace poisson1d {

Distributor::Distributor(size_t input_port,
                         size_t output_port,
                         size_t sink_port,
                         const std::string& input_host,
                         const std::string& output_bind_host,
                         const std::string& sink_host)
: problem_ptr(NULL),
  context(1),
  input_socket(context, ZMQ_PULL),
  output_socket(context, ZMQ_PUSH),
  sink_socket(context, ZMQ_PUSH)
{
    std::stringstream input_sstream;
    input_sstream << "tcp://" << input_host << ":" << input_port;
    std::string input_address = input_sstream.str();
    input_socket.connect(input_address.c_str());

    std::stringstream output_sstream;
    output_sstream << "tcp://" << output_bind_host << ":" << output_port;
    std::string output_bind_address = output_sstream.str();
    output_socket.bind(output_bind_address.c_str());

    std::stringstream sink_sstream;
    sink_sstream << "tcp://" << sink_host << ":" << sink_port;
    std::string sink_address = sink_sstream.str();
    sink_socket.connect(sink_address.c_str());
    sleep(1);


    //std::cerr << "[DISTRIBUTOR] " << input_sstream.str() << " " << output_sstream.str() << " ";
    //std::cerr << sink_sstream.str() << std::endl;
}

Distributor::~Distributor()
{
    delete problem_ptr;
}

void Distributor::acquire()
{
    zmq::message_t msg;

    input_socket.recv(&msg);
    Byte* input_buffer = static_cast<Byte*>(msg.data());
    Problem* problem = new Problem();
    problem->unpack(input_buffer);
    problem_ptr = problem;
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

    //std::cout << "Sent problem info to sink!" << std::endl;

    Partitioner partitioner(*problem_ptr);
    for (size_t job_id(0); job_id < problem_ptr->get_num_jobs(); ++job_id) {
        Job* job = partitioner.get_job_alloc(job_id);
        size_t job_size = job->get_packed_size();
        Byte* buffer = new Byte[job_size];
        job->pack(buffer);

        zmq::message_t msg(buffer, job_size, utils::dealloc_hook);
        output_socket.send(msg);
        delete job;
        //std::cout << "Sent one job: " << job_id << std::endl;
    }
}

} //namespace poisson1d
