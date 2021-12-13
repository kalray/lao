SET(XCC_FILES
  Except
  Hacker
  Int128
  Int256
  FTypes
  Flip16
  Flip32
  Flip64
  Memory
  BBTree
  String
  Buffer
  ETable
  Escape
  XMLCan
  XMLGen
  BSL
  )


# For floating point tests we need to link with testsoftfloat.
# This link_directories must be called before Flip{32,64}.tst target # is created.
LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/testsoftfloat)
#BD3 LINK_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/IeeeCC754)

generate_library_rules(BSL)

# message(STATUS "xcc files are ${XCC_FILES}") 
# message(STATUS "xcc files_c are ${XCC_FILES_C}") 
# message(STATUS "xcc files_h are ${XCC_FILES_H}") 

#BD3 ADD_SUBDIRECTORY(testsoftfloat)
#BD3 ADD_SUBDIRECTORY(IeeeCC754)

#BD3 PROJECT(Flip_tst C CXX)
#BD3 ADD_DEPENDENCIES(Flip32.tst testsoftfloat IeeeCC754)
#BD3 ADD_DEPENDENCIES(Flip64.tst testsoftfloat IeeeCC754)

#BD3 TARGET_LINK_LIBRARIES(Flip32.tst libIeeeCC754.a libtestsoftfloat.a BSL m)
#BD3 TARGET_LINK_LIBRARIES(Flip64.tst libIeeeCC754.a libtestsoftfloat.a BSL m)
#BD3 SET_TARGET_PROPERTIES(Flip32.tst PROPERTIES LINKER_LANGUAGE CXX)
#BD3 SET_TARGET_PROPERTIES(Flip64.tst PROPERTIES LINKER_LANGUAGE CXX)


 
