/*
 * p1d_packable.hpp
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


#ifndef P1D_PACKABLE
#define P1D_PACKABLE

#include <cstring>

namespace poisson1d {

class Packable
{
    public:
        virtual ~Packable() {}
        virtual Byte* pack(Byte* const buffer) const = 0;
        virtual const Byte* unpack(const Byte* const buffer) = 0;
        virtual size_t get_packed_size() const = 0;

    protected:
        template<typename T>
        inline Byte* write_to_buffer(const T& var, Byte* const buffer) const
        {
            Byte* cursor = buffer;
            T * cast_cursor = reinterpret_cast<T*>(cursor);
            *cast_cursor = var;
            cursor += sizeof(T);
            return cursor;
        }

        template<typename T>
        inline const Byte* read_from_buffer(T& var, const Byte* const buffer) const
        {
            const Byte* cursor = buffer;
            const T* cast_cursor = reinterpret_cast<const T*>(cursor);
            var = *cast_cursor;
            cursor += sizeof(T);
            return cursor;
        }

};

//inline std::ostream& operator<<(std::ostream& stream, const Packable& serializable)
//{
    //serializable.pack(stream);
    //return stream;
//}

//inline std::istream& operator>>(std::istream& stream, Packable& serializable)
//{
    //serializable.unpack(stream);
    //return stream;
//}

} //namespace poisson1d

#include "p1d_packable.tmpl.hpp"

#endif
