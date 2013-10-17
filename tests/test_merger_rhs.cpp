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
    size_t num_rows[num_jobs];
    size_t offset[num_jobs];

    for(size_t i(0); i < num_jobs; ++i)
    {
        num_rows[i] = n/num_jobs;
        if((n % num_jobs) != 0 && i < (n % num_jobs))
        {
            num_rows[i] += 1;
            offset[i]= num_rows[i] * i;
        }
        else
        {
            offset[i] = (n % num_jobs) * (num_rows[i] + 1) + (i - (n % num_jobs)) * num_rows[i];
        }
    }

    Real array1[num_rows[0]];
    Real array2[num_rows[1]];
    Real array3[num_rows[2]];
    Real array4[num_rows[3]];
    Real array5[num_rows[4]];

    Merger merger(n, num_jobs);

    populate_array(array1, num_rows[0], 1.0);
    populate_array(array2, num_rows[1], 2.0);
    populate_array(array3, num_rows[2], 3.0);
    populate_array(array4, num_rows[3], 4.0);
    populate_array(array5, num_rows[4], 5.0);

    merger.merge_rhs(array1, 0);
    merger.merge_rhs(array2, 1);
    merger.merge_rhs(array3, 2);
    merger.merge_rhs(array4, 3);
    merger.merge_rhs(array5, 4);

    const Real* full_array = merger.get_rhs_ptr();
    //for(size_t i(0); i < n; ++i)
    //{
        //std::cout << "i=" << i << " val=" << full_array[i] << endl;
    //}
    //_array(full_array, n);
    for(size_t i(0); i < num_jobs; ++i)
    {
        for(size_t j(offset[i]); j < offset[i]+num_rows[i]; ++j)
        {
            assert(full_array[j] == (Real)(i+1));
        }
    }
}

int main(int argc, char* argv[])
{
    test_5_way_partitioning(100);
    test_5_way_partitioning(101);
    test_5_way_partitioning(102);
    test_5_way_partitioning(103);
    test_5_way_partitioning(104);

    return 0;
}
