# Sub-project name + standard configuration
setup_subproject(PRO)

if(YAML_ENABLED)
  include_directories(${YAML}/include)
  link_directories(${YAML}/lib)
endif(YAML_ENABLED)

if(SOFTFLOAT_ENABLED)
  include_directories(${SOFTFLOAT_PREFIX}/include)
  link_directories(${SOFTFLOAT_PREFIX}/lib)
endif(SOFTFLOAT_ENABLED)

# Includes for other sub-projects headers
include_directories(${PROJECT_BINARY_DIR}/CDT)
include_directories(${PROJECT_BINARY_DIR}/CDT/BSL)
include_directories(${PROJECT_BINARY_DIR}/CDT/CCL)
include_directories(${PROJECT_BINARY_DIR}/CDT/CAL)
include_directories(${PROJECT_BINARY_DIR}/TSL)
include_directories(${PROJECT_BINARY_DIR}/TSL/MDT)
include_directories(${PROJECT_BINARY_DIR}/TSL/AIR)
include_directories(${PROJECT_BINARY_DIR}/ECL)
include_directories(${PROJECT_BINARY_DIR}/ECL/LIR)
include_directories(${PROJECT_BINARY_DIR}/ECL/PFA)
include_directories(${PROJECT_BINARY_DIR}/ECL/CGO)
include_directories(${PROJECT_BINARY_DIR}/ECL/JIT)
include_directories(${FAMILY}/${TARGET})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/XFA)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/SCD)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/O64)

add_subdirectory(XFA)
add_subdirectory(SCD)
add_subdirectory(O64)

add_executable(${TARGET}-lao3 lao3.c)
target_link_libraries(${TARGET}-lao3 SCD XFA JIT CGO PFA LIR AIR MDT CAL CCL BSL)
set_target_properties(${TARGET}-lao3 PROPERTIES INSTALL_RPATH $ORIGIN/../${LIBRARY_INSTALL_BASENAME}/lao)
install_results(${TARGET}-lao3)

# Add source O64.c else nothing gets into the library
add_library(lao SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/O64/O64.c
  )
set_target_properties(lao PROPERTIES PREFIX "")
#TODO: flag "-Wl,-Bsymbolic" ?
target_link_libraries(lao 
  BSL CCL CAL MDT AIR LIR PFA CGO JIT XFA SCD O64
  )
set_target_properties(lao PROPERTIES LINKER_LANGUAGE C)

install_results(lao)
if(INSTALL_OPEN64)
  install(TARGETS lao DESTINATION ${INSTALL_OPEN64})
endif()


#add_custom_target(check
#  COMMAND echo "Checking ${SUB_PROJECT_NAME}..."
#  )

set(CPP2CFLAGS -std=gnu99 -E -dD -P)


set(LAO_STUB lao_stub.h)
set(LAO_INTERFACE lao_interface.h)
set(LAO_TARGET lao_target.h)

add_custom_command(
  OUTPUT ${LAO_STUB}
  COMMAND perl -00 -ne '/^typedef enum/m && print' 
    ${PROJECT_BINARY_DIR}/TSL/AIR/Symbol.h
    ${PROJECT_BINARY_DIR}/ECL/LIR/Optimize.h
    ${PROJECT_BINARY_DIR}/ECL/LIR/Configure.h
    ${PROJECT_BINARY_DIR}/ECL/LIR/Dependence.h
    > STUB.h
  COMMAND echo "typedef void *DependenceNode\;" >> STUB.h
  COMMAND echo "extern void lao_init\\(void\\)\\;" >> STUB.h
  COMMAND echo "extern void lao_fini\\(void\\)\\;" >> STUB.h
  COMMAND echo "extern void lao_init_pu\\(void\\)\\;" >> STUB.h
  COMMAND echo "extern void lao_fini_pu\\(void\\)\\;" >> STUB.h
  COMMAND echo "extern bool lao_optimize_pu\\(unsigned\\)\\;" >> STUB.h
  COMMAND ${CMAKE_C_COMPILER} ${CPP2CFLAGS} #-I${CMAKE_INSTALL_PREFIX}/include
    -D_INCLUDED_MDT__h -D_INCLUDED_MDT_h -D_INCLUDED_CCL_h STUB.h
    | env LC_ALL=C egrep -v -e '\#\(define|undef\) \(_[_A-Z]|[a-z]\)'
    | sed -e 's/\\<this\\>/THIS/g\;s/\\<operator\\>/OPERATOR/g\;/^\$\$/d'
    > ${LAO_STUB}
  COMMAND rm -f STUB.h
  DEPENDS AIR LIR
  )

add_custom_command(
  OUTPUT ${LAO_INTERFACE}
  COMMAND perl -00 -ne '/^typedef /m && print'
    ${PROJECT_BINARY_DIR}/TSL/MDT/MDT.h > INTERFACE.h
  COMMAND echo '\#define Except_REQUIRE\(t\)' >> INTERFACE.h
  COMMAND echo '\#define Except_CHECK\(t\)' >> INTERFACE.h
  COMMAND echo "typedef struct Memory_ *Memory\;" >> INTERFACE.h
  COMMAND echo "typedef struct Label_ *Label\;" >> INTERFACE.h
  COMMAND echo "typedef struct Symbol_ *Symbol\;" >> INTERFACE.h
  COMMAND echo "typedef struct Temporary_ *Temporary\;" >> INTERFACE.h
  COMMAND echo "typedef struct Operation_ *Operation\;" >> INTERFACE.h
  COMMAND echo "typedef struct BasicBlock_ *BasicBlock\;" >> INTERFACE.h
  COMMAND echo "typedef struct LoopScope_ *LoopScope\;" >> INTERFACE.h
  COMMAND cat
    ${PROJECT_BINARY_DIR}/TSL/MDT/MDT.h
    ${CMAKE_CURRENT_BINARY_DIR}/O64/CGIR.h
    ${CMAKE_CURRENT_BINARY_DIR}/O64/Interface.h >> INTERFACE.h
  COMMAND ${CMAKE_C_COMPILER} ${CPP2CFLAGS} #-I${CMAKE_INSTALL_PREFIX}/include
    -D_INCLUDED_MDT__h -D_INCLUDED_MDT_h -D_INCLUDED_CCL_h INTERFACE.h 
    | env LC_ALL=C egrep -v -e '\#\(define|undef\) \(_[_A-Z]|[a-z]\)'
    | sed -e 's/\\<this\\>/THIS/g\;s/\\<operator\\>/OPERATOR/g\;/^\$\$/d' 
    > ${LAO_INTERFACE}
    COMMAND rm -f INTERFACE.h
  DEPENDS MDT O64
  )

# LAO_TARGET): $(includedir)/MDT/MDT_.h $(includedir)/MDT.h
add_custom_command(
  OUTPUT ${LAO_TARGET}
  COMMAND ${CMAKE_C_COMPILER} ${CPP2CFLAGS} 
    -I${SOFTFLOAT_PREFIX}/include
    -I${FAMILY}/${TARGET}
    -I${PROJECT_BINARY_DIR}/TSL
    -I${PROJECT_BINARY_DIR}/CDT/CCL
    -DBehavior_Header=0 "-DExcept_REQUIRE\\(t\\)=" -D_INCLUDED_CCL_h 
    ${PROJECT_BINARY_DIR}/TSL/MDT/MDT_.h > TARGET.h
  COMMAND ${cat_prog} TARGET.h
    | env LC_ALL=C egrep -v -e '\#\(define|undef\) \(_[_A-Z]|[a-z]\)'
    | sed -e 's/\\<this\\>/THIS/g\;s/\\<operator\\>/OPERATOR/g\;/^\$\$/d'
    > ${LAO_TARGET}
  COMMAND ${CMAKE_COMMAND} -E remove -f TARGET.h
  DEPENDS MDT
  )

add_custom_target(refs
# Do not install headers in BE/O64 as Open64 is not used anymore
#  COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    lao_stub.h ${INCLUDES_MDS_DIR}/
#  COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    lao_interface.h ${INCLUDES_MDS_DIR}/
#  COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    lao_target.h ${INCLUDES_MDS_DIR}/
  COMMENT "Updated O64 header files in ${INCLUDES_MDS_DIR}"
  DEPENDS 
    ${CMAKE_CURRENT_BINARY_DIR}/${LAO_STUB}
    ${CMAKE_CURRENT_BINARY_DIR}/${LAO_INTERFACE}
    ${CMAKE_CURRENT_BINARY_DIR}/${LAO_TARGET}
  )
