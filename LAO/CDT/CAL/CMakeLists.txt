
set(XCC_FILES
  DGraph
  Dominators
  Hierarchy
  Network
  LProgram
  CAL
)


set(library_link_libraries BSL CCL)
generate_library_rules(CAL)


if(GLPK_ENABLED)
  #target_link_libraries(CAL_shared glpk m)
  target_link_libraries(CAL m libglpk.a) 
endif()

