set(XCC_FILES
  Liveness
  Dominance
  LoopForest
  CodeLayout
  SuperBlock
  ValueNumber
  Interference
  SSAForm
  LiveSets
  LiveCheck
  SSAConstruct
  Deallocator
  OldLiveCheck
  Coalescer
  Decoalescer
  Boissinot
  Sreedhar
  PFA
  )

set(FIND_MORE_FILES true) # look for files like kv3-XXX.c in the FAMILY directory
set(library_link_libraries CCL MDT AIR LIR)
generate_library_rules(PFA)
set(FIND_MORE_FILES)      # stop looking for those files
