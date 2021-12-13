# Sub-project name + standard configuration
setup_subproject(TSL)

# Includes for other sub-projects headers
include_directories(${PROJECT_BINARY_DIR}/CDT)
include_directories(${PROJECT_BINARY_DIR}/CDT/BSL)
include_directories(${PROJECT_BINARY_DIR}/CDT/CCL)
include_directories(${PROJECT_BINARY_DIR}/CDT/CAL)
include_directories(${FAMILY}/${TARGET})

add_subdirectory(MDT)
add_subdirectory(AIR)
