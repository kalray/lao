set(XCC_FILES
  Label
  Symbol
  Literal
  Section
  Object
  Common
  Frame
  Instruction
  NativeBlock
  NativeStream
  TopDown
  Scoreboard
  CodeTrace
  Trampoline
  AIR
  )

set(FIND_MORE_FILES true) # look for files like kv3-XXX.c in the FAMILY directory
set(library_link_libraries BSL CCL MDT)
generate_library_rules(AIR)
set(FIND_MORE_FILES)      # stop looking for those files
