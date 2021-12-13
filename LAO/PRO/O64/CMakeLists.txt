set(XCC_FILES
  CGIR
  Interface
  O64
  )

set(library_link_libraries BSL CCL CAL MDT AIR LIR PFA CGO JIT XFA SCD)
generate_library_rules(O64)
target_link_libraries(O64 dl)
#target_link_libraries(O64_shared dl)
# Library dl only required for CGIR.tst => check if better not on all
