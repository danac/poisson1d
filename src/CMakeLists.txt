include_directories(${ZeroMQ_INCLUDE_DIR})
include_directories(${MUPARSER_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(zmq_client client.cpp)
target_link_libraries(zmq_client ${ZeroMQ_LIBRARIES})

set(poisson1d_src
        p1d_assembler.cpp
        p1d_solver.cpp
        p1d_mesh.cpp
)

add_library(poisson1d ${poisson1d_src})
target_link_libraries(poisson1d ${MUPARSER_LIBRARIES})
