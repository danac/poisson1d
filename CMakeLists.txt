project(akselos_assignment)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

find_package(ZeroMQ REQUIRED)
find_package(MuParser)
find_package(Boost REQUIRED COMPONENTS python)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
set(Eigen3_FIND_VERSION_MAJOR 3)
set(Eigen3_FIND_VERSION_MINOR 2)
find_package(Eigen3)
find_package(Matplotlib)
find_package(PyQt4)
enable_testing()

add_subdirectory(third-party)
add_subdirectory(src)
add_subdirectory(tests)
