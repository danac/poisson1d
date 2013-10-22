#!/usr/bin/env python

#  test_python_binding.py
#
#  Copyright 2013 Dana Christen <dana.christen@gmail.com>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
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
import sys, inspect, os
current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
sys.path.insert(0, os.path.join(current_dir, '..', 'src'))

import zmq
import py_poisson1d as p1d
import numpy as np
import subprocess as subp

if __name__ == "__main__":

    a = 0
    b = 13.013013
    n = 10
    fa = 1
    fb = 2
    rhs_func = "x"
    num_jobs = 2

    bind_port = 6000

    cpp_exec = os.path.join(current_dir, "test_python_binding")
    cpp_proc = subp.Popen([cpp_exec, str(bind_port), str(b)])

    mesh = p1d.Mesh(a, b, n)
    problem = p1d.Problem(mesh, fa, fb, rhs_func, num_jobs)
    buf = bytearray(problem.pack())

    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:{}".format(bind_port))

    message = socket.recv()
    print("Received request! Sending the packed problem...")

    socket.send(buf)

    if cpp_proc.wait() != 0:
        raise Exception
