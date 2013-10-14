include_directories(../include)

option(BUILD_MUPARSER_SHARED "Build the muparser libraries shared." OFF)
option(BUILD_MUPARSER_SHARED "Build the muparser libraries shared." ON)

if(BUILD_MUPARSER_SHARED)
  set(MUPARSER_LIBRARY_TYPE SHARED)
else(BUILD_MUPARSER_SHARED)
  set(MUPARSER_LIBRARY_TYPE STATIC)
endif(BUILD_MUPARSER_SHARED)

file(GLOB muParser_src "*.cpp*")

add_library(muparser ${MUPARSER_LIBRARY_TYPE} ${muParser_src})

