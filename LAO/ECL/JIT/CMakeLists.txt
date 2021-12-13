set(XCC_FILES
  #BD3 ExecutionEngine
  #BD3 Interpreter
  Compiler
  JIT
  #BD3 Context
  CodeBuffer
  )

#BD3 if(CMAKE_CROSSCOMPILING) 
#BD3   set(ASM_FILES ${FAMDIR_LAO}/${TARGET}-Trampoline-host.s)    
#BD3 else(CMAKE_CROSSCOMPILING) 
#BD3   set(ASM_FILES Trampoline_dummy)    
#BD3 endif(CMAKE_CROSSCOMPILING)    

set(FIND_MORE_FILES true)
set(library_link_libraries CCL MDT AIR LIR PFA CGO JIT)
generate_library_rules(JIT)
set(FIND_MORE_FILES)

