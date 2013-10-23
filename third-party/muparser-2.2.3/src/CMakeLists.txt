include_directories(../include)

file(GLOB muParser_src "*.cpp*")

add_library(muparser SHARED ${muParser_src})
install(TARGETS muparser DESTINATION .)
