/*
 * test_merger_matrix.cpp
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
    size_t num_nnz_first = n/num_jobs*3-2;
    size_t num_nnz = n/num_jobs*3;
    size_t num_nnz_last = (n/num_jobs+(n%num_jobs))*3-2;

    Real array1[num_nnz_first];
    Real array2[num_nnz];
    Real array3[num_nnz];
    Real array4[num_nnz];
    Real array5[num_nnz_last];

    Merger merger(n, num_jobs);

    populate_array(array1, num_nnz_first, 1.0);
    populate_array(array2, num_nnz, 2.0);
    populate_array(array3, num_nnz, 3.0);
    populate_array(array4, num_nnz, 4.0);
    populate_array(array5, num_nnz_last, 5.0);

    merger.merge_matrix(array1, 0);
    merger.merge_matrix(array2, 1);
    merger.merge_matrix(array3, 2);
    merger.merge_matrix(array4, 3);
    merger.merge_matrix(array5, 4);

    const Real* full_array = merger.get_matrix_ptr();

    size_t count(0);

    for(size_t i(0); i < num_nnz_first; ++i)
    {
        assert(full_array[i] == 1.);
    }
    for(size_t i(num_nnz_first); i < num_nnz; ++i)
    {
        assert(full_array[i] == 2.);
    }
    for(size_t i(num_nnz_first+num_nnz); i < num_nnz_first+num_nnz+num_nnz; ++i)
    {
        assert(full_array[i] == 3.);
    }
    for(size_t i(num_nnz_first+2*num_nnz); i < num_nnz_first+2*num_nnz+num_nnz; ++i)
    {
        assert(full_array[i] == 4.);
    }
    for(size_t i(num_nnz_first+3*num_nnz); i < num_nnz_first+3*num_nnz+num_nnz_last; ++i)
    {
        assert(full_array[i] == 5.);
    }

}

int main(int argc, char* argv[])
{
    test_5_way_partitioning(100);
    test_5_way_partitioning(101);

    return 0;
}
