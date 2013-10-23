include_directories(${ZeroMQ_INCLUDE_DIR})
include_directories(${POISSON1D_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories($Boost_INCLUDE_DIRS})
include_directories(${ZMQ_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})

set(POISSON_1D_SRC
        p1d_assembler.cpp
        p1d_solver.cpp
        p1d_mesh.cpp
        p1d_mesh_iterator.cpp
        p1d_merger.cpp
        p1d_partitioner.cpp
        p1d_structs.cpp
        p1d_helper_functions.cpp
        p1d_distributor.cpp
        p1d_worker.cpp
        p1d_gatherer.cpp
)

add_library(poisson1d SHARED ${POISSON_1D_SRC})
target_link_libraries(poisson1d ${MUPARSER_LIBRARIES} ${ZeroMQ_LIBRARIES})
set_property(TARGET poisson1d PROPERTY INSTALL_RPATH .)
install(TARGETS poisson1d DESTINATION .)

macro(add_poisson1d_executable name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} poisson1d)
    set_property(TARGET ${name} PROPERTY INSTALL_RPATH .)
    install(TARGETS ${name} DESTINATION .)
endmacro()

add_poisson1d_executable(ventilator)
add_poisson1d_executable(worker)
add_poisson1d_executable(sink)

PYTHON_ADD_MODULE(py_poisson1d p1d_python_binding.cpp)
target_link_libraries(py_poisson1d poisson1d)
target_link_libraries(py_poisson1d ${Boost_LIBRARIES})
set_property(TARGET py_poisson1d PROPERTY INSTALL_RPATH .)
install(TARGETS py_poisson1d DESTINATION .)

file(COPY p1d_driver.py driver.py DESTINATION .)
install(FILES p1d_driver.py driver.py DESTINATION .)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
install(FILES ${headers} DESTINATION include)
