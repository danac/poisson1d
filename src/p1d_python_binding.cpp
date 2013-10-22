/*
 * p1d_python_binding.cpp
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

#include <boost/python.hpp>
#include "p1d_structs.hpp"

using namespace boost::python;
using namespace poisson1d;

template<typename T>
PyObject* pack_into_pybuffer(const T& self)
{
    size_t size = self.get_packed_size();
    Byte* buffer = new Byte[size];
    self.pack(buffer);

    void* buff = static_cast<void*>(buffer);
    Py_buffer pybuffer;
    int res = PyBuffer_FillInfo(&pybuffer, 0, buff, size, false, PyBUF_CONTIG);
    if (res == -1)
        return NULL;
    return PyMemoryView_FromBuffer(&pybuffer);
}


BOOST_PYTHON_MODULE(py_poisson1d)
{
    enum_<MeshGlobalPosition>("MeshGlobalPosition")
        .value("_left", _left)
        .value("_right", _right)
        .value("_middle", _middle)
        .value("_full", _full)
        .value("_undefined", _undefined)
        ;

    class_<Mesh>("Mesh", init<Real, Real, size_t>())
        .def("get_packed_size", &Mesh::get_packed_size)
        .def("pack", &pack_into_pybuffer<Mesh>, (arg("self")))
    ;

    class_<Problem>("Problem", init<Mesh&, Real, Real, std::string, size_t>())
        .def("get_packed_size", &Problem::get_packed_size)
        .def("pack", &pack_into_pybuffer<Problem>, (arg("self")))
    ;
};
