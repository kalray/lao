set(XCC_FILES
  ControlFlow
  Predicator
  Accelerator
  PrePass
  Allocator
  ELinearScan
  DomTreeScan
  PostPass
  Spill
  CGO
  )

set(FIND_MORE_FILES true)
set(library_link_libraries CCL MDT AIR LIR PFA)
generate_library_rules(CGO)
set(FIND_MORE_FILES)

# Add some dependencies
