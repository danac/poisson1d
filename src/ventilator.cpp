/*
 * ventilator.cpp
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
#include <unistd.h>
#include <cstdio>

#include "p1d_structs.hpp"
#include "p1d_distributor.hpp"

using namespace poisson1d;

int main (int argc, char *argv[])
{
    if(argc < 4)
    {
        printf("Wrong number of arguments!\n");
        printf("Syntax: ventilator <driver_port> <output_port> <sink_port>");
        return 1;
    }

    int driver_port = atoi(argv[1]);
    int output_port = atoi(argv[2]);
    int sink_port = atoi(argv[3]);

    Distributor distributor(driver_port, output_port, sink_port);
    distributor.acquire();
    distributor.distribute();

    sleep(1);
    return 0;
}
