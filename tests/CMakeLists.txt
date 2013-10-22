option(BUILD_POISSON1D_TESTS "Build the tests." ON)
option(BUILD_POISSON1D_TESTS "Build the tests." OFF)

if(BUILD_POISSON1D_TESTS)

    include_directories(${Boost_INCLUDE_DIRS}} ${PYTHON_INCLUDE_DIRS} ${MUPARSER_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})
    include_directories(${CMAKE_SOURCE_DIR}/src)

    python_add_module(test_boost_python test_boost_python.cpp)
    file(COPY test_boost_python.py DESTINATION .)
    add_test(test_boost_python test_boost_python.py)
    target_link_libraries(test_boost_python ${Boost_LIBRARIES})

    add_executable(test_zeromq test_zeromq.cpp)
    add_test(test_zeromq test_zeromq)
    target_link_libraries(test_zeromq ${ZeroMQ_LIBRARIES})

    macro(add_cpp_test name)
    add_executable(${name} ${name}.cpp)
    add_test(${name} ${name})
    target_link_libraries(${name} poisson1d)
    endmacro()

    macro(add_py_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} poisson1d)
    file(COPY ${name}.py DESTINATION .)
    add_test(NAME ${name} COMMAND ${name}.py)
    endmacro()

    add_cpp_test(test_muparser)

    add_cpp_test(test_mesh)

    add_cpp_test(test_mesh_packing)

    add_cpp_test(test_assembler_matrix)

    add_cpp_test(test_assembler_rhs)

    add_cpp_test(test_solver)

    add_cpp_test(test_merger_rhs)

    add_cpp_test(test_merger_matrix)

    add_cpp_test(test_partitioner)

    add_cpp_test(test_problem)

    add_cpp_test(test_solution)

    add_cpp_test(test_job)

    add_cpp_test(test_job_result)

    add_py_test(test_python_binding)

endif(BUILD_POISSON1D_TESTS)
