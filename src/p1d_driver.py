#!/usr/bin/env python

#  p1d_driver.py
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
#

import sys, inspect, os
# Hack to find a module in an arbitrary location (py_poisson1d.so is in ../lib)
current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
sys.path.insert(0, os.path.join(current_dir, '..', 'lib'))

import zmq
import struct
import six
import numpy as np
import subprocess as subp
import py_poisson1d as p1d

# Import math functions supported by muParser
from numpy import sin,cos,tan,sinh,cosh,tanh,log2,log10,log,exp,sqrt,sign,rint,abs,min,max,sum,pi
from math import asin,acos,atan,asinh,acosh,atanh

class P1D_Driver:

    def __init__(self, parameters, executables, ports):

        """ Attributes """
        # Dictionary holding references to the spawned processes
        self.pids = {}
        # Dictionary holding the ports the programs must bind to
        self.ports = ports
        # Dictionary holding the paths to the C++ executables relative to this script
        self.executables = executables
        # Dictionary holding the problem parameters
        self.parameters = parameters
        # 0MQ socket used to communicate with the C++ "task ventilator"
        self.vent_socket = None
        # 0MQ socket used to retrive the solution from the C++ "sink"
        self.result_socket = None

        # Future buffer containing the solution
        self.solution_buffer = None

        # Total time spent by the C++ processes assembling the finite-elements matrix
        self.assembly_time = 0

        self.solving = True

        if "nosolve" in self.executables['sink']:
            self.solving = False

        """ Initialization """
        # Convert the port numbers to strings for convenience
        self.ports = {key:str(val) for key,val in self.ports.items()}

        # Initialize a 0MQ context
        context = zmq.Context()

        # Set-up a 0MQ socket to push the problem to the "task ventilator"
        self.vent_socket = context.socket(zmq.PUSH)
        self.vent_socket.bind("tcp://*:" + self.ports['driver'])

        # Set-up another 0MQ socket to retrieve the solution form the "sink" (which also solves the problem)
        self.result_socket = context.socket(zmq.SUB)
        self.result_socket.setsockopt_string(zmq.SUBSCRIBE, six.u("")) # Hack for Python2 caompatibility
        self.result_socket.connect("tcp://localhost:" + self.ports['sink_pub'])

    # Function used to start the C++ programsa
    def launch_cpp_execs(self):

        current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))

        # Launch the C++ processes based on the given program names and ports
        # "Sink" process
        sink_command = [os.path.join(current_dir, self.executables['sink']), self.ports['sink'], self.ports['sink_pub']]
        sink_process = subp.Popen(sink_command)
        self.pids['sink'] = sink_process
        print("[DRIVER] Spawned ventilator, PID {}".format(sink_process.pid))

        # "Worker" processes (there may be several of them)
        self.pids['workers'] = []
        worker_comand = [os.path.join(current_dir, self.executables['workers']), self.ports['vent'], self.ports['sink'], self.ports['sink_pub']]
        for i in range(0, self.parameters['num_jobs']):
            worker_process = subp.Popen(worker_comand)
            self.pids['workers'].append(worker_process)
            print("[DRIVER] Spawned worker {}, PID {}".format(i, worker_process.pid))

        # "Ventilator" process
        vent_command = [os.path.join(current_dir, self.executables['vent']), self.ports['driver'], self.ports['vent'], self.ports['sink']]
        vent_process = subp.Popen(vent_command)
        self.pids['vent'] = vent_process
        print("[DRIVER] Spawned ventilator, PID {}".format(vent_process.pid))

    # Function used to kill the C++ programs in case of interruption
    def kill_cpp_execs(self):
        # Kill all Popen instances contained in the dictionary
        for key, val in self.pids.items():
            # There can be several workers
            if key == 'workers':
                for proc in val:
                    proc.kill()
            else:
                val.kill()

    def run(self):
        a = self.parameters['a']
        b = self.parameters['b']
        n = self.parameters['n']
        fa = self.parameters['fa']
        fb = self.parameters['fb']
        rhs_func = self.parameters['rhs_func']
        num_jobs = self.parameters['num_jobs']

        # Create an instance of the C++ Problem class using the binding
        mesh = p1d.Mesh(a, b, n)
        problem = p1d.Problem(mesh, fa, fb, rhs_func, num_jobs)

        # Spawn the C++ programs (i.e one ventilator, one or more workers and a sink)
        self.launch_cpp_execs()

        # Pack the problem instance. The buffer is allocated by the C++ binding.
        buf = bytearray(problem.pack())

        # Send the buffer to the "ventilator"
        self.vent_socket.send(buf)

        # Allocate a Numpy array to hold the solution
        self.solution = np.zeros([n, 1], dtype=np.float64)

        # Boolean used to deal with Ctrl-C interrupts in the middle of the execution
        interrupt = False

        # Listen to the messages published by the sink
        while True:
            try:
                message = self.result_socket.recv()

                # Ignore the kill signal meant to shutdown the workers
                if message != b'KILL\x00':
                    # We actually receive a multipart message
                    # The first part is the assembly time...
                    self.assembly_time = struct.unpack('d', message)[0]
                    # ...and the second part is the solution vector
                    self.solution_buffer = self.result_socket.recv()
                    break;

            # In case of an interrupt, kill the C++ processes
            except KeyboardInterrupt:
                interrupt = True
                self.kill_cpp_execs(pids)
                break;

    def plot(self):
        if self.solving:
            a = self.parameters['a']
            b = self.parameters['b']
            n = self.parameters['n']
            rhs_func = self.parameters['rhs_func']

            # Trick to get the size of a double on the machine
            dtype = eval("np.float"+str(struct.calcsize('d')*8))
            solution = np.frombuffer(self.solution_buffer, dtype=dtype)

            # Allocate arrays for the x variable and for the right-hand side function
            x = np.linspace(a, b, n)
            rhs = self.get_rhs(x)

            # Plot the final results
            self.draw(x, rhs, solution)
        else:
            print("[DRIVER] Nothing to plot because the problem was not solved, only assembled.")

    # Function used to plot the solution and right-hand side
    @staticmethod
    def draw(x, rhs, solution):
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

    def get_rhs(self, x):

        # Fix the logarithm function names
        rhs_func_parsed = ( self.parameters['rhs_func']
            .replace('log', 'log10')
            .replace('ln', 'log')
            .replace('_pi', 'pi') )

        # Create a function handle based on the expression
        rhs_lambda = lambda x: eval(rhs_func_parsed)

        # Turn it into an optimized Numpy function handle
        rhs_f = np.vectorize(rhs_lambda)
        rhs = rhs_f(x)

        return rhs
