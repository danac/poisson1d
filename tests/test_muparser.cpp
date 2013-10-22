/*
 * test_muparser.cpp
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

#include "muParserTest.h"

int main(int argc, char* argv[])
{
    mu::console() << _T( "-----------------------------------------------------------\n");
    mu::console() << _T( "Running test suite:\n\n");

    // Skip the self test if the value type is set to an integer type.
    if (mu::TypeInfo<mu::value_type>::IsInteger())
    {
        mu::console() << _T( "  Test skipped: integer data type are not compatible with the unit test!\n\n");
    }
    else
    {
        mu::Test::ParserTester pt;
        pt.Run();
    }

    return 0;
}
