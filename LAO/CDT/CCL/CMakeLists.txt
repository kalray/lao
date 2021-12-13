set(XCC_FILES
  PQueue
  IArray
  IStack
  IQueue
  PtrSeq
  IBList
  IDList
  TSTree
  PtrSet
  BitSet
  Sparse
  IETree
  HTable
  Scoped
  PtrMap
  Indexed
  CCL
  )

set(library_link_libraries BSL)
generate_library_rules(CCL)
