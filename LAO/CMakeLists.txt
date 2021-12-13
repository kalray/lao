# Check cmake version
cmake_minimum_required (VERSION 2.6)

## ccache not really better (takes time to enter ccache & verify no need no compile anyway)
#set(CMAKE_C_COMPILER_INIT "ccache gcc")
#set(CMAKE_C_COMPILER_INIT "/usr/lib/ccache/gcc")

# Project name
project (LAO C)

# Allow duplicate targets
# useful for 'make check' => same 'check' target in subdirectories
set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS true)

option(ONLY_DOC "Only for building documentation with doxygen. Does not require FAMILY, TARGET, and others to be set." false)

if(NOT ONLY_DOC)

###########################################
# Options
###########################################

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#####################
# Macros for options

# Defines a new option 'opt' that toggle the #def of '_opt'
#
function(underscore_option opt str default)
  option(${opt} ${str} ${default})
  if(${opt})
    set(_${opt} true CACHE INTERNAL ${str})
  else()
    #set(_${opt} CACHE) # clear variable
  endif()
endfunction(underscore_option opt str default)



# Check if an option is correctly defined
function(check_defined opt)
  if(NOT ${opt})
    message(SEND_ERROR "Option ${opt} is not defined!
      Please define it using -D${opt}=<value> or ccmake")
  endif()
endfunction(check_defined)

function(check_defined_path opt)
  if(NOT ${opt})
    message(SEND_ERROR "Option ${opt} is not defined!
      Please define it using -D${opt}=<path> or ccmake")
  else()
    if(NOT EXISTS ${${opt}})
      message(SEND_ERROR "Option ${opt} is set to non-existent file or directory!
      ${${opt}}")
    endif()
  endif()
endfunction(check_defined_path)

function(check_enabled opt)
  if(${opt}_ENABLED)
    check_defined_path(${opt})
  endif()
endfunction(check_enabled)

####################
# The actual options

# examples for FAMILY and YAML

set(FAMILY "" CACHE PATH "Path to processor family to use")
set(TARGET "" CACHE STRING "Processor target to use")

set(INSTALL_PREFIX_RUN "" CACHE PATH "Path where to install runtime files for open64. Leave empty if no installation required.")
set(INSTALL_PREFIX_LIB "" CACHE PATH "Path where to install library files for open64. Leave empty if no installation required.")

option(RE2C_ENABLED "Use re2c to compile regular expressions" OFF)
option(GLPK_ENABLED "Use glpk to solve linear programs" ON)
option(CPLEX_ENABLED "Use cplex to solve linear programs" OFF)
option(YAML_ENABLED "Use yaml library to feed lao with Minir files" ON)
option(SOFTFLOAT_ENABLED "Use softfloat library for floating point emulation" ON)
option(WARNINGS "Enable gcc warnings" ON)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" ON)

underscore_option(DEBUG  "Enable low-level debug" OFF)
underscore_option(NTRACE "Disable traces" OFF)
underscore_option(NDTOR  "Disable destructors (DTOR)" OFF)

option(FLO_DEBUG "Enable personnal debugging messages (Florent)" OFF)

set(MEMORY_STATS 0 CACHE STRING "Memory statistics mask")

option(Memory_DEBUG "Enable memory debug" OFF)

set(LAO_INPUT ${PROJECT_SOURCE_DIR}/../../../LAO_INPUT
  CACHE PATH "Directory where to find .input.c files")
set(LAO_TRACE ${PROJECT_SOURCE_DIR}/../../../LAO_TRACE
  CACHE PATH "Directory where to find trace options")

if( CMAKE_BUILD_TYPE STREQUAL "Release" OR
    CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
    )
  add_definitions(-fno-strict-aliasing)
endif()

# Setup install paths
macro(SetupInstallPaths)
# Inputs:
# CROSS_COMPILE_32BITS: request 32 bits compilation
# CMAKE_INSTALL_PREFIX: standard CMAKE installation path prefix
#
# Outputs:
# _CROSS_COMPILE_32BITS: Actually cross compile 32 bits
# HOST_ARCH_32: True is host bitness is 32bits
# HOST_ARCH_64: True is host bitness is 64bits
# LIBRARY_INSTALL_BASENAME: The library directory basename (lib)
# LIBRARY_INSTALL_DIR: The library install full path

  # Check default install prefix
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT) 
    set(CMAKE_INSTALL_PREFIX INSTALL_DEFAULT CACHE STRING "Install prefix" FORCE) 
  endif() 

  # Now define install paths depending on whether we are on 32 or 64 bit
  # machine
  if(NOT DEFINED CMAKE_SYSTEM_PROCESSOR)
    execute_process(
      COMMAND uname -m
      OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )
  endif()

  # Keeping track of if we are cross compiling for 32 bits
  set(_CROSS_COMPILE_32BITS OFF)
  message(STATUS "Compiler host arch is \"${CMAKE_SYSTEM_PROCESSOR}\"")

  if( EXISTS "/etc/fedora-release" )
    set( DISTRIB "Fedora" )
  elseif( EXISTS "/etc/redhat-release" )
    set( DISTRIB "CentOS" )
  elseif( EXISTS "/etc/debian_version" )
    set( DISTRIB "Debian" )
  else()
    message(FATAL_ERROR "Could not detect the distro...")
  endif()
  
  message(STATUS "DISTRIB is ${DISTRIB}.")

  # Check if building on a 64-bit machine
  if(CMAKE_SYSTEM_PROCESSOR MATCHES ".*_64")

    set(HOST_ARCH_32 FALSE)
    set(HOST_ARCH_64 TRUE)

    if(CROSS_COMPILE_32BITS AND CMAKE_SIZEOF_VOID_P EQUAL 8)
      message(STATUS "Compiling for 32-bit")

      add_definitions(-m32) 
      set(CMAKE_EXE_LINKER_FLAGS "-m32")
      set(CMAKE_SHARED_LINKER_FLAGS "-m32")

      set(_CROSS_COMPILE_32BITS ON)
      set (_DEFAULT_LIBRARY_INSTALL_DIR lib)
    else()
      # Compiling for 64-bit 
      set (_DEFAULT_LIBRARY_INSTALL_DIR lib)
    endif ()

  # Otherwise, nothing special
  else()
    set (_DEFAULT_LIBRARY_INSTALL_DIR lib)
    set(HOST_ARCH_32 TRUE)
    set(HOST_ARCH_64 FALSE)
  endif()

  # the library install dir
  set(LIBRARY_INSTALL_DIR "${_DEFAULT_LIBRARY_INSTALL_DIR}" CACHE PATH "Installation directory for libraries")
  if(NOT IS_ABSOLUTE "${LIBRARY_INSTALL_DIR}")
    set(LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${LIBRARY_INSTALL_DIR}")
  endif()
  set(LIBRARY_INSTALL_BASENAME "${_DEFAULT_LIBRARY_INSTALL_DIR}")

  if(HOST_ARCH_32)
    message(STATUS "Host Architecture is 32 bits")
  elseif(HOST_ARCH_64)
    message(STATUS "Host Architecture is 64 bits")
  else()
    message(FATAL_ERROR "Unknown host Architecture")
  endif()

  message(STATUS "Install directory:    ${CMAKE_INSTALL_PREFIX}")
  message(STATUS "Install lib basename: ${LIBRARY_INSTALL_BASENAME}")
  message(STATUS "Install lib dir:      ${LIBRARY_INSTALL_DIR}")

  set(LIBRARY_INTERNAL_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/kalray_internal/${LIBRARY_INSTALL_BASENAME})
  set(INTERNAL_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/kalray_internal)

  message(STATUS "Internal install dir:     ${INTERNAL_INSTALL_DIR}")
  message(STATUS "Internal install lib dir: ${LIBRARY_INTERNAL_INSTALL_DIR}")

endmacro()

SetupInstallPaths()

# Installing lao libraries and headers in lib*/lao
set(_DEFAULT_LIBRARY_SEARCH_DIR ${_DEFAULT_LIBRARY_INSTALL_DIR})
set(_DEFAULT_LIBRARY_INSTALL_DIR ${_DEFAULT_LIBRARY_INSTALL_DIR}/lao)
set(_DEFAULT_INCLUDE_INSTALL_DIR ${KALRAY_INTERNAL}/include/lao)


# FIXME! Alignment of malloc(), that is, maximun alignment required by the basic C types.
if( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  set(ALIGN_MAX 8 CACHE STRING "Set memory align (4 on linux 8 on windows, 8 on 64bits machines?)")
else()
  set(ALIGN_MAX 4 CACHE STRING "Set memory align (4 on linux 8 on windows, 8 on 64bits machines?)")
endif()


if(GLPK_ENABLED)
  if(NOT KVX_GLPK)
    message(FATAL_ERROR, "GLPK_ENABLED and KVX_GLPK not defined.")
  endif()

  include_directories(${KVX_GLPK}/include)
  link_directories(${KVX_GLPK}/${_DEFAULT_LIBRARY_SEARCH_DIR})
endif(GLPK_ENABLED)

if(YAML_ENABLED)
  if(NOT LIBYAML_PREFIX)
    message(FATAL_ERROR, "YAML_ENABLED and LIBYAML_PREFIX not defined.")
  endif()

  include_directories(${LIBYAML_PREFIX}/include)

  find_file(YAML_STATIC libyaml.a ${LIBYAML_PREFIX}/${_DEFAULT_LIBRARY_SEARCH_DIR})
  link_directories(${LIBYAML_PREFIX}/${_DEFAULT_LIBRARY_SEARCH_DIR})
  if(NOT YAML_STATIC)
    message(SEND_ERROR "Cannot find libyaml.a in path (\"${LIBYAML_PREFIX}\"), please check LIBYAML_PREFIX!")
  endif()
endif(YAML_ENABLED)

if(SOFTFLOAT_ENABLED)
  if(NOT SOFTFLOAT_PREFIX)
    message(FATAL_ERROR, "SOFTFLOAT_ENABLED and SOFTFLOAT_PREFIX not defined.")
  endif()

  include_directories(${SOFTFLOAT_PREFIX}/include)

  find_file(SOFTFLOAT_STATIC softfloat.a ${SOFTFLOAT_PREFIX}/${_DEFAULT_LIBRARY_SEARCH_DIR})
  link_directories(${SOFTFLOAT_PREFIX}/${_DEFAULT_LIBRARY_SEARCH_DIR})
  if(NOT SOFTFLOAT_STATIC)
    message(SEND_ERROR "Cannot find softfloat.a in path (\"${SOFTFLOAT_PREFIX}/${_DEFAULT_LIBRARY_SEARCH_DIR}\"), please check SOFTFLOAT_PREFIX!")
  endif()
endif(SOFTFLOAT_ENABLED)


#check_defined(TOOLROOT)
#message(STATUS "toolroot is ${TOOLROOT}")
# Check TARGET and FAMILY are defined
check_defined(TARGET)
message(STATUS "Checking FAMILY ${FAMILY}")
check_defined(FAMILY)

# Setup 'cat' utility to work on windows also
set(cat_prog cat)
if(WIN32)
  if(NOT UNIX)
    set(cat_prog type)
  endif()
endif(WIN32)

message(STATUS "Configuring LAO for TARGET ${TARGET}")
message(STATUS "using FAMILY path ${FAMILY}/${TARGET}")
set(FAMDIR_LAO ${FAMILY})
if(NOT EXISTS ${FAMDIR_LAO})
  message(SEND_ERROR "FAMILY hierarchy missing: ${FAMDIR_LAO} could not be FOUND")
endif()


##########################
## GCC compilation options


# Language standard
add_definitions(-std=gnu99)

# Exceptions handling
add_definitions(-fexceptions)

# use in shared libraries
add_definitions(-fpic)

# Warnings
add_definitions(-Wall)
add_definitions(-Wno-unused)

if(WARNINGS)
  add_definitions(-Wmissing-prototypes)
  add_definitions(-Wmissing-declarations)
  add_definitions(-Wnested-externs)
  add_definitions(-Wpointer-arith)
  add_definitions(-Wno-cast-qual)
  add_definitions(-Wcast-align)
  add_definitions(-Wwrite-strings)
endif(WARNINGS)


find_program(BISON bison)
find_program(PERL perl)
find_program(RUN_TEST run-test ${PROJECT_SOURCE_DIR}/. DOC "Script to run a self test & compare results against reference")

if(NOT BISON)
  message(SEND_ERROR "bison could not be found")
endif(NOT BISON)
if(NOT PERL)
  message(SEND_ERROR "perl could not be found")
endif(NOT PERL)
if(NOT RUN_TEST)
  message(SEND_ERROR "run-test could not be found")
endif(NOT RUN_TEST)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})


## Setup a subproject of LAO
# - defines subproject name
# - configure main project header file
# - include directories for headers
macro(setup_subproject name)
  set(SUB_PROJECT_NAME ${name})
  message(STATUS "Setting sub-project ${name} up.")

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${name}.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/${name}.h
    )
  # Install header file for this sub-project
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${name}.h
    DESTINATION ${_DEFAULT_INCLUDE_INSTALL_DIR}
    )

  include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  include_directories(${CMAKE_CURRENT_BINARY_DIR})

endmacro(setup_subproject)



## File generation using xccode

# Look for perl script
find_file(XCCODE xccode.pl
  ${PROJECT_SOURCE_DIR}/CDT/BIN
  DOC "Perl script to generate _.c _.h and .h files"
  NO_CMAKE_FIND_ROOT_PATH
  )
if(NOT XCCODE)
  message(SEND_ERROR "xccode not found in CDT/BIN")
endif(NOT XCCODE)
set(XCC_FLAGS --line)

# use CCACHE if available
# find_program(CCACHE ccache)
# if(NOT CCACHE)
#   message(WARNINGS "cannot find `ccache'")
# else()
#   set(CC ${CCACHE} ${CC})
#   set(XCC_FLAGS --always ${XCC_FLAGS}) # `a'lways regenerate files even content is the same
# endif()
# message(STATUS "CC is then ${CC}")
# message(STATUS "xccode flags are ${XCC_FLAGS}")


# Generate custom make commands to produce _.h .h and _.c file from .c
# variables XCC__C, XCC__H and XCC_H must be set appropriately (best is to use
# macros 'xcc_cgen' and 'xcc_hgen')
# 'library' is required to link the .tst test files
macro(xcc_generic xccfile library)
  # Check if additionnal file from FAMILY should be passed to xccode
  if(FIND_MORE_FILES)
    set(extra_file ${FAMDIR_LAO}/${TARGET}-${xccfile}.c)
    if(EXISTS ${extra_file})
      message(STATUS "Extra file ${extra_file} added to ${library} library.")
      set(MORE_XCC_FILES ${MORE_XCC_FILES} ${extra_file})
    else()
      set(extra_file) # clear variable
    endif()
  else(FIND_MORE_FILES)
    set(MORE_XCC_FILES) # clear variable
  endif(FIND_MORE_FILES)

  # "xcc" pre-processing of .c files
  add_custom_command (
    OUTPUT ${XCC__C} ${XCC__H} ${XCC_H}
    COMMAND ${PERL} -w -I ${PROJECT_SOURCE_DIR}/CDT/LIB
      ${XCCODE} ${XCC_FLAGS}
      ${CMAKE_CURRENT_SOURCE_DIR}/${xccfile}.c ${extra_file}
    DEPENDS ${XCCODE} ${CMAKE_CURRENT_SOURCE_DIR}/${xccfile}.c
    )

  # generate test
  if(XCC__C)
    set(test_file ${xccfile}.tst)

    # Create self test executable
     add_executable(
       ${test_file}
       EXCLUDE_FROM_ALL
       ${CMAKE_CURRENT_SOURCE_DIR}/${xccfile}.c)

    set_target_properties(${test_file} PROPERTIES COMPILE_FLAGS -DXCC__C)
    target_link_libraries(${test_file} ${library})
    #target_link_libraries(${test_file} ${library}_shared)

    ### Generation of references
    # Generate ref file
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${xccfile}.1
      COMMAND ${test_file}
        < ${CMAKE_CURRENT_SOURCE_DIR}/0/${xccfile}.0
        > ${CMAKE_CURRENT_BINARY_DIR}/${xccfile}.1
      DEPENDS ${test_file} ${CMAKE_CURRENT_SOURCE_DIR}/0/${xccfile}.0
      )

    set(RES_FILES ${RES_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${xccfile}.1)

    # Comparison of the output of the self test with reference
    add_test (${SUB_PROJECT_NAME}-${library}-${xccfile}
      diff -u
      ${TEST_REF_DIR}/${xccfile}.1
      ${CMAKE_CURRENT_BINARY_DIR}/${xccfile}.1
      )


    ### Generation of references
    # Generate ref file
    # Use add_custom_target instead of add_custom_command to prevent
    # `make clean' from deleting ref files
    add_custom_target(
      ref-${xccfile}.1
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${xccfile}.1 ${TEST_REF_DIR}
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${xccfile}.1
      )

    # Add ref file to list of refs
    add_dependencies(refs ref-${xccfile}.1)

  endif(XCC__C)
endmacro(xcc_generic)

## Set XCC__C variable for an "xcc" file that generates a _.c
macro (xcc_cgen xccfile)
  set(XCC__C ${CMAKE_CURRENT_BINARY_DIR}/${xccfile}_.c)
endmacro (xcc_cgen)

## Set XCC__H and XCC_H variables, i.e., the _.h and .h generated
## Also add .c and .h to the list of XCC_FILES
macro (xcc_hgen xccfile)
  set(XCC__H ${CMAKE_CURRENT_BINARY_DIR}/${xccfile}_.h)
  set(XCC_H ${CMAKE_CURRENT_BINARY_DIR}/${xccfile}.h)
  set(XCC_FILES_C ${XCC_FILES_C} ${xccfile}.c)
  set(XCC_FILES_H ${XCC_FILES_H} ${xccfile}.h)
  set(XCC_FILES__H ${XCC_FILES__H} ${xccfile}_.h)
endmacro (xcc_hgen)

# Generates rules for files in variables
#   XCC_FILES     -> _.c, _.h and .h generation
# and populates variables
#   XCC_FILES_C
#   XCC_FILES_H
#   XCC_FILES__H
# This should be used in subdirectories
macro (generate_library_rules library)

  # Add ref target to regenerate ref files used for self-tests
  # without dependencies for now, they will be added later with
  # `add_dependencies'
  add_custom_target(refs
    COMMENT "References updated for library ${library}"
    )

  set(TEST_REF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/v1)
  if(NOT EXISTS ${TEST_REF_DIR})
    set(TEST_REF_DIR ${FAMDIR_LAO}/${TARGET}/v1)
  endif()


  foreach(xccfile ${XCC_FILES})
    xcc_cgen(${xccfile})
    xcc_hgen(${xccfile})
    xcc_generic(${xccfile} ${library})
  endforeach(xccfile)
  set(XCC__C) # clear variable

#   threat all asm files as c files
  foreach(asmfile ${ASM_FILES})
    set_property(SOURCE ${asmfile} PROPERTY LANGUAGE C)
  endforeach(asmfile)

  if (LAO_STATIC)
    add_library(${library} STATIC ${XCC_FILES_C} ${XCC_FILES_H} ${MORE_XCC_FILES} ${ASM_FILES})
  else (LAO_STATIC)
    add_library(${library} ${XCC_FILES_C} ${XCC_FILES_H} ${MORE_XCC_FILES} ${ASM_FILES})
  endif (LAO_STATIC)
  set_target_properties(${library}
    PROPERTIES
    OUTPUT_NAME ${library}
    CLEAN_DIRECT_OUTPUT 0
    CMAKE_BUILD_DIR ${library}
    )

  set_target_properties(${library}
    PROPERTIES
    INSTALL_RPATH $ORIGIN/.:$ORIGIN
    )
  
  # Add link libraries
  target_link_libraries(${library} ${library_link_libraries})

  # setup check target
  add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND}
    COMMENT "Performing self-check for library ${library}"
    DEPENDS ${RES_FILES}
    )

  # Install lib<library>.a file
  install_results(${library})

  ## Why was that there?
  #install_prefix(${library})

  # Install individual header files for ISS
  foreach(hfile ${XCC_FILES_H} ${XCC_FILES__H})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${hfile}
      DESTINATION ${_DEFAULT_INCLUDE_INSTALL_DIR}/${library}
      )
  endforeach(hfile)

  # Install main header file
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${library}.h
    DESTINATION ${_DEFAULT_INCLUDE_INSTALL_DIR}
    )

endmacro (generate_library_rules)

#################################################
# INSTALLATION
#################################################

#set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})

# Default installation directory is build directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}
    CACHE PATH "Default install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

message(STATUS "Installation prefix set to ${CMAKE_INSTALL_PREFIX}")

# Install standard generated files
macro(install_results results)
  install(TARGETS ${results}
    RUNTIME DESTINATION ${KALRAY_INTERNAL}/bin         # executable
    LIBRARY DESTINATION ${_DEFAULT_LIBRARY_INSTALL_DIR}         # shared lib
    ARCHIVE DESTINATION ${_DEFAULT_LIBRARY_INSTALL_DIR}         # static lib
    )
endmacro(install_results)

# Install standard generated files
macro(install_prefix results)
  if(INSTALL_PREFIX_LIB)
  install(TARGETS ${results}
    RUNTIME DESTINATION ${INSTALL_PREFIX_RUN}
    LIBRARY DESTINATION ${INSTALL_PREFIX_LIB}
    ARCHIVE DESTINATION ${INSTALL_PREFIX_LIB}
    )
  endif()
endmacro(install_prefix results)



# TODO: too many _.h files installed
# The only library headers _.h required are: BSL, MDT, AIR


# Install all .tuple files found in ${FAMILY}
file(GLOB TUPLE_FILES ${FAMDIR_LAO}/${TARGET}/*.tuple)
message(STATUS "Tuple files from ${FAMDIR_LAO}/${TARGET} will be installed: ${TUPLE_FILES}")
install(FILES ${TUPLE_FILES}
  DESTINATION ${_DEFAULT_INCLUDE_INSTALL_DIR}
  )

#################################################
# Testing
#################################################

enable_testing()

#add_custom_target(check)
#add_dependencies(check install)

#################################################
# Build subdirectories
#################################################

if(RE2C_ENABLED)
  add_subdirectory(RE2C)
endif()

add_subdirectory(CDT) # Compiler Development Tools
add_subdirectory(TSL) # Target Specific Libraries
add_subdirectory(ECL) # Embedded Compiler Libraries
add_subdirectory(PRO) # Legacy Pro64 (before Open64)


#################################################
# Traces
# Note: traces are based on the old way of using
# lao through the .input.c files generated by
# open64
#################################################

# Get compile_definitions for Traces.make
set(CFLAGS "-std=gnu99 -fPIC")
message(STATUS "Compile defs for traces are ${CFLAGS}")


# Debug
# if(DEBUG)
#   add_definitions(-g -O0)
#   set(_DEBUG true CACHE INTERNAL "Enable debug mode")
# else(DEBUG)
#   add_definitions(-O2 -fno-strict-aliasing)
#   set(_DEBUG CACHE) # clear
# endif(DEBUG)


##TODO: set these options?
# set(CMAKE_SHARED_LINKER_FLAGS -shared -Wl,-Bsymbolic)


configure_file(Traces.make.in Traces.make)

# Add a new target based on targets of Traces.make
macro(set_traces_target target)
  add_custom_target(${target}
    COMMAND ${CMAKE_BUILD_TOOL} -f Traces.make ${target}
  )
endmacro()

foreach(target echo inputs traces tests clear noinputs)
  set_traces_target(${target})
endforeach()


## Archive build
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_SOURCE_IGNORE_FILES
  "/build.*/;/.git/;.*~"
  )
include(CPack)

endif(NOT ONLY_DOC)


############################################
# Documentation via Doxygen
############################################

include(TargetDoc.cmake OPTIONAL)
