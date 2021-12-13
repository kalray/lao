find_file(DECLARE Declare.pm ${PROJECT_SOURCE_DIR}/CDT/LIB
  NO_CMAKE_FIND_ROOT_PATH
  )

if(NOT DECLARE)
  message(SEND_ERROR "Cannot find file Declare.pm")
endif(NOT DECLARE)
