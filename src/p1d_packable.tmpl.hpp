/*
 * p1d_packable.tmpl.hpp
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

#include "p1d_packable.hpp"
#include "p1d_common.hpp"
#include <string>

namespace poisson1d {

template<>
inline Byte* Packable::write_to_buffer(const std::string& var, Byte* const buffer) const
{
    Byte* cursor = buffer;
    const Byte * cast_value_ptr = var.c_str();
    size_t value_size = var.size()+1;
    memcpy(cursor, cast_value_ptr, value_size);
    cursor += value_size;
    return cursor;
}

template<>
inline const Byte* Packable::read_from_buffer(std::string& var, const Byte* const buffer) const
{
    const Byte* cursor = buffer;
    var = std::string(cursor);
    cursor += var.size()+1;
    return cursor;
}

} //namespace poisson1d
