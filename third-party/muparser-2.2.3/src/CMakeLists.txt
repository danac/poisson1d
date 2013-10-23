include_directories(../include)

file(GLOB muParser_src "*.cpp*")

add_library(muparser SHARED ${muParser_src})
install(TARGETS muparser LIBRARY DESTINATION ${POISSON1D_INSTALL_LIB_DIR})
