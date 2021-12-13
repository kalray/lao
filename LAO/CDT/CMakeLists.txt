# Sub-project name
setup_subproject(CDT)

find_program(BISON bison)
find_program(PERL perl)

if(NOT BISON)
  message(SEND_ERROR "bison could not be found")
endif(NOT BISON)
if(NOT PERL)
  message(SEND_ERROR "perl could not be found")
endif(NOT PERL)


#find_program(XCCODE xccode BIN)
#if(NOT XCCODE)
#  message(SEND_ERROR "xccode could not be found")
#endif(NOT XCCODE)

# set(ENV{PERLLIB} LIB)

set(TEST_REF_DIR) # clear variable
add_subdirectory(BIN)
add_subdirectory(BSL)
add_subdirectory(CCL)
add_subdirectory(CAL)

#message(STATUS "Check depends in CDT is ${CHECK_DEPENDS}")
#write_parent_check_depends()

#set(CHECK_DEPENDS ${CHECK_DEPENDS} PARENT_SCOPE)
