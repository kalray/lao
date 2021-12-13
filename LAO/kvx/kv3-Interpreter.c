#ifdef $XCC_h
/*
 * kv3-Interpreter.c
 *
 * Copyright 2009-2011 Kalray
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#include "JIT/JIT_.h"

#ifdef $XCC__h
#define HELPER(routine) Behavior_##routine
#define DECLARE(declare) static inline declare
#define BehaviorDeclare(ID, DECLARE) DECLARE
#include "Behavior.tuple"
#undef BehaviorDeclare
#undef DECLARE
#endif//$XCC__h


#ifdef $XCC__h
static inline void
Behavior_barrier(void *this) {
  Except_THROW("Behavior_barrier not implemented");
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   /* In the simulator the pipe is always empty after the execution of a bundle. */
//   /* Flush the prefetch buffer */
//   prefetch_buffer_flush(core->data_common.prefetch_buffer,KVX_NPC);
//   /* FIXME: manager load/store accesses ... */
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_MEM_cas(void *this, uint64_t opnd1, uint64_t opnd2, uint64_t opnd3, uint64_t opnd4) {
  Except_THROW("Behavior_MEM_cas not implemented");
//   Instr instr = (Instr)this;
//   uint32_t address = opnd1;
//   uint64_t sizeOf = opnd2;
//   uint64_t swapped_value = opnd3;
//   uint64_t compared_value = opnd4;
//   memory_access_t mem_access_compare;
//   memory_access_t mem_access_swap;
//   core_t *core = instr->core;
//
//   mem_access_compare.type = MEMORY_ACCESS_COMPARE_AND_SWAP;
//   mem_access_compare.address = address;
//   mem_access_compare.data = (uint8_t*)&compared_value;
//   mem_access_compare.size = sizeOf;
//   mem_access_compare.next = &mem_access_swap;
//   mem_access_compare.byte_enable = NULL;
//
//   mem_access_swap.type = MEMORY_ACCESS_COMPARE_AND_SWAP;
//   mem_access_swap.address = address;
//   mem_access_swap.data = (uint8_t*)&swapped_value;
//   mem_access_swap.size = sizeOf;
//   mem_access_swap.next = NULL;
//   mem_access_swap.byte_enable = NULL;
//
//   // Patrice: Should check if loaded value is 0 and if the case store 'stored_value' into memory...
//   core->runtime_cb.data_mem_access_check(core->runtime_cb.data_mem_access(core->runtime_cb.data_mem_driver,&mem_access_compare),MEMORY_ACCESS_COMPARE_AND_SWAP,address, sizeOf, instr);
//   if(core->attributes.cycle_based) {
//     core->data_common.ldst_stall = mem_access_compare.payload;
//     core->data_common.ldst_stall += mem_access_swap.payload;
//   }
//   if(mem_access_compare.size < 8) {
//     /* Access should be done in one memory access ... */
//     core->runtime_cb.data_mem_access_check(MEMORY_ACCESS_FAILURE,MEMORY_ACCESS_COMPARE_AND_SWAP,address, sizeOf, instr);
//   }
//
//   instr->address = address;
//   instr->effective_addr++;
//
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_MEM_casu(void *this, uint64_t opnd1, uint64_t opnd2, uint64_t opnd3, uint64_t opnd4) {
  Except_THROW("Behavior_MEM_casu not implemented");
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_MEM_faa(void *this, uint64_t opnd1, uint64_t opnd2, uint64_t opnd3) {
  Except_THROW("Behavior_MEM_faa not implemented");
//   Instr instr = (Instr)this;
//   uint32_t address = opnd1;
//   uint64_t sizeOf = opnd2;
//   uint64_t swapped_value = opnd3;
//   uint64_t compared_value = opnd4;
//   memory_access_t mem_access_compare;
//   memory_access_t mem_access_swap;
//   core_t *core = instr->core;
//
//   mem_access_compare.type = MEMORY_ACCESS_COMPARE_AND_SWAP;
//   mem_access_compare.address = address;
//   mem_access_compare.data = (uint8_t*)&compared_value;
//   mem_access_compare.size = sizeOf;
//   mem_access_compare.next = &mem_access_swap;
//   mem_access_compare.byte_enable = NULL;
//
//   mem_access_swap.type = MEMORY_ACCESS_COMPARE_AND_SWAP;
//   mem_access_swap.address = address;
//   mem_access_swap.data = (uint8_t*)&swapped_value;
//   mem_access_swap.size = sizeOf;
//   mem_access_swap.next = NULL;
//   mem_access_swap.byte_enable = NULL;
//
//   // Patrice: Should check if loaded value is 0 and if the case store 'stored_value' into memory...
//   core->runtime_cb.data_mem_access_check(core->runtime_cb.data_mem_access(core->runtime_cb.data_mem_driver,&mem_access_compare),MEMORY_ACCESS_COMPARE_AND_SWAP,address, sizeOf, instr);
//   if(core->attributes.cycle_based) {
//     core->data_common.ldst_stall = mem_access_compare.payload;
//     core->data_common.ldst_stall += mem_access_swap.payload;
//   }
//   if(mem_access_compare.size < 8) {
//     /* Access should be done in one memory access ... */
//     core->runtime_cb.data_mem_access_check(MEMORY_ACCESS_FAILURE,MEMORY_ACCESS_COMPARE_AND_SWAP,address, sizeOf, instr);
//   }
//
//   instr->address = address;
//   instr->effective_addr++;
//
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_MEM_faau(void *this, uint64_t opnd1, uint64_t opnd2, uint64_t opnd3) {
  Except_THROW("Behavior_MEM_faau not implemented");
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_dflush(void *this) {
  Except_THROW("Behavior_MEM_dflush not implemented");
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   if( core->runtime_cb.dcache_flush(core->runtime_cb.dcache_driver) == MEMORY_ACCESS_WARNING ) {
//     core->attributes.print(core->attributes.ostream,"WARNING data cache flush on Proc %d at PC = %#x (simulation cycle %lld)\n",core->configuration.id,KVX_SRF_PC,core->stats.cycles);
//   }
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_dflushl(void *this, uint64_t opnd1, uint64_t opnd2) {
  Except_THROW("Behavior_MEM_dflushl not implemented");
//   uint32_t address = opnd1;
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   if( core->runtime_cb.dcache_flush_line(core->runtime_cb.dcache_driver, address) == MEMORY_ACCESS_WARNING ) {
//     core->attributes.print(core->attributes.ostream,"WARNING data cache flush line on Proc %d at PC = %#x (simulation cycle %lld)\n",core->configuration.id,KVX_SRF_PC,core->stats.cycles);
//   }
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_dinval(void *this) {
  Except_THROW("Behavior_MEM_dinval not implemented");
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   core->runtime_cb.dcache_invalidate(core->runtime_cb.dcache_driver);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_dinvall(void *this, uint64_t opnd1, uint64_t opnd2) {
  Except_THROW("Behavior_MEM_dinvall not implemented");
//   uint32_t address = opnd1;
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   core->runtime_cb.dcache_invalidate_line(core->runtime_cb.dcache_driver,address);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_dpurge(void *this) {
  Except_THROW("Behavior_MEM_dinvall not implemented");
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   if( core->runtime_cb.dcache_purge(core->runtime_cb.dcache_driver) ) {
//     core->attributes.print(core->attributes.ostream,"WARNING data cache purge on Proc %d at PC = %#x (simulation cycle %lld)\n",core->configuration.id,KVX_SRF_PC,core->stats.cycles);
//   }
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_dpurgel(void *this, uint64_t opnd1, uint64_t opnd2) {
  Except_THROW("Behavior_MEM_dpurgel not implemented");
//   uint32_t address = opnd1;
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   if( core->runtime_cb.dcache_purge_line(core->runtime_cb.dcache_driver,address) ) {
//     core->attributes.print(core->attributes.ostream,"WARNING data cache purge line on Proc %d at PC = %#x (simulation cycle %lld)\n",core->configuration.id,KVX_SRF_PC,core->stats.cycles);
//   }
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_dtouchl(void *this, uint64_t opnd1, uint64_t opnd2) {
  Except_THROW("Behavior_MEM_dtouchl not implemented");
//   uint32_t address = opnd1;
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//
//   core->runtime_cb.dcache_prefetch(core->runtime_cb.dcache_driver,address);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_dzerol(void *this, uint64_t opnd1, uint64_t opnd2) {
  Except_THROW("Behavior_MEM_dzerol not implemented");
//   uint32_t address = opnd1;
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   core->runtime_cb.dcache_set_line_value(core->runtime_cb.dcache_driver,address,0);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_dzerolu(void *this, uint64_t opnd1, uint64_t opnd2) {
  Except_THROW("Behavior_MEM_dzerolu not implemented");
//   uint32_t address = opnd1;
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   core->runtime_cb.dcache_set_line_value(core->runtime_cb.dcache_driver,address,0);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_fence(void *this) {
  Except_THROW("Behavior_MEM_fence not implemented");
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//   Error_assert(this,0,("MEM_fence is not yet managed\n"));
//   CORE_SET_STATE(core, CORE_STATE_EXCEPTION, 4); /* SIGILL */
}
#endif//$XCC__h


#ifdef $XCC__h
static inline void
Behavior_MEM_iinval(void *this) {
  Except_THROW("Behavior_MEM_iinval not implemented");
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//
//   core->runtime_cb.icache_invalidate(core->runtime_cb.icache_driver);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_iinvals(void *this, uint64_t opnd1, uint64_t opnd2) {
  Except_THROW("Behavior_MEM_iinvals not implemented");
//   uint32_t address = opnd1;
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//
//   core->runtime_cb.icache_invalidate_line(core->runtime_cb.icache_driver,address);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_itouchl(void *this, uint64_t opnd1, uint64_t opnd2) {
  Except_THROW("Behavior_MEM_itouchl not implemented");
//   uint32_t address = opnd1;
//   Instr instr = (Instr)this;
//   core_t *core = instr->core;
//
//   core->runtime_cb.icache_prefetch(core->runtime_cb.icache_driver,address);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_MEM_load(void *this, uint64_t opnd1, uint64_t opnd2, uint64_t opnd3) {
  uintptr_t address = opnd1;
  uint64_t sizeOf = opnd2;
  uint64_t value = 0;
  memcpy((uint8_t*)&value, (uint8_t*)address, sizeOf);
  return value;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_store(void *this, uint64_t opnd1, uint64_t opnd2, uint64_t opnd3) {
  Interpreter inter = (Interpreter)this;
  uintptr_t address = opnd1;
  uint64_t sizeOf = opnd2;
  uint64_t value = opnd3;
  memcpy((void*)address, (uint8_t*)&value, sizeOf);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_MEM_swap(void *this, uint64_t opnd1, uint64_t opnd2, uint64_t opnd3) {
  Except_THROW("Behavior_MEM_swap not implemented");
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_MEM_swapu(void *this, uint64_t opnd1, uint64_t opnd2, uint64_t opnd3) {
  Except_THROW("Behavior_MEM_swapu not implemented");
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_wpurge(void *this) {
  Except_THROW("Behavior_MEM_wpurge not implemented");
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_throw_MISALIGN(void *this, uint64_t opnd1) {
  Except_THROW("Behavior_throw_MISALIGN not implemented");
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_break(void *this) {
  Except_THROW("Behavior_break not implemented");
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_clear(void *this, uint64_t opnd1, uint64_t opnd2) {
  Except_THROW("Behavior_clear not implemented");
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_idle(void *this, uint64_t opnd1) {
  Except_THROW("Behavior_idle not implemented");
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_illegal(void *this)
{
  Except_THROW("Behavior_illegal should not be called");
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_illegal1(void *this, uint64_t opnd1)
{
  Except_THROW("Behavior_illegal1 should not be called");
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_illegal2(void *this, uint64_t opnd1, uint64_t opnd2)
{
  Except_THROW("Behavior_illegal2 should not be called");
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_illegal3(void *this, uint64_t opnd1, uint64_t opnd2,  uint64_t opnd3)
{
  Except_THROW("Behavior_illegal3 should not be called");
  return 0;
}
#endif//$XCC__h


// This function access the regfile and reads the
// corresponding bits defined by an offset and a size.
#ifdef $XCC__h
static inline uint64_t
Behavior_readFromRegister_PC(void *this, unsigned stage, unsigned offset, unsigned extent, unsigned size) {
  Interpreter inter = (Interpreter)this;
  unsigned int mask = (1 << extent) - 1;
  // You can only access the whole PC register
  assert((extent <= 1) && (offset == 0));
  return (KVX_PC);
}
#endif//$XCC__h

// This function access the regfile and reads the
// corresponding bits defined by an offset and a size.
#ifdef $XCC__h
static inline uint64_t
Behavior_readFromRegister_NPC(void *this, unsigned stage, unsigned offset, unsigned extent, unsigned size) {
  Interpreter inter = (Interpreter)this;
  unsigned int mask = (1 << extent) - 1;
  // You can only access the whole next_pc register
  assert((extent <= 1) && (offset == 0));
  return (KVX_NPC);
}
#endif//$XCC__h

// This function access the regfile and reads the
// corresponding bits defined by an offset and a size.
#ifdef $XCC__h
static inline uint64_t
Behavior_readFromRegister_PS(void *this, unsigned stage, unsigned offset, unsigned extent, unsigned size) {
  Interpreter inter = (Interpreter)this;
  unsigned int mask = (1 << extent) - 1;
  Except_THROW("Cannot access PS yet!");
  return 0;
}
#endif//$XCC__h

// This function access the regfile and reads the
// corresponding bits defined by an offset and a size.
#ifdef $XCC__h
static inline uint64_t
Behavior_readFromRegister_CS(void *this, unsigned stage, unsigned offset, unsigned extent, unsigned size) {
  Interpreter inter = (Interpreter)this;
  unsigned int mask = (1 << extent) - 1;
//       return (KVX_REG(Register_kvx_CS >> offset) & mask);
  Except_THROW("Cannot access CS yet!");
  return 0;
}
#endif//$XCC__h

// This function access the regfile and reads the
// corresponding bits defined by an offset and a size.
#ifdef $XCC__h
static inline uint64_t
Behavior_readFromRegister_SRF(void *this, unsigned stage, unsigned offset, unsigned extent, unsigned size) {
  Interpreter inter = (Interpreter)this;
  unsigned int mask = (1 << extent) - 1;
  uint64_t returnValue = 0;
  size_t i = 0;
  assert((extent * size) < 128);
  for (i = 0; i < extent; ++i) {
    returnValue = (returnValue << (size * i)) | KVX_SRF(offset + i);
  }
  return returnValue;
}
#endif//$XCC__h

// This function access the specified regfile and writes the value to
// the bits defined by an offset and a size.
#ifdef $XCC__h
static inline void
Behavior_writeToRegister_CS(void *this, unsigned stage, unsigned offset, unsigned extent, unsigned size, uint64_t value) {
  Interpreter inter = (Interpreter)this;
  Except_THROW("Cannot access CS yet!");
}
#endif//$XCC__h

// This function access the specified regfile and writes the value to
// the bits defined by an offset and a size.
#ifdef $XCC__h
static inline void
Behavior_writeToRegister_SRF(void *this, unsigned stage, unsigned offset, unsigned extent, unsigned size, uint64_t value) {
  Interpreter inter = (Interpreter)this;
  Except_THROW("Cannot access SRF yet!");
}
#endif//$XCC__h

// This function access the specified regfile and writes the value to
// the bits defined by an offset and a size.
#ifdef $XCC__h
static inline void
Behavior_writeToRegister_NPC(void *this, unsigned stage, unsigned offset, unsigned extent, unsigned size, uint64_t value) {
  Interpreter inter = (Interpreter)this;
  // You can only access the whole next_pc register
  assert((extent <= 1) && (offset == 0));
  KVX_NPC = (value);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_readOperand(void *this, int opnd_idx) {
   Interpreter inter = (Interpreter)this;
   return Interpreter_OPERANDS(inter)[opnd_idx];
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_writeOperand(void *this, int opnd_idx, uint64_t value) {
   Interpreter inter = (Interpreter)this;
   Interpreter_OPERANDS(inter)[opnd_idx] = value;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_notify(void *this, uint64_t opnd1, uint64_t opnd2) {
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_raise(void *this, uint64_t opnd1, uint64_t opnd2) {
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_readRegister_GPR(void *this, unsigned stage, int opnd_idx, int register_id) {
//   TODO: SSA values support needed
  Interpreter inter = (Interpreter)this;
  Interpreter_OPERANDS(inter)[opnd_idx] = KVX_GPR(register_id);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_readRegister_PGR(void *this, unsigned stage, int opnd_idx, int register_id) {
//   TODO: SSA values support needed
  Interpreter inter = (Interpreter)this;
  Interpreter_OPERANDS(inter)[opnd_idx] = ((uint64_t)(KVX_GPR(2*register_id + 1)) << 32)| (uint64_t)(KVX_GPR(2*register_id));
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_readRegister_SRF(void *this, unsigned stage, int opnd_idx, int register_id) {
//   TODO: SSA values support needed
  Interpreter inter = (Interpreter)this;
  Interpreter_OPERANDS(inter)[opnd_idx] = KVX_SRF(register_id);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_readRegister_PS(void *this, unsigned stage, int opnd_idx, int register_id) {
//   TODO: SSA values support needed
  Interpreter inter = (Interpreter)this;
  Except_THROW("Cannot access PS yet!");
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_writeRegister_GPR(void *this, unsigned stage, int opnd_idx, int register_id) {
//   TODO: Virtual SSA registers support needed
  Interpreter inter = (Interpreter)this;
  KVX_GPR(register_id) = Interpreter_OPERANDS(inter)[opnd_idx];
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_writeRegister_PGR(void *this, unsigned stage, int opnd_idx, int register_id) {
//   TODO: Virtual SSA registers support needed
  Interpreter inter = (Interpreter)this;
  KVX_GPR(2*register_id) = Interpreter_OPERANDS(inter)[opnd_idx];
  KVX_GPR(2*register_id + 1) = ((uint64_t)Interpreter_OPERANDS(inter)[opnd_idx]) >> 32;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_writeRegister_SRF(void *this, unsigned stage, int opnd_idx, int register_id) {
//   TODO: Virtual SSA registers support needed
  Interpreter inter = (Interpreter)this;
  KVX_SRF(register_id) = Interpreter_OPERANDS(inter)[opnd_idx];
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_writeRegister_NRF(void *this, unsigned stage, int opnd_idx, int register_id) {
//   TODO: Virtual SSA registers support needed
  Interpreter inter = (Interpreter)this;
  Except_THROW("Cannot access NRF yet!");
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_writeRegister_PS(void *this, unsigned stage, int opnd_idx, int register_id) {
//   TODO: Virtual SSA registers support needed
  Interpreter inter = (Interpreter)this;
  Except_THROW("Cannot access PS yet!");
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_syscall(void *this, uint64_t opnd1) {
//   syscall(this, opnd1);
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_waitall(void *this, uint64_t opnd1, uint64_t opnd2) {
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_waitany(void *this, uint64_t opnd1, uint64_t opnd2) {
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline uint64_t
Behavior_wantany(void *this, uint64_t opnd1, uint64_t opnd2) {
  return 0;
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_invalitlb(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_invaldtlb(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_readtlb(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_indexjtlb(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_MEM_winval(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_throw_OPCODE(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_throw_PRIVILEGE(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_writetlb(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_indexltlb(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_probetlb(void *this)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline void
Behavior_syncgroup(void *this, uint64_t opnd1, uint64_t opnd2, uint64_t opnd3)
{
}
#endif//$XCC__h

#ifdef $XCC__h
static inline bool
Behavior_wfx_check_access(void *this, Int256_ srf_reg_index)
{
}
#endif//$XCC__h

