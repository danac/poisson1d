/*
 * sink.cpp
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

#include "p1d_gatherer.hpp"
#include <cstdio>
#include <unistd.h>

using namespace poisson1d;

int main (int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Wrong number of arguments!\n");
        printf("Syntax: sink <input_port> <publishing_port>");
        return 1;
    }

    int input_port = atoi(argv[1]);
    int pub_port = atoi(argv[2]);

    Gatherer gatherer(input_port, pub_port);
    gatherer.gather();
    sleep (1);

    return 0;
}
