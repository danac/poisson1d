#!/usr/bin/env python

#  driver.py
#
#  Copyright 2013 Dana Christen <dana.christen@gmail.com>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#

import zmq
import py_poisson1d as p1d
import numpy as np
import subprocess as subp

def launch(program):
    pass

if __name__ == "__main__":

    a = 0
    b = 13.013013
    n = 10
    fa = 1
    fb = 2
    rhs_func = "x"
    num_jobs = 2

    mesh = p1d.Mesh(a, b, n)
    problem = p1d.Problem(mesh, fa, fb, rhs_func, num_jobs)
    buf = bytearray(problem.pack())

    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:6000")

    message = socket.recv()
    print("Received request! Sending the packed problem...")

    socket.send(buf)

    x = np.array()

    while True:
        message = socket.recv()
        if message != b'KILL\x00':
            buf = bytearray(message)
            x = np.frombuffer(buf, dtype=np.float64)
            break;

    print(x)
