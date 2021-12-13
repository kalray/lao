set(XCC_FILES
  Issue
  Effect
  Lifetime
  CFGraph
  RegionFlow
  BlockTrace
  DDGraph
  XFA
  )

set(library_link_libraries CCL CAL MDT AIR LIR PFA)
generate_library_rules(XFA)
