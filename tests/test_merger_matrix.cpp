/*
 * test_merger_matrix.cpp
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
    size_t num_nnz[num_jobs];
    for(size_t i(0); i < num_jobs; ++i)
    {
        size_t num_rows = n/num_jobs;
        if((n % num_jobs) != 0 && i < (n % num_jobs))
        {
            num_rows += 1;
        }

        if(i == 0 || i == num_jobs-1)
        {
            num_nnz[i] = num_rows*3-2;
        }
        else
        {
            num_nnz[i] = num_rows * 3;
        }
    }

    Real array1[num_nnz[0]];
    Real array2[num_nnz[1]];
    Real array3[num_nnz[2]];
    Real array4[num_nnz[3]];
    Real array5[num_nnz[4]];

    Merger merger(n, num_jobs);

    populate_array(array1, num_nnz[0], 1.0);
    populate_array(array2, num_nnz[1], 2.0);
    populate_array(array3, num_nnz[2], 3.0);
    populate_array(array4, num_nnz[3], 4.0);
    populate_array(array5, num_nnz[4], 5.0);

    merger.merge_matrix(array1, 0);
    merger.merge_matrix(array2, 1);
    merger.merge_matrix(array3, 2);
    merger.merge_matrix(array4, 3);
    merger.merge_matrix(array5, 4);

    const Real* full_array = merger.get_matrix_ptr();
    size_t offset(0);
    for(size_t j(0); j < num_jobs; ++j)
    {
        offset = ( (j == 0) ? 0 : offset+num_nnz[j-1] );
        for(size_t i(offset); i < offset + num_nnz[j]; ++i)
        {
            assert(full_array[i] == Real(j)+1);
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
