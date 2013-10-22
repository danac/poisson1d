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

def launch_cpp_execs(executables, ports, num_jobs):
    pids = dict()

    pids['sink'] = subp.Popen([executables['sink'], ports['sink'], ports['sink_pub']])
    print("[DRIVER] Spawned: ", [executables['sink'], ports['sink'], ports['sink_pub']])
    pids['workers'] = []
    for i in range(0, num_jobs):
        pids['workers'].append(subp.Popen([executables['workers'], ports['vent'], ports['sink'], ports['sink_pub']]))
        print("[DRIVER] Spawned: ", [executables['workers'], ports['vent'], ports['sink'], ports['sink_pub']])
    pids['vent'] = subp.Popen([executables['vent'], ports['driver'], ports['vent'], ports['sink']])
    print("[DRIVER] Spawned: ", [executables['vent'], ports['driver'], ports['vent'], ports['sink']])
    return pids

def kill_cpp_execs(pids):
    # Kill all Popen instances contained in the dictionary
    for key, val in pids.items():
        # There can be several workers
        if key == 'workers':
            for proc in val:
                proc.kill()
        else:
            val.kill()

def plot(x, rhs, solution):
    import matplotlib.pyplot as plt

    # Make the font slightly larger
    params = {
            'text.usetex': False,
            'font.family':'serif',
            'font.size':16,
            'legend.fontsize':16
         }

    plt.rcParams.update(params)

    fig, ax1 = plt.subplots()

    # First axis in red, displaying the solution vector
    ax1.plot(x, solution, 'r-')
    ax1.set_xlabel('x')
    ax1.set_ylabel("Solution", color='r')
    for tl in ax1.get_yticklabels():
        tl.set_color('r')

    # Second axis in blue, showing the right-hand side function
    ax2 = ax1.twinx()
    ax2.plot(x, rhs, 'b--')
    ax2.set_ylabel("RHS", color='b')
    for tl in ax2.get_yticklabels():
        tl.set_color('b')

    plt.grid(True)
    plt.show()

if __name__ == "__main__":

    ########### Parameters ###########
    ports = {
        # Port to which this Python script will bind to pass the parameters to the C++ part
        'driver'   : 14000,
        # Port to which the C++ "task ventilator" will bind to distribute the jobs
        'vent'     : 14100,
        # Port to which the C++ "sink" will bind to receive the job results
        'sink'     : 14200,
        # Port to which the C++ "sink" will bind to send the kill signal and publish the final solution
        'sink_pub' : 14201
    }

    # The main C++ programs
    executables = {
        'vent'    : "./ventilator",
        'sink'    : "./sink",
        'workers' : "./worker"
    }

    # The problem parameters
    a = 0
    b = 1
    n = 1000
    fa = 1
    fb = 10
    rhs_func = "100*x*x" # Expression for the righ-hand side
    num_jobs = 2

    #################################

    # Create an instance of the C++ Problem class using the binding
    mesh = p1d.Mesh(a, b, n)
    problem = p1d.Problem(mesh, fa, fb, rhs_func, num_jobs)

    # Convert the port numbers to strings for convenience
    ports = {key:str(val) for key,val in ports.items()}

    # Set-up a 0MQ socket to push the problem to the "task ventilator"
    context = zmq.Context()
    vent_socket = context.socket(zmq.PUSH)
    vent_socket.bind("tcp://*:" + ports['driver'])

    # Set-up another 0MQ socket to retrieve the solution form the "sink" (which also solves the problem)
    result_socket = context.socket(zmq.SUB)
    result_socket.setsockopt_string(zmq.SUBSCRIBE, "")
    result_socket.connect("tcp://localhost:" + ports['sink_pub'])

    # Spawn the C++ programs (i.e one ventilator, one or more workers and a sink)
    pids = launch_cpp_execs(executables, ports, num_jobs)

    # Pack the problem instance. The buffer is allocated by the C++ binding.
    buf = bytearray(problem.pack())

    # Send the buffer to the "ventilator"
    vent_socket.send(buf)

    # Allocate a Numpy array to hold the solution
    solution = np.zeros([n, 1], dtype=np.float64)

    # Boolean used to deal with Ctrl-C interrupts in the middle of the execution
    interrupt = False

    # Listen to the messages published by the sink
    while True:
        try:
            message = result_socket.recv()

            # Ignore the kill signal meant to shutdown the workers
            if message != b'KILL\x00':
                buf = bytearray(message)
                solution = np.frombuffer(buf, dtype=np.float64)
                break;

        # In case of an interrupt, kill the C++ processes
        except KeyboardInterrupt:
            interrupt = True
            kill_cpp_execs(pids)
            break;

    if not interrupt:
        # Create a function handle based on the expression for the right-hand side
        rhs_lambda = lambda x: eval(rhs_func)

        # Turn it into an optimized Numpy function handle
        rhs_f = np.vectorize(rhs_lambda)

        # Allocate arrays for the x variable and for the right-hand side function
        x = np.linspace(a, b, n)
        rhs = x
        rhs = rhs_f(rhs)

        # Plot the final results
        plot(x, rhs, solution)


