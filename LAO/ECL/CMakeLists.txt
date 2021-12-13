# Sub-project name + standard configuration
setup_subproject(ECL)

if(YAML_ENABLED)
  include_directories(${YAML}/include)
  link_directories(${YAML}/lib)
endif(YAML_ENABLED)

# Includes for other sub-projects headers
include_directories(${PROJECT_BINARY_DIR}/CDT)
include_directories(${PROJECT_BINARY_DIR}/CDT/BSL)
include_directories(${PROJECT_BINARY_DIR}/CDT/CCL)
include_directories(${PROJECT_BINARY_DIR}/CDT/CAL)
include_directories(${PROJECT_BINARY_DIR}/TSL)
include_directories(${PROJECT_BINARY_DIR}/TSL/MDT)
include_directories(${PROJECT_BINARY_DIR}/TSL/AIR)
include_directories(${FAMILY}/${TARGET})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/LIR)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/PFA)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/CGO)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/JIT)


add_subdirectory(LIR)
add_subdirectory(PFA)
add_subdirectory(CGO)
add_subdirectory(JIT)


link_directories(${PROJECT_BINARY_DIR}/CDT/CCL)
link_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${TARGET}-lao lao.c)
target_link_libraries(${TARGET}-lao JIT CGO PFA LIR AIR MDT CAL CCL BSL)



# if(YAML_ENABLED)
#   target_link_libraries(${TARGET}-lao yaml)
# endif(YAML_ENABLED)
# if(GLPK_ENABLED)
#   target_link_libraries(${TARGET}-lao glpk)
# endif(GLPK_ENABLED)

#install_results(${TARGET}-lao)

#add_custom_command(   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/laoi_cmdline.c
#                        COMMAND gengetopt --output-dir=${CMAKE_CURRENT_BINARY_DIR} 
#                                --file-name=laoi_cmdline --func-name=laoi_cmdline 
#                                --arg-struct-name=laoi_args_info --string-parser 
#                                --no-version --no-help < ${PROJECT_SOURCE_DIR}/ECL/laoi.ggo
#                        DEPENDS ${PROJECT_SOURCE_DIR}/ECL/laoi.ggo
#                        COMMENT "Generate laoi command line parser")

#add_custom_target( laoi_args DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/laoi_cmdline.c) 

#add_executable(${TARGET}-laoi laoi.c laoi_cmdline.c)
#target_link_libraries(${TARGET}-laoi JIT CGO PFA LIR AIR MDT CAL CCL BSL)
#install_results(${TARGET}-laoi)

