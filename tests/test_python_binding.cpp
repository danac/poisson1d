/*
 * test_python_binding.cpp
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

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include "p1d_structs.hpp"

using namespace poisson1d;

int main(int argc, char* argv[])
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);


    std::stringstream sstream;
    sstream << "tcp://" << "localhost" << ":" << atoi(argv[1]);
    std::string connect_address = sstream.str();

    std::cout << "Connecting to the Python server…" << std::endl;
    socket.connect(connect_address.c_str());

    zmq::message_t request(1);
    memcpy((void *)request.data(), "", 1);
    std::cout << "Sending request... " << std::endl;
    socket.send(request);

    zmq::message_t reply;
    socket.recv (&reply);
    Problem problem;
    Byte* buffer = static_cast<Byte*>(reply.data());
    problem.unpack(buffer);

    Real b = problem.get_mesh().get_b();
    Real b_ref = atof(argv[2]);

    std::cout << "Received problem! " << std::endl;
    std::cout << "b boundary: obtained " << b << ", expected " << b_ref << std::endl;

    if(b != b_ref)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
