#cmake_minimum_required (VERSION 2.6)
PROJECT(IeeeCC754)

ADD_LIBRARY(IeeeCC754 STATIC
            src/bitstring.cc
            src/fp.cc
            src/hex.cc
            src/IeeeCC754.cc
            BasicOp/Flip/fpenv_Flip.cc
            BasicOp/Flip/BasicOptest.cc
            Conversion/Flip/Conversiontest.cc
)

ADD_CUSTOM_TARGET(CP
  COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/BasicOp/testsets/ ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/Conversion/testsets/*
  ${CMAKE_CURRENT_BINARY_DIR}/testsets 
)

# We want IeeeCC754 to be built after Flip* has been compiled.
ADD_DEPENDENCIES(IeeeCC754 BSL CP 
            ${CMAKE_CURRENT_BINARY_DIR}/../FTypes.h)

SET_PROPERTY(SOURCE
  BasicOp/Flip/fpenv_Flip.cc
  src/bitstring.cc
  src/fp.cc
  src/hex.cc
  src/IeeeCC754.cc
  BasicOp/Flip/fpenv_Flip.cc
  BasicOp/Flip/BasicOptest.cc
  Conversion/Flip/Conversiontest.cc
  APPEND PROPERTY
  OBJECT_DEPENDS
            ${CMAKE_CURRENT_BINARY_DIR}/../FTypes.h
)
  


INCLUDE_DIRECTORIES (include)

REMOVE_DEFINITIONS(-Wdeclaration-after-statement)
REMOVE_DEFINITIONS(-Wmissing-prototypes)
REMOVE_DEFINITIONS(-Wnested-externs)
REMOVE_DEFINITIONS(-Wdeclaration-after-statement)
REMOVE_DEFINITIONS(-Wmissing-prototypes)
REMOVE_DEFINITIONS(-Wnested-externs)



# We force O1 because with O3 it fails on trayas when using Release
# build_type.
SET_TARGET_PROPERTIES(IeeeCC754 PROPERTIES COMPILE_FLAGS  "-g -Wall -I. -I../ -DBasicOperations -DConversions") 
