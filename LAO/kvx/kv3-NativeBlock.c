/*
 * kv3/NativeBlock.xcc
 *  (c) Copyright 2010-2018 Kalray SA.
 * Automatically generated from the Machine Description System (MDS).
 */

#include "MDT/MDT_.h"
#include "AIR/AIR_.h"

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_(NativeBlock this, Opcode opcode);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_(NativeBlock this, Opcode opcode)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerZ(NativeBlock this, Opcode opcode, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerZ(NativeBlock this, Opcode opcode, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_sysnumber(NativeBlock this, Opcode opcode, ImmediateValue sysnumber);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_sysnumber(NativeBlock this, Opcode opcode, ImmediateValue sysnumber)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)sysnumber;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerZ_pcrel17(NativeBlock this, Opcode opcode, Register registerZ, ImmediateValue pcrel17);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerZ_pcrel17(NativeBlock this, Opcode opcode, Register registerZ, ImmediateValue pcrel17)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerZ;
  decoded[1] = (OperandDecoded)pcrel17;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_systemT2_registerZ(NativeBlock this, Opcode opcode, Register systemT2, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_systemT2_registerZ(NativeBlock this, Opcode opcode, Register systemT2, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)systemT2;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_systemAlone_registerZ(NativeBlock this, Opcode opcode, Register systemAlone, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_systemAlone_registerZ(NativeBlock this, Opcode opcode, Register systemAlone, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)systemAlone;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_systemT4_registerZ(NativeBlock this, Opcode opcode, Register systemT4, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_systemT4_registerZ(NativeBlock this, Opcode opcode, Register systemT4, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)systemT4;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerZ_systemS2(NativeBlock this, Opcode opcode, Register registerZ, Register systemS2);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerZ_systemS2(NativeBlock this, Opcode opcode, Register registerZ, Register systemS2)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerZ;
  decoded[1] = (OperandDecoded)systemS2;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerZ_systemS3(NativeBlock this, Opcode opcode, Register registerZ, Register systemS3);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerZ_systemS3(NativeBlock this, Opcode opcode, Register registerZ, Register systemS3)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerZ;
  decoded[1] = (OperandDecoded)systemS3;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_systemT3_registerZ(NativeBlock this, Opcode opcode, Register systemT3, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_systemT3_registerZ(NativeBlock this, Opcode opcode, Register systemT3, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)systemT3;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_systemRA_registerZ(NativeBlock this, Opcode opcode, Register systemRA, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_systemRA_registerZ(NativeBlock this, Opcode opcode, Register systemRA, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)systemRA;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerZ_systemS4(NativeBlock this, Opcode opcode, Register registerZ, Register systemS4);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerZ_systemS4(NativeBlock this, Opcode opcode, Register registerZ, Register systemS4)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerZ;
  decoded[1] = (OperandDecoded)systemS4;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerZ_systemAlone(NativeBlock this, Opcode opcode, Register registerZ, Register systemAlone);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerZ_systemAlone(NativeBlock this, Opcode opcode, Register registerZ, Register systemAlone)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerZ;
  decoded[1] = (OperandDecoded)systemAlone;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_pcrel27(NativeBlock this, Opcode opcode, ImmediateValue pcrel27);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_pcrel27(NativeBlock this, Opcode opcode, ImmediateValue pcrel27)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)pcrel27;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_branchcond_registerZ_pcrel17(NativeBlock this, Opcode opcode, ModifierMember branchcond, Register registerZ, ImmediateValue pcrel17);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_branchcond_registerZ_pcrel17(NativeBlock this, Opcode opcode, ModifierMember branchcond, Register registerZ, ImmediateValue pcrel17)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)branchcond;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)pcrel17;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_registerW_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerW, ImmediateValue signed10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_registerW_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerW, ImmediateValue signed10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)signed10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_registerW_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerW, ImmediateValue upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_registerW_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerW, ImmediateValue upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)upper27_lower10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_registerW_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerW, ImmediateValue extend27_upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_registerW_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerW, ImmediateValue extend27_upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)extend27_upper27_lower10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerW_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerW, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerW_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerW, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerW;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerW_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerW, ImmediateValue offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerW_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerW, ImmediateValue offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerW;
  decoded[4] = (OperandDecoded)offset27;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerW_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerW, ImmediateValue extend27_offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerW_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerW, ImmediateValue extend27_offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerW;
  decoded[4] = (OperandDecoded)extend27_offset27;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_doscale_registerW_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember doscale, Register registerW, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_doscale_registerW_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember doscale, Register registerW, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)doscale;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)registerY;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_registerM_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerM, ImmediateValue signed10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_registerM_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerM, ImmediateValue signed10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)registerM;
  decoded[2] = (OperandDecoded)signed10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_registerM_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerM, ImmediateValue upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_registerM_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerM, ImmediateValue upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)registerM;
  decoded[2] = (OperandDecoded)upper27_lower10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_registerM_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerM, ImmediateValue extend27_upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_registerM_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerM, ImmediateValue extend27_upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)registerM;
  decoded[2] = (OperandDecoded)extend27_upper27_lower10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerM_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerM, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerM_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerM, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerM;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerM_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerM, ImmediateValue offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerM_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerM, ImmediateValue offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerM;
  decoded[4] = (OperandDecoded)offset27;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerM_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerM, ImmediateValue extend27_offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerM_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerM, ImmediateValue extend27_offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerM;
  decoded[4] = (OperandDecoded)extend27_offset27;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_doscale_registerM_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember doscale, Register registerM, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_doscale_registerM_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember doscale, Register registerM, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)doscale;
  decoded[2] = (OperandDecoded)registerM;
  decoded[3] = (OperandDecoded)registerY;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_registerN_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerN, ImmediateValue signed10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_registerN_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerN, ImmediateValue signed10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)registerN;
  decoded[2] = (OperandDecoded)signed10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_registerN_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerN, ImmediateValue upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_registerN_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerN, ImmediateValue upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)registerN;
  decoded[2] = (OperandDecoded)upper27_lower10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_registerN_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerN, ImmediateValue extend27_upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_registerN_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, Register registerN, ImmediateValue extend27_upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)registerN;
  decoded[2] = (OperandDecoded)extend27_upper27_lower10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerN_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerN, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerN_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerN, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerN;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerN_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerN, ImmediateValue offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerN_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerN, ImmediateValue offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerN;
  decoded[4] = (OperandDecoded)offset27;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerN_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerN, ImmediateValue extend27_offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_lsucond_registerY_registerN_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember lsucond, Register registerY, Register registerN, ImmediateValue extend27_offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerN;
  decoded[4] = (OperandDecoded)extend27_offset27;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_variant_doscale_registerN_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember doscale, Register registerN, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_variant_doscale_registerN_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember variant, ModifierMember doscale, Register registerN, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)variant;
  decoded[1] = (OperandDecoded)doscale;
  decoded[2] = (OperandDecoded)registerN;
  decoded[3] = (OperandDecoded)registerY;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_speculate_registerA_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, Register registerA, ImmediateValue signed10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_speculate_registerA_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, Register registerA, ImmediateValue signed10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)speculate;
  decoded[1] = (OperandDecoded)registerA;
  decoded[2] = (OperandDecoded)signed10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_speculate_registerA_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, Register registerA, ImmediateValue upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_speculate_registerA_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, Register registerA, ImmediateValue upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)speculate;
  decoded[1] = (OperandDecoded)registerA;
  decoded[2] = (OperandDecoded)upper27_lower10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_speculate_registerA_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, Register registerA, ImmediateValue extend27_upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_speculate_registerA_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, Register registerA, ImmediateValue extend27_upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)speculate;
  decoded[1] = (OperandDecoded)registerA;
  decoded[2] = (OperandDecoded)extend27_upper27_lower10;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_speculate_lsucond_registerY_registerA_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerA, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_speculate_lsucond_registerY_registerA_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerA, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)speculate;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerA;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_speculate_lsucond_registerY_registerA_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerA, ImmediateValue offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_speculate_lsucond_registerY_registerA_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerA, ImmediateValue offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)speculate;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerA;
  decoded[4] = (OperandDecoded)offset27;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_speculate_lsucond_registerY_registerA_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerA, ImmediateValue extend27_offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_speculate_lsucond_registerY_registerA_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerA, ImmediateValue extend27_offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)speculate;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerA;
  decoded[4] = (OperandDecoded)extend27_offset27;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_speculate_doscale_registerA_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, ModifierMember doscale, Register registerA, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_speculate_doscale_registerA_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember speculate, ModifierMember doscale, Register registerA, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)speculate;
  decoded[1] = (OperandDecoded)doscale;
  decoded[2] = (OperandDecoded)registerA;
  decoded[3] = (OperandDecoded)registerY;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_column_speculate_registerAq_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, Register registerAq, ImmediateValue signed10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_column_speculate_registerAq_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, Register registerAq, ImmediateValue signed10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)column;
  decoded[1] = (OperandDecoded)speculate;
  decoded[2] = (OperandDecoded)registerAq;
  decoded[3] = (OperandDecoded)signed10;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_column_speculate_registerAq_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, Register registerAq, ImmediateValue upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_column_speculate_registerAq_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, Register registerAq, ImmediateValue upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)column;
  decoded[1] = (OperandDecoded)speculate;
  decoded[2] = (OperandDecoded)registerAq;
  decoded[3] = (OperandDecoded)upper27_lower10;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_column_speculate_registerAq_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, Register registerAq, ImmediateValue extend27_upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_column_speculate_registerAq_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, Register registerAq, ImmediateValue extend27_upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)column;
  decoded[1] = (OperandDecoded)speculate;
  decoded[2] = (OperandDecoded)registerAq;
  decoded[3] = (OperandDecoded)extend27_upper27_lower10;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_column_speculate_lsucond_registerY_registerAq_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerAq, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_column_speculate_lsucond_registerY_registerAq_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerAq, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)column;
  decoded[1] = (OperandDecoded)speculate;
  decoded[2] = (OperandDecoded)lsucond;
  decoded[3] = (OperandDecoded)registerY;
  decoded[4] = (OperandDecoded)registerAq;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_column_speculate_lsucond_registerY_registerAq_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerAq, ImmediateValue offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_column_speculate_lsucond_registerY_registerAq_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerAq, ImmediateValue offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)column;
  decoded[1] = (OperandDecoded)speculate;
  decoded[2] = (OperandDecoded)lsucond;
  decoded[3] = (OperandDecoded)registerY;
  decoded[4] = (OperandDecoded)registerAq;
  decoded[5] = (OperandDecoded)offset27;
  decoded[6] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_column_speculate_lsucond_registerY_registerAq_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerAq, ImmediateValue extend27_offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_column_speculate_lsucond_registerY_registerAq_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, ModifierMember lsucond, Register registerY, Register registerAq, ImmediateValue extend27_offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)column;
  decoded[1] = (OperandDecoded)speculate;
  decoded[2] = (OperandDecoded)lsucond;
  decoded[3] = (OperandDecoded)registerY;
  decoded[4] = (OperandDecoded)registerAq;
  decoded[5] = (OperandDecoded)extend27_offset27;
  decoded[6] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_column_speculate_doscale_registerAq_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, ModifierMember doscale, Register registerAq, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_column_speculate_doscale_registerAq_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember column, ModifierMember speculate, ModifierMember doscale, Register registerAq, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)column;
  decoded[1] = (OperandDecoded)speculate;
  decoded[2] = (OperandDecoded)doscale;
  decoded[3] = (OperandDecoded)registerAq;
  decoded[4] = (OperandDecoded)registerY;
  decoded[5] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ_registerT(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ, Register registerT);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ_registerT(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ, Register registerT)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)signed10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerT;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ_registerT(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ, Register registerT);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ_registerT(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ, Register registerT)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerT;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ_registerT(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ, Register registerT);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ_registerT(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ, Register registerT)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)extend27_upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerT;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_doscale_registerY_registerZ_registerT(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerY, Register registerZ, Register registerT);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_doscale_registerY_registerZ_registerT(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerY, Register registerZ, Register registerT)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)doscale;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerT;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ_registerT(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ, Register registerT);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ_registerT(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ, Register registerT)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerT;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ_registerT(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerT);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ_registerT(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerT)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)offset27;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerT;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ_registerT(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerT);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ_registerT(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerT)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)extend27_offset27;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerT;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ_registerU(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ_registerU(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)signed10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ_registerU(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ_registerU(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ_registerU(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ_registerU(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)extend27_upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_doscale_registerY_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerY, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_doscale_registerY_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerY, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)doscale;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)offset27;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)extend27_offset27;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ_registerV(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ, Register registerV);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ_registerV(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ, Register registerV)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)signed10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerV;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ_registerV(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ, Register registerV);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ_registerV(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ, Register registerV)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerV;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ_registerV(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ, Register registerV);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ_registerV(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ, Register registerV)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)extend27_upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerV;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_doscale_registerY_registerZ_registerV(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerY, Register registerZ, Register registerV);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_doscale_registerY_registerZ_registerV(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerY, Register registerZ, Register registerV)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)doscale;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerV;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ_registerV(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ, Register registerV);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ_registerV(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ, Register registerV)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerV;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ_registerV(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerV);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ_registerV(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerV)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)offset27;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerV;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ_registerV(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerV);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ_registerV(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerV)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)extend27_offset27;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerV;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ_registerE(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ, Register registerE);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ_registerE(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ, Register registerE)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)signed10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerE;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ_registerE(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ, Register registerE);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ_registerE(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ, Register registerE)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerE;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ_registerE(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ, Register registerE);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ_registerE(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ, Register registerE)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)extend27_upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerE;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_doscale_registerY_registerZ_registerE(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerY, Register registerZ, Register registerE);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_doscale_registerY_registerZ_registerE(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerY, Register registerZ, Register registerE)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)doscale;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerE;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ_registerE(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ, Register registerE);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ_registerE(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ, Register registerE)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerE;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ_registerE(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerE);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ_registerE(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerE)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)offset27;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerE;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ_registerE(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerE);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ_registerE(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerE)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)extend27_offset27;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerE;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_signed10_registerZ(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue signed10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_signed10_registerZ(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue signed10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)signed10;
  decoded[2] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)upper27_lower10;
  decoded[2] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue extend27_upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue extend27_upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)extend27_upper27_lower10;
  decoded[2] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_doscale_registerW_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerW, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_doscale_registerW_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember doscale, Register registerW, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)doscale;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerW_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerW, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerW_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerW, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerW_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerW, ImmediateValue offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerW_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerW, ImmediateValue offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)offset27;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerW_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerW, ImmediateValue extend27_offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerW_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerW, ImmediateValue extend27_offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)extend27_offset27;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_signed10_registerZ(NativeBlock this, Opcode opcode, ImmediateValue signed10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)signed10;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ImmediateValue upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ImmediateValue extend27_upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)extend27_upper27_lower10;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerY_registerZ(NativeBlock this, Opcode opcode, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerY_registerZ(NativeBlock this, Opcode opcode, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerY;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)offset27;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_lsucond_registerY_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)lsucond;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)extend27_offset27;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_signed10(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue signed10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_signed10(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue signed10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)signed10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_upper27_lower10(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_upper27_lower10(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_extend27_upper27_lower10(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue extend27_upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_extend27_upper27_lower10(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue extend27_upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)extend27_upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_upper27_lower5(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue upper27_lower5);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_upper27_lower5(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue upper27_lower5)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)upper27_lower5;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_splat32_registerW_registerZ_upper27_lower5(NativeBlock this, Opcode opcode, ModifierMember splat32, Register registerW, Register registerZ, ImmediateValue upper27_lower5);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_splat32_registerW_registerZ_upper27_lower5(NativeBlock this, Opcode opcode, ModifierMember splat32, Register registerW, Register registerZ, ImmediateValue upper27_lower5)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)splat32;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)upper27_lower5;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ(NativeBlock this, Opcode opcode, Register registerW, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ(NativeBlock this, Opcode opcode, Register registerW, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_silent2_registerW_registerZ(NativeBlock this, Opcode opcode, ModifierMember silent2, Register registerW, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_silent2_registerW_registerZ(NativeBlock this, Opcode opcode, ModifierMember silent2, Register registerW, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)silent2;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_rounding2_silent2_registerW_registerZ(NativeBlock this, Opcode opcode, ModifierMember rounding2, ModifierMember silent2, Register registerW, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_rounding2_silent2_registerW_registerZ(NativeBlock this, Opcode opcode, ModifierMember rounding2, ModifierMember silent2, Register registerW, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)rounding2;
  decoded[1] = (OperandDecoded)silent2;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_rounding2_silent2_registerW_registerP(NativeBlock this, Opcode opcode, ModifierMember rounding2, ModifierMember silent2, Register registerW, Register registerP);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_rounding2_silent2_registerW_registerP(NativeBlock this, Opcode opcode, ModifierMember rounding2, ModifierMember silent2, Register registerW, Register registerP)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)rounding2;
  decoded[1] = (OperandDecoded)silent2;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)registerP;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_stopbit2_stopbit4_startbit(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue stopbit2_stopbit4, ImmediateValue startbit);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_stopbit2_stopbit4_startbit(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue stopbit2_stopbit4, ImmediateValue startbit)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)stopbit2_stopbit4;
  decoded[3] = (OperandDecoded)startbit;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_unsigned6(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue unsigned6);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_registerZ_unsigned6(NativeBlock this, Opcode opcode, Register registerW, Register registerZ, ImmediateValue unsigned6)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)unsigned6;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_signed10(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, ImmediateValue signed10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_signed10(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, ImmediateValue signed10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)comparison;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)signed10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_upper27_lower10(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, ImmediateValue upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_upper27_lower10(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, ImmediateValue upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)comparison;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_extend27_upper27_lower10(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, ImmediateValue extend27_upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_extend27_upper27_lower10(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, ImmediateValue extend27_upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)comparison;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)extend27_upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)comparison;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_upper27_lower5(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, ImmediateValue upper27_lower5);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_comparison_registerW_registerZ_upper27_lower5(NativeBlock this, Opcode opcode, ModifierMember comparison, Register registerW, Register registerZ, ImmediateValue upper27_lower5)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)comparison;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)upper27_lower5;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_scalarcond_registerZ_registerW_signed10(NativeBlock this, Opcode opcode, ModifierMember scalarcond, Register registerZ, Register registerW, ImmediateValue signed10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_scalarcond_registerZ_registerW_signed10(NativeBlock this, Opcode opcode, ModifierMember scalarcond, Register registerZ, Register registerW, ImmediateValue signed10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)scalarcond;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)signed10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_scalarcond_registerZ_registerW_upper27_lower10(NativeBlock this, Opcode opcode, ModifierMember scalarcond, Register registerZ, Register registerW, ImmediateValue upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_scalarcond_registerZ_registerW_upper27_lower10(NativeBlock this, Opcode opcode, ModifierMember scalarcond, Register registerZ, Register registerW, ImmediateValue upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)scalarcond;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_scalarcond_registerZ_registerW_extend27_upper27_lower10(NativeBlock this, Opcode opcode, ModifierMember scalarcond, Register registerZ, Register registerW, ImmediateValue extend27_upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_scalarcond_registerZ_registerW_extend27_upper27_lower10(NativeBlock this, Opcode opcode, ModifierMember scalarcond, Register registerZ, Register registerW, ImmediateValue extend27_upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)scalarcond;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)extend27_upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_scalarcond_registerZ_registerW_registerY(NativeBlock this, Opcode opcode, ModifierMember scalarcond, Register registerZ, Register registerW, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_scalarcond_registerZ_registerW_registerY(NativeBlock this, Opcode opcode, ModifierMember scalarcond, Register registerZ, Register registerW, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)scalarcond;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_simdcond_registerZ_registerW_registerY(NativeBlock this, Opcode opcode, ModifierMember simdcond, Register registerZ, Register registerW, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_simdcond_registerZ_registerW_registerY(NativeBlock this, Opcode opcode, ModifierMember simdcond, Register registerZ, Register registerW, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)simdcond;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_signed16(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue signed16);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_signed16(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue signed16)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)signed16;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_extend6_upper27_lower10(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue extend6_upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_extend6_upper27_lower10(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue extend6_upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)extend6_upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerW_extend27_upper27_lower10(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue extend27_upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerW_extend27_upper27_lower10(NativeBlock this, Opcode opcode, Register registerW, ImmediateValue extend27_upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerW;
  decoded[1] = (OperandDecoded)extend27_upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_floatcomp_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember floatcomp, Register registerW, Register registerZ, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_floatcomp_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember floatcomp, Register registerW, Register registerZ, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)floatcomp;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_floatcomp_registerW_registerZ_upper27_lower5(NativeBlock this, Opcode opcode, ModifierMember floatcomp, Register registerW, Register registerZ, ImmediateValue upper27_lower5);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_floatcomp_registerW_registerZ_upper27_lower5(NativeBlock this, Opcode opcode, ModifierMember floatcomp, Register registerW, Register registerZ, ImmediateValue upper27_lower5)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)floatcomp;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)upper27_lower5;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_silent2_registerW_registerP(NativeBlock this, Opcode opcode, ModifierMember silent2, Register registerW, Register registerP);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_silent2_registerW_registerP(NativeBlock this, Opcode opcode, ModifierMember silent2, Register registerW, Register registerP)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)silent2;
  decoded[1] = (OperandDecoded)registerW;
  decoded[2] = (OperandDecoded)registerP;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerW_registerZ_unsigned6(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerW, Register registerZ, ImmediateValue unsigned6);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerW_registerZ_unsigned6(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerW, Register registerZ, ImmediateValue unsigned6)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)rounding;
  decoded[1] = (OperandDecoded)silent;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)unsigned6;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerM_registerZ_registerY(NativeBlock this, Opcode opcode, Register registerM, Register registerZ, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerM_registerZ_registerY(NativeBlock this, Opcode opcode, Register registerM, Register registerZ, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerM;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerM_registerZ_signed10(NativeBlock this, Opcode opcode, Register registerM, Register registerZ, ImmediateValue signed10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerM_registerZ_signed10(NativeBlock this, Opcode opcode, Register registerM, Register registerZ, ImmediateValue signed10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerM;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)signed10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerM_registerZ_upper27_lower10(NativeBlock this, Opcode opcode, Register registerM, Register registerZ, ImmediateValue upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerM_registerZ_upper27_lower10(NativeBlock this, Opcode opcode, Register registerM, Register registerZ, ImmediateValue upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerM;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerM_registerZ_extend27_upper27_lower10(NativeBlock this, Opcode opcode, Register registerM, Register registerZ, ImmediateValue extend27_upper27_lower10);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerM_registerZ_extend27_upper27_lower10(NativeBlock this, Opcode opcode, Register registerM, Register registerZ, ImmediateValue extend27_upper27_lower10)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerM;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)extend27_upper27_lower10;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerM_registerP_registerO(NativeBlock this, Opcode opcode, Register registerM, Register registerP, Register registerO);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerM_registerP_registerO(NativeBlock this, Opcode opcode, Register registerM, Register registerP, Register registerO)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerM;
  decoded[1] = (OperandDecoded)registerP;
  decoded[2] = (OperandDecoded)registerO;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerW, Register registerZ, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerW, Register registerZ, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)rounding;
  decoded[1] = (OperandDecoded)silent;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerM_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerZ, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerM_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerZ, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)rounding;
  decoded[1] = (OperandDecoded)silent;
  decoded[2] = (OperandDecoded)registerM;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerM_registerP_registerO(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerP, Register registerO);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerM_registerP_registerO(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerP, Register registerO)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)rounding;
  decoded[1] = (OperandDecoded)silent;
  decoded[2] = (OperandDecoded)registerM;
  decoded[3] = (OperandDecoded)registerP;
  decoded[4] = (OperandDecoded)registerO;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerN_registerR(NativeBlock this, Opcode opcode, Register registerN, Register registerR);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerN_registerR(NativeBlock this, Opcode opcode, Register registerN, Register registerR)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerN;
  decoded[1] = (OperandDecoded)registerR;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerAl_registerZ_registerY(NativeBlock this, Opcode opcode, Register registerAl, Register registerZ, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerAl_registerZ_registerY(NativeBlock this, Opcode opcode, Register registerAl, Register registerZ, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerAl;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerAh_registerZ_registerY(NativeBlock this, Opcode opcode, Register registerAh, Register registerZ, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerAh_registerZ_registerY(NativeBlock this, Opcode opcode, Register registerAh, Register registerZ, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerAh;
  decoded[1] = (OperandDecoded)registerZ;
  decoded[2] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBe_registerCo_byteshift(NativeBlock this, Opcode opcode, Register registerN, Register registerBe, Register registerCo, ImmediateValue byteshift);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBe_registerCo_byteshift(NativeBlock this, Opcode opcode, Register registerN, Register registerBe, Register registerCo, ImmediateValue byteshift)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerN;
  decoded[1] = (OperandDecoded)registerBe;
  decoded[2] = (OperandDecoded)registerCo;
  decoded[3] = (OperandDecoded)byteshift;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBo_registerCe_byteshift(NativeBlock this, Opcode opcode, Register registerN, Register registerBo, Register registerCe, ImmediateValue byteshift);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBo_registerCe_byteshift(NativeBlock this, Opcode opcode, Register registerN, Register registerBo, Register registerCe, ImmediateValue byteshift)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerN;
  decoded[1] = (OperandDecoded)registerBo;
  decoded[2] = (OperandDecoded)registerCe;
  decoded[3] = (OperandDecoded)byteshift;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBe_registerCo_registerZ(NativeBlock this, Opcode opcode, Register registerN, Register registerBe, Register registerCo, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBe_registerCo_registerZ(NativeBlock this, Opcode opcode, Register registerN, Register registerBe, Register registerCo, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerN;
  decoded[1] = (OperandDecoded)registerBe;
  decoded[2] = (OperandDecoded)registerCo;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBo_registerCe_registerZ(NativeBlock this, Opcode opcode, Register registerN, Register registerBo, Register registerCe, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBo_registerCe_registerZ(NativeBlock this, Opcode opcode, Register registerN, Register registerBo, Register registerCe, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerN;
  decoded[1] = (OperandDecoded)registerBo;
  decoded[2] = (OperandDecoded)registerCe;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBe_registerCo_byteshift(NativeBlock this, Opcode opcode, Register registerA, Register registerBe, Register registerCo, ImmediateValue byteshift);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBe_registerCo_byteshift(NativeBlock this, Opcode opcode, Register registerA, Register registerBe, Register registerCo, ImmediateValue byteshift)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA;
  decoded[1] = (OperandDecoded)registerBe;
  decoded[2] = (OperandDecoded)registerCo;
  decoded[3] = (OperandDecoded)byteshift;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBo_registerCe_byteshift(NativeBlock this, Opcode opcode, Register registerA, Register registerBo, Register registerCe, ImmediateValue byteshift);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBo_registerCe_byteshift(NativeBlock this, Opcode opcode, Register registerA, Register registerBo, Register registerCe, ImmediateValue byteshift)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA;
  decoded[1] = (OperandDecoded)registerBo;
  decoded[2] = (OperandDecoded)registerCe;
  decoded[3] = (OperandDecoded)byteshift;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBe_registerCo_registerZ(NativeBlock this, Opcode opcode, Register registerA, Register registerBe, Register registerCo, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBe_registerCo_registerZ(NativeBlock this, Opcode opcode, Register registerA, Register registerBe, Register registerCo, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA;
  decoded[1] = (OperandDecoded)registerBe;
  decoded[2] = (OperandDecoded)registerCo;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBo_registerCe_registerZ(NativeBlock this, Opcode opcode, Register registerA, Register registerBo, Register registerCe, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBo_registerCe_registerZ(NativeBlock this, Opcode opcode, Register registerA, Register registerBo, Register registerCe, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA;
  decoded[1] = (OperandDecoded)registerBo;
  decoded[2] = (OperandDecoded)registerCe;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerAq_registerBq(NativeBlock this, Opcode opcode, Register registerAq, Register registerBq);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerAq_registerBq(NativeBlock this, Opcode opcode, Register registerAq, Register registerBq)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerAq;
  decoded[1] = (OperandDecoded)registerBq;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerAp_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerAp, Register registerBp, Register registerC, Register registerD);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerAp_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerAp, Register registerBp, Register registerC, Register registerD)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerAp;
  decoded[1] = (OperandDecoded)registerBp;
  decoded[2] = (OperandDecoded)registerC;
  decoded[3] = (OperandDecoded)registerD;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerAq_registerBq_registerC_registerD(NativeBlock this, Opcode opcode, Register registerAq, Register registerBq, Register registerC, Register registerD);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerAq_registerBq_registerC_registerD(NativeBlock this, Opcode opcode, Register registerAq, Register registerBq, Register registerC, Register registerD)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerAq;
  decoded[1] = (OperandDecoded)registerBq;
  decoded[2] = (OperandDecoded)registerC;
  decoded[3] = (OperandDecoded)registerD;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA0_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerA0, Register registerBp, Register registerC, Register registerD);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA0_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerA0, Register registerBp, Register registerC, Register registerD)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA0;
  decoded[1] = (OperandDecoded)registerBp;
  decoded[2] = (OperandDecoded)registerC;
  decoded[3] = (OperandDecoded)registerD;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA1_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerA1, Register registerBp, Register registerC, Register registerD);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA1_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerA1, Register registerBp, Register registerC, Register registerD)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA1;
  decoded[1] = (OperandDecoded)registerBp;
  decoded[2] = (OperandDecoded)registerC;
  decoded[3] = (OperandDecoded)registerD;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA2_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerA2, Register registerBp, Register registerC, Register registerD);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA2_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerA2, Register registerBp, Register registerC, Register registerD)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA2;
  decoded[1] = (OperandDecoded)registerBp;
  decoded[2] = (OperandDecoded)registerC;
  decoded[3] = (OperandDecoded)registerD;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA3_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerA3, Register registerBp, Register registerC, Register registerD);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA3_registerBp_registerC_registerD(NativeBlock this, Opcode opcode, Register registerA3, Register registerBp, Register registerC, Register registerD)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA3;
  decoded[1] = (OperandDecoded)registerBp;
  decoded[2] = (OperandDecoded)registerC;
  decoded[3] = (OperandDecoded)registerD;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAl_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAl, Register registerBq);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAl_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAl, Register registerBq)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)roundint;
  decoded[1] = (OperandDecoded)saturate;
  decoded[2] = (OperandDecoded)registerAl;
  decoded[3] = (OperandDecoded)registerBq;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAh_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAh, Register registerBq);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAh_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAh, Register registerBq)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)roundint;
  decoded[1] = (OperandDecoded)saturate;
  decoded[2] = (OperandDecoded)registerAh;
  decoded[3] = (OperandDecoded)registerBq;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAx_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAx, Register registerBq);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAx_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAx, Register registerBq)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)roundint;
  decoded[1] = (OperandDecoded)saturate;
  decoded[2] = (OperandDecoded)registerAx;
  decoded[3] = (OperandDecoded)registerBq;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAy_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAy, Register registerBq);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAy_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAy, Register registerBq)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)roundint;
  decoded[1] = (OperandDecoded)saturate;
  decoded[2] = (OperandDecoded)registerAy;
  decoded[3] = (OperandDecoded)registerBq;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAz_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAz, Register registerBq);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAz_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAz, Register registerBq)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)roundint;
  decoded[1] = (OperandDecoded)saturate;
  decoded[2] = (OperandDecoded)registerAz;
  decoded[3] = (OperandDecoded)registerBq;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAt_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAt, Register registerBq);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_roundint_saturate_registerAt_registerBq(NativeBlock this, Opcode opcode, ModifierMember roundint, ModifierMember saturate, Register registerAt, Register registerBq)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)roundint;
  decoded[1] = (OperandDecoded)saturate;
  decoded[2] = (OperandDecoded)registerAt;
  decoded[3] = (OperandDecoded)registerBq;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_xrounding_silent2_rectify_registerA_registerB(NativeBlock this, Opcode opcode, ModifierMember xrounding, ModifierMember silent2, ModifierMember rectify, Register registerA, Register registerB);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_xrounding_silent2_rectify_registerA_registerB(NativeBlock this, Opcode opcode, ModifierMember xrounding, ModifierMember silent2, ModifierMember rectify, Register registerA, Register registerB)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)xrounding;
  decoded[1] = (OperandDecoded)silent2;
  decoded[2] = (OperandDecoded)rectify;
  decoded[3] = (OperandDecoded)registerA;
  decoded[4] = (OperandDecoded)registerB;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_xrounding_silent2_registerA_registerBp(NativeBlock this, Opcode opcode, ModifierMember xrounding, ModifierMember silent2, Register registerA, Register registerBp);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_xrounding_silent2_registerA_registerBp(NativeBlock this, Opcode opcode, ModifierMember xrounding, ModifierMember silent2, Register registerA, Register registerBp)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)xrounding;
  decoded[1] = (OperandDecoded)silent2;
  decoded[2] = (OperandDecoded)registerA;
  decoded[3] = (OperandDecoded)registerBp;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBe(NativeBlock this, Opcode opcode, Register registerA, Register registerBe);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBe(NativeBlock this, Opcode opcode, Register registerA, Register registerBe)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA;
  decoded[1] = (OperandDecoded)registerBe;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBo(NativeBlock this, Opcode opcode, Register registerA, Register registerBo);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerA_registerBo(NativeBlock this, Opcode opcode, Register registerA, Register registerBo)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerA;
  decoded[1] = (OperandDecoded)registerBo;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBe(NativeBlock this, Opcode opcode, Register registerN, Register registerBe);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBe(NativeBlock this, Opcode opcode, Register registerN, Register registerBe)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerN;
  decoded[1] = (OperandDecoded)registerBe;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBo(NativeBlock this, Opcode opcode, Register registerN, Register registerBo);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerN_registerBo(NativeBlock this, Opcode opcode, Register registerN, Register registerBo)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerN;
  decoded[1] = (OperandDecoded)registerBo;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_brknumber(NativeBlock this, Opcode opcode, ImmediateValue brknumber);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_brknumber(NativeBlock this, Opcode opcode, ImmediateValue brknumber)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)brknumber;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_v2_systemT2_registerZ(NativeBlock this, Opcode opcode, Register systemT2, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_v2_systemT2_registerZ(NativeBlock this, Opcode opcode, Register systemT2, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)systemT2;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_v2_systemAlone_registerZ(NativeBlock this, Opcode opcode, Register systemAlone, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_v2_systemAlone_registerZ(NativeBlock this, Opcode opcode, Register systemAlone, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)systemAlone;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_v2_systemT4_registerZ(NativeBlock this, Opcode opcode, Register systemT4, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_v2_systemT4_registerZ(NativeBlock this, Opcode opcode, Register systemT4, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)systemT4;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_v2_systemT3_registerZ(NativeBlock this, Opcode opcode, Register systemT3, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_v2_systemT3_registerZ(NativeBlock this, Opcode opcode, Register systemT3, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)systemT3;
  decoded[1] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_boolcas_signed10_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ImmediateValue signed10, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_boolcas_signed10_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ImmediateValue signed10, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)boolcas;
  decoded[1] = (OperandDecoded)signed10;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_boolcas_upper27_lower10_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ImmediateValue upper27_lower10, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_boolcas_upper27_lower10_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ImmediateValue upper27_lower10, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)boolcas;
  decoded[1] = (OperandDecoded)upper27_lower10;
  decoded[2] = (OperandDecoded)registerZ;
  decoded[3] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_boolcas_doscale_registerY_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ModifierMember doscale, Register registerY, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_boolcas_doscale_registerY_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ModifierMember doscale, Register registerY, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)boolcas;
  decoded[1] = (OperandDecoded)doscale;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_boolcas_lsucond_registerY_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ModifierMember lsucond, Register registerY, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_boolcas_lsucond_registerY_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ModifierMember lsucond, Register registerY, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)boolcas;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerZ;
  decoded[4] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_boolcas_lsucond_registerY_offset27_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_boolcas_lsucond_registerY_offset27_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)boolcas;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)offset27;
  decoded[4] = (OperandDecoded)registerZ;
  decoded[5] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_boolcas_lsucond_registerY_extend27_offset27_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerU);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_boolcas_lsucond_registerY_extend27_offset27_registerZ_registerU(NativeBlock this, Opcode opcode, ModifierMember boolcas, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ, Register registerU)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)boolcas;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)extend27_offset27;
  decoded[4] = (OperandDecoded)registerZ;
  decoded[5] = (OperandDecoded)registerU;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_cachelev_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ImmediateValue signed10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_cachelev_signed10_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ImmediateValue signed10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)cachelev;
  decoded[1] = (OperandDecoded)signed10;
  decoded[2] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_cachelev_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ImmediateValue upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_cachelev_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ImmediateValue upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)cachelev;
  decoded[1] = (OperandDecoded)upper27_lower10;
  decoded[2] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_cachelev_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ImmediateValue extend27_upper27_lower10, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_cachelev_extend27_upper27_lower10_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ImmediateValue extend27_upper27_lower10, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)cachelev;
  decoded[1] = (OperandDecoded)extend27_upper27_lower10;
  decoded[2] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_cachelev_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_cachelev_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)cachelev;
  decoded[1] = (OperandDecoded)registerY;
  decoded[2] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_cachelev_lsucond_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ModifierMember lsucond, Register registerY, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_cachelev_lsucond_registerY_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ModifierMember lsucond, Register registerY, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)cachelev;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_cachelev_lsucond_registerY_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_cachelev_lsucond_registerY_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ModifierMember lsucond, Register registerY, ImmediateValue offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)cachelev;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)offset27;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_cachelev_lsucond_registerY_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_cachelev_lsucond_registerY_extend27_offset27_registerZ(NativeBlock this, Opcode opcode, ModifierMember cachelev, ModifierMember lsucond, Register registerY, ImmediateValue extend27_offset27, Register registerZ)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)cachelev;
  decoded[1] = (OperandDecoded)lsucond;
  decoded[2] = (OperandDecoded)registerY;
  decoded[3] = (OperandDecoded)extend27_offset27;
  decoded[4] = (OperandDecoded)registerZ;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_conjugate_rounding_silent_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember conjugate, ModifierMember rounding, ModifierMember silent, Register registerW, Register registerZ, Register registerY);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_conjugate_rounding_silent_registerW_registerZ_registerY(NativeBlock this, Opcode opcode, ModifierMember conjugate, ModifierMember rounding, ModifierMember silent, Register registerW, Register registerZ, Register registerY)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)conjugate;
  decoded[1] = (OperandDecoded)rounding;
  decoded[2] = (OperandDecoded)silent;
  decoded[3] = (OperandDecoded)registerW;
  decoded[4] = (OperandDecoded)registerZ;
  decoded[5] = (OperandDecoded)registerY;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_conjugate_rounding_silent_registerM_registerP_registerO(NativeBlock this, Opcode opcode, ModifierMember conjugate, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerP, Register registerO);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_conjugate_rounding_silent_registerM_registerP_registerO(NativeBlock this, Opcode opcode, ModifierMember conjugate, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerP, Register registerO)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)conjugate;
  decoded[1] = (OperandDecoded)rounding;
  decoded[2] = (OperandDecoded)silent;
  decoded[3] = (OperandDecoded)registerM;
  decoded[4] = (OperandDecoded)registerP;
  decoded[5] = (OperandDecoded)registerO;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerW_registerP_registerO(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerW, Register registerP, Register registerO);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerW_registerP_registerO(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerW, Register registerP, Register registerO)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)rounding;
  decoded[1] = (OperandDecoded)silent;
  decoded[2] = (OperandDecoded)registerW;
  decoded[3] = (OperandDecoded)registerP;
  decoded[4] = (OperandDecoded)registerO;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerM_registerQ_registerR(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerQ, Register registerR);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_rounding_silent_registerM_registerQ_registerR(NativeBlock this, Opcode opcode, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerQ, Register registerR)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)rounding;
  decoded[1] = (OperandDecoded)silent;
  decoded[2] = (OperandDecoded)registerM;
  decoded[3] = (OperandDecoded)registerQ;
  decoded[4] = (OperandDecoded)registerR;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_multransm_multransl_rounding_silent_registerM_registerP_registerO(NativeBlock this, Opcode opcode, ModifierMember multransm_multransl, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerP, Register registerO);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_multransm_multransl_rounding_silent_registerM_registerP_registerO(NativeBlock this, Opcode opcode, ModifierMember multransm_multransl, ModifierMember rounding, ModifierMember silent, Register registerM, Register registerP, Register registerO)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)multransm_multransl;
  decoded[1] = (OperandDecoded)rounding;
  decoded[2] = (OperandDecoded)silent;
  decoded[3] = (OperandDecoded)registerM;
  decoded[4] = (OperandDecoded)registerP;
  decoded[5] = (OperandDecoded)registerO;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_channel_registerA(NativeBlock this, Opcode opcode, ModifierMember channel, Register registerA);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_channel_registerA(NativeBlock this, Opcode opcode, ModifierMember channel, Register registerA)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)channel;
  decoded[1] = (OperandDecoded)registerA;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerAp_registerBp_registerDp(NativeBlock this, Opcode opcode, Register registerAp, Register registerBp, Register registerDp);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerAp_registerBp_registerDp(NativeBlock this, Opcode opcode, Register registerAp, Register registerBp, Register registerDp)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerAp;
  decoded[1] = (OperandDecoded)registerBp;
  decoded[2] = (OperandDecoded)registerDp;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_registerAq_registerB_registerD(NativeBlock this, Opcode opcode, Register registerAq, Register registerB, Register registerD);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_registerAq_registerB_registerD(NativeBlock this, Opcode opcode, Register registerAq, Register registerB, Register registerD)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)registerAq;
  decoded[1] = (OperandDecoded)registerB;
  decoded[2] = (OperandDecoded)registerD;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_xrounding_silent2_registerAe_registerBp_registerDp(NativeBlock this, Opcode opcode, ModifierMember xrounding, ModifierMember silent2, Register registerAe, Register registerBp, Register registerDp);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_xrounding_silent2_registerAe_registerBp_registerDp(NativeBlock this, Opcode opcode, ModifierMember xrounding, ModifierMember silent2, Register registerAe, Register registerBp, Register registerDp)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)xrounding;
  decoded[1] = (OperandDecoded)silent2;
  decoded[2] = (OperandDecoded)registerAe;
  decoded[3] = (OperandDecoded)registerBp;
  decoded[4] = (OperandDecoded)registerDp;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

#ifdef $XCC__h
extern Instruction
NativeBlock_makeInstruction_kv3_xrounding_silent2_registerAo_registerBp_registerDp(NativeBlock this, Opcode opcode, ModifierMember xrounding, ModifierMember silent2, Register registerAo, Register registerBp, Register registerDp);
#endif//$XCC__h

Instruction
NativeBlock_makeInstruction_kv3_xrounding_silent2_registerAo_registerBp_registerDp(NativeBlock this, Opcode opcode, ModifierMember xrounding, ModifierMember silent2, Register registerAo, Register registerBp, Register registerDp)
{
  Memory memory = NativeBlock_MEMORY(this);
  Instruction instruction = Memory_alloc(memory, sizeof(Instruction_));
  OperandDecoded *decoded = OperandsBuffer_DECODED(Instruction_BUFFER(instruction));
  decoded[0] = (OperandDecoded)xrounding;
  decoded[1] = (OperandDecoded)silent2;
  decoded[2] = (OperandDecoded)registerAo;
  decoded[3] = (OperandDecoded)registerBp;
  decoded[4] = (OperandDecoded)registerDp;
  Instruction_Ctor(instruction, opcode, NULL);
  InstructionStack_push2(NativeBlock_INSTRUCTIONS(this), instruction);
  return instruction;
}

