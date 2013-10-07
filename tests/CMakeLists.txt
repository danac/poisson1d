include_directories(${Boost_INCLUDE_DIRS}} ${PYTHON_INCLUDE_DIRS} ${MUPARSER_INCLUDE_DIRS})

macro(add_cpp_test name)
add_executable(${name} ${name}.cpp)
add_test(${name} ${name})
endmacro()

python_add_module(test_boost_python test_boost_python.cpp)
file(COPY test_boost_python.py DESTINATION .)
add_test(test_boost_python test_boost_python.py)
target_link_libraries(test_boost_python ${Boost_LIBRARIES})

add_cpp_test(test_zeromq)
target_link_libraries(test_zeromq ${ZeroMQ_LIBRARIES})

add_cpp_test(test_muparser)
target_link_libraries(test_muparser ${MUPARSER_LIBRARIES})
