/*
 * test_merger_rhs.cpp
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

#include "p1d_merger.hpp"
#include <cassert>
#include <iostream>

using namespace poisson1d;
using namespace std;

void populate_array(Real* array, size_t size, Real value)
{
    for(size_t i(0); i < size; ++i)
    {
        array[i] = value;
    }
}

void dump_array(const Real* array, size_t size)
{
    for(size_t i(0); i < size; ++i)
    {
        cout << i << " " << array[i] << endl;
    }
    cout << endl;
}

void test_5_way_partitioning(size_t n)
{
    size_t num_jobs(5);
    size_t num_rows = n/num_jobs;
    size_t num_rows_last = num_rows + (n % num_jobs);

    Real array1[num_rows];
    Real array2[num_rows];
    Real array3[num_rows];
    Real array4[num_rows];
    Real array5[num_rows_last];

    Merger merger(n, num_jobs);

    populate_array(array1, num_rows, 1.0);
    populate_array(array2, num_rows, 2.0);
    populate_array(array3, num_rows, 3.0);
    populate_array(array4, num_rows, 4.0);
    populate_array(array5, num_rows_last, 5.0);

    merger.merge_rhs(array1, 0);
    merger.merge_rhs(array2, 1);
    merger.merge_rhs(array3, 2);
    merger.merge_rhs(array4, 3);
    merger.merge_rhs(array5, 4);

    const Real* full_array = merger.get_rhs_ptr();
    //_array(full_array, n);
    for(size_t i(0); i < num_jobs; ++i)
    {
        for(size_t j(i*num_rows); j < (i+1)*num_rows; ++j)
        {
            assert(full_array[j] == (Real)(i+1));
        }
    }
    if(n % num_jobs)
    {
        assert(full_array[num_jobs*num_rows] == num_jobs);
    }
}

int main(int argc, char* argv[])
{
    test_5_way_partitioning(100);
    test_5_way_partitioning(101);

    return 0;
}
