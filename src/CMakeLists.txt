include_directories(${ZeroMQ_INCLUDE_DIR})
include_directories(${MUPARSER_INCLUDE_DIRS})

add_executable(zmq_client client.cpp)
target_link_libraries(zmq_client ${ZeroMQ_LIBRARIES})

add_library(p1d_assembler p1d_assembler.cpp)
target_link_libraries(p1d_assembler ${MUPARSER_LIBRARIES})
