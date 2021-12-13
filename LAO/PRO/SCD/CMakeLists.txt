set(XCC_FILES
  FastList
  GrahamList
  Relaxation
  Scheduler
  Insertion
  Acyclic
  Formulation
  Construction
  RCMS
  SCD
  )

set(library_link_libraries CCL CAL MDT AIR LIR PFA XFA)
generate_library_rules(SCD)
# Library dl only required for CGIR.tst => check if better not on all
target_link_libraries(SCD dl)
#target_link_libraries(SCD_shared dl)
