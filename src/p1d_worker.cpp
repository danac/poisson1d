/*
 * p1d_worker.cpp
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

#include "p1d_worker.hpp"
#include "p1d_assembler.hpp"
#include "p1d_helper_functions.hpp"

#include <sstream>
#include <iostream>

namespace poisson1d {

Worker::Worker(size_t input_port,
               size_t output_port,
               size_t control_port,
               const std::string& input_host,
               const std::string& output_host,
               const std::string& control_host)
: context(1),
  input_socket(context, ZMQ_PULL),
  output_socket(context, ZMQ_PUSH),
  control_socket(context, ZMQ_SUB)
{
    std::stringstream input_sstream;
    input_sstream << "tcp://" << input_host << ":" << input_port;
    std::string input_bind_address = input_sstream.str();
    input_socket.connect(input_bind_address.c_str());

    std::stringstream output_sstream;
    output_sstream << "tcp://" << output_host << ":" << output_port;
    std::string output_bind_address = output_sstream.str();
    output_socket.connect(output_bind_address.c_str());

    std::stringstream control_sstream;
    control_sstream << "tcp://" << control_host << ":" << control_port;
    std::string control_bind_address = control_sstream.str();
    control_socket.connect(control_bind_address.c_str());
    control_socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

void Worker::work()
{
    zmq::pollitem_t poll_items [] = {
        { input_socket, 0, ZMQ_POLLIN, 0 },
        { control_socket, 0, ZMQ_POLLIN, 0 }
    };

    while(true)
    {
        zmq::message_t message;
        zmq::poll (&poll_items[0], 2, -1);

        if (poll_items[0].revents & ZMQ_POLLIN) {
            input_socket.recv(&message);

            Byte* input_buffer = static_cast<Byte*>(message.data());
            Job job;
            job.unpack(input_buffer);

            //std::cout << "Received a job: " << job.get_rank() << std::endl;

            DistributedAssembler assembler(job);

            JobResult* job_result = assembler.get_job_result_alloc();
            size_t job_result_size = job_result->get_packed_size();
            Byte* output_buffer = new Byte[job_result_size];
            job_result->pack(output_buffer);

            zmq::message_t msg(output_buffer, job_result_size, utils::dealloc_hook);
            output_socket.send(msg);

            //std::cout << "Sending a job result: " << job_result->get_rank() << std::endl;

            delete job_result;
        }
        if (poll_items[1].revents & ZMQ_POLLIN)
        {
            break;                      //  Exit loop
        }
    }
}
} //namespace poisson1d
