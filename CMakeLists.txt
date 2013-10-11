project(akselos_assignment)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(ZeroMQ REQUIRED)
find_package(MuParser)
find_package(Boost COMPONENTS python REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Matplotlib)
find_package(PyQt4)
enable_testing()

add_subdirectory(third-party)
add_subdirectory(src)
add_subdirectory(tests)
