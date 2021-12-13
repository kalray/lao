set(XCC_FILES
  Platform
  Processor
  Decoding
  NativeType
  Storage
  Register
  RegFile
  RegClass
  RegMask
  Modifier
  Relocation
  Immediate
  Operand
  Format
  Behavior
  Opcode
  Operator
  Bundling
  Template
  Bundle
  Resource
  Reservation
  Scheduling
  Convention
  Decode
  MDT
  )

set(FIND_MORE_FILES true) # look for files like kv3-XXX.c in the kv3 directory
set(library_link_libraries BSL CCL)
generate_library_rules(MDT)
set(FIND_MORE_FILES)      # stop looking for those files
