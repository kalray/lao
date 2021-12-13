set(XCC_FILES
  Data
  Optimize
  Configure
  Temporary
  Operation
  Variable
  Selector
  BasicBlock
  Dependence
  LoopScope
  CodeRegion
  Procedure
  Program
  Minir
  Tirex
  LIR
  )

set(FIND_MORE_FILES true)
set(library_link_libraries CCL MDT AIR)
generate_library_rules(LIR)
set(FIND_MORE_FILES)


if(YAML_ENABLED)
  target_link_libraries(LIR yaml)
  #target_link_libraries(LIR_shared yaml)
endif()
