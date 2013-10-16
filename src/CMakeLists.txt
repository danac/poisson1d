include_directories(${ZeroMQ_INCLUDE_DIR})
include_directories(${POISSON1D_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories($Boost_INCLUDE_DIRS})
add_executable(zmq_client client.cpp)
target_link_libraries(zmq_client ${ZeroMQ_LIBRARIES})

set(POISSON_1D_SRC
        p1d_assembler.cpp
        p1d_solver.cpp
        p1d_mesh.cpp
        p1d_mesh_iterator.cpp
        p1d_merger.cpp
        p1d_partitioner.cpp
)

option(BUILD_POISSON1D_SHARED "Build the poisson1d library shared." OFF)
option(BUILD_POISSON1D_SHARED "Build the poisson1d library shared." ON)

if(BUILD_POISSON1D_SHARED)
  set(POISSON1D_LIBRARY_TYPE SHARED)
else(BUILD_POISSON1D_SHARED)
  set(POISSON1D_LIBRARY_TYPE STATIC)
endif(BUILD_POISSON1D_SHARED)

add_library(poisson1d ${POISSON1D_LIBRARY_TYPE} ${POISSON_1D_SRC})
target_link_libraries(poisson1d ${MUPARSER_LIBRARIES})
