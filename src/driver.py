#!/usr/bin/env python

#  driver.py
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

from p1d_driver import P1D_Driver

if __name__ == "__main__":

    # The problem parameters
    parameters = {
        'a'        : 0,
        'b'        : 1,
        'n'        : 100,
        'fa'       : 1,
        'fb'       : 1,
        'rhs_func' : "-5*sin(_pi*x)", # Expression for the righ-hand side
        'num_jobs' : 2
    }

    # The ports to which this script and C++ programs will bind
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

    # The main C++ executables (relative to this script location)
    # sink-nosolve does not solve and returns an array filled with zeros instead
    executables = {
        'vent'    : "./ventilator",
        'sink'    : "./sink",
        #'sink'    : "./sink-nosolve",
        'workers' : "./worker"
    }

    driver = P1D_Driver(parameters=parameters, ports=ports, executables=executables)

    driver.run()

    print("[DRIVER] Assembly time: {} milliseconds".format(driver.assembly_time))

    driver.plot()

