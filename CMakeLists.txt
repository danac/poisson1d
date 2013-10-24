project(akselos_assignment)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wall" CACHE STRING "Specify additional C++ compiler flags." FORCE)
endif()

#if(NOT CMAKE_BUILD_TYPE)
  #set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      #"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      #FORCE)
#endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_INSTALL_PREFIX MATCHES "/usr/local")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install CACHE STRING "Install path prefix, prepended onto install  directories." FORCE)
endif(CMAKE_INSTALL_PREFIX MATCHES "/usr/local")

set(POISSON1D_INSTALL_LIB_DIR "lib")
set(POISSON1D_INSTALL_BIN_DIR "bin")

find_package(ZeroMQ REQUIRED)
find_package(MuParser)
find_package(Boost REQUIRED COMPONENTS python)
find_package(PythonInterp REQUIRED)
set(PythonLibs_FIND_VERSION ${PYTHON_VERSION_STRING})
find_package(PythonLibs REQUIRED)
set(Eigen3_FIND_VERSION_MAJOR 3)
set(Eigen3_FIND_VERSION_MINOR 2)
find_package(Eigen3)
find_package(Matplotlib)
#find_package(PyQt4)

enable_testing()

add_subdirectory(third-party)
add_subdirectory(src)
add_subdirectory(tests)

message(STATUS "Installation prefix set to: ${CMAKE_INSTALL_PREFIX}")
