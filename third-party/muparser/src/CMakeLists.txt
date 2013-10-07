include_directories(../include)

OPTION(BUILD_MUPARSER_SHARED "Build the muparser libraries shared." OFF)
OPTION(BUILD_MUPARSER_SHARED "Build the muparser libraries shared." ON)

IF(BUILD_MUPARSER_SHARED)
  SET(MUPARSER_LIBRARY_TYPE SHARED)
ELSE(BUILD_MUPARSER_SHARED)
  SET(MUPARSER_LIBRARY_TYPE STATIC)
ENDIF(BUILD_MUPARSER_SHARED)

file(GLOB muParser_src "*.cpp*")

add_library(muparser ${MUPARSER_LIBRARY_TYPE} ${muParser_src})

