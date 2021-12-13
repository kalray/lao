#ifdef $XCC_h
/*
 * !!!!	RegionFlow.xcc
 *
 * Benoit Dupont de Dinechin (bd3@kalray.eu).
 * Duco van Amstel (duco.van-amstel@kalray.eu)
 *
 * Copyright 2010 - 2013 Kalray S.A.
 * Copyright 2002 - 2007 STMicroelectronics.
 * Copyright 1995 - 1998 Commissariat a l'Energie Atomique.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of either (at your option): the GNU
 * General Public License (GPL) version 2; the GNU Lesser General
 * Public License (LGPL) version 2.1; any later version of these
 * licences as published by the Free Software Foundation.
 */
#endif//$XCC_h

#ifdef $XCC_h
/**
 * @file
 * @ingroup XFA
 * @brief Region Flow (to be replaced).
 */
#endif//$XCC_h


#include "XFA/XFA_.h"

#ifdef $XCC__h
#include "XFA/RegionFlow.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Control-flow representation and optimization for a CodeRegion.
 */
struct RegionFlow_ {
  //@args	Memory memory, CodeRegion codeRegion
  CodeRegion CODEREGION;
  //@access CONFIGURE	CodeRegion_configure(RegionFlow_CODEREGION(this))
  Procedure PROCEDURE;
  CFGraph_ CFGRAPH_;			// The flow-graph of this RegionFlow.
  //@access CFGRAPH	RegionFlow__CFGRAPH_(this)
  //@access MEMORY	CFGraph_memory(RegionFlow_CFGRAPH(this))
  //@access NODECOUNT	CFGraph_NODECOUNT(RegionFlow_CFGRAPH(this))
  //@access ROOTNODE	CFGraph_ROOTNODE(RegionFlow_CFGRAPH(this))
  //@access SINKNODE	CFGraph_SINKNODE(RegionFlow_CFGRAPH(this))
  Hierarchy HIERARCHY;			// NSCC Hierarchy
};
#endif//$XCC_h

RegionFlow
RegionFlow_Ctor(RegionFlow this,
                Memory memory, CodeRegion codeRegion)
{
  int32_t blockCount = CodeRegion_blockCount(codeRegion);
  CFGraph cfgraph = CFGraph_Ctor(RegionFlow_CFGRAPH(this), memory, blockCount+2);
  CFGNode rootNode = CFGraph_rootNode(cfgraph);
  *CFGNode__BASICBLOCK(rootNode) = CodeRegion_rootBlock(codeRegion);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    CFGNode node = CFGraph_makeNode(cfgraph, basicBlock);
    DGraphNodeCell node_cell = DGraphNode_CELL(node);
    *DGraphNodeCell__INDEX(node_cell) = BasicBlock_index(basicBlock);	// FIXME!
    Except_CHECK(CFGNode_INDEX(node) == BasicBlock_index(basicBlock));
  } CodeRegion_ENDEACH_BasicBlock;
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_LEAVE_FORBACK_BasicBlockEdge(basicBlock, successor) {
      CFGArcFlags flags = 0;
      BasicBlock tailBlock = basicBlock;
      BasicBlock headBlock = BasicBlockEdge_destBlock(successor);
      float probability = BasicBlockEdge_probability(successor);
      CFGNode tailNode = BasicBlock_cfgNode(tailBlock);
      CFGNode headNode = BasicBlock_cfgNode(headBlock);
      if (BasicBlock_index(headBlock) != 0) {
        Operation control_operation = BasicBlock_getControlOperation(tailBlock);
        if (control_operation != NULL) {
          if (Operation_isConditional(control_operation) &&
              Operation_isGoTo(control_operation)) {
            Label label = Operation_getTargetLabel(control_operation);
            if (label != NULL && BasicBlock_hasLabel(headBlock, label));
            else flags |= CFGArcFlag_Fall;
          }
          if (Operation_isRoutine(control_operation)) {
            flags |= CFGArcFlag_Fall;
          }
        } else flags |= CFGArcFlag_Fall;
        CFGraph_makeArc(cfgraph, tailNode, headNode, probability, flags);
        BasicBlock_killEdge(basicBlock, successor);
      }
    } BasicBlock_LEAVE_ENDBACK_BasicBlockEdge;
  } CodeRegion_ENDEACH_BasicBlock;
  CFGraph_complete(cfgraph);
  CFGraph_sortLeaveArcs(cfgraph);
  CodeRegion_setRegionFlow(codeRegion, this);
  *RegionFlow__CODEREGION(this) = codeRegion;
  *RegionFlow__PROCEDURE(this) = CodeRegion_procedure(codeRegion);
  *RegionFlow__HIERARCHY(this) = NULL;
  return this;
}

void
RegionFlow_Dtor(RegionFlow this)
{
  CFGraph_Dtor(RegionFlow_CFGRAPH(this));
  Hierarchy_delete(RegionFlow_HIERARCHY(this));
  *RegionFlow__HIERARCHY(this) = NULL;
}

size_t
RegionFlow_Size(Memory memory, CodeRegion codeRegion)
{
  return sizeof(RegionFlow_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(RegionFlow_)\t%zu\n", sizeof(RegionFlow_));
}
#endif//$XCC__c

#ifdef $XCC_h
RegionFlow
RegionFlow_new(Memory parent, CodeRegion codeRegion);
#endif//$XCC_h

RegionFlow
RegionFlow_new(Memory parent, CodeRegion codeRegion)
{
  Memory memory = Memory_new(parent, true);
  size_t size = RegionFlow_Size(memory, codeRegion);
  RegionFlow this = Memory_alloc(memory, size);
  return RegionFlow_Ctor(this, memory, codeRegion);
}

#ifdef $XCC_h
RegionFlow
RegionFlow_delete(RegionFlow this);
#endif//$XCC_h

RegionFlow
RegionFlow_delete(RegionFlow this)
{
  if (this != NULL) {
    Memory memory = RegionFlow_MEMORY(this);
#ifndef _NDTOR
    RegionFlow_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * This RegionFlow Memory.
 */
static inline Memory
RegionFlow_memory(RegionFlow this)
{
  return RegionFlow_MEMORY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This RegionFlow CodeRegion.
 */
static inline CodeRegion
RegionFlow_codeRegion(RegionFlow this)
{
  return RegionFlow_CODEREGION(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This RegionFlow Configure.
 */
static inline Configure
RegionFlow_configure(RegionFlow this)
{
  return RegionFlow_CONFIGURE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This RegionFlow Procedure.
 */
static inline Procedure
RegionFlow_procedure(RegionFlow this)
{
  return RegionFlow_PROCEDURE(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Iterate this RegionFlow BasicBlocks.
 */
#define RegionFlow_FOREACH_BasicBlock(this, basicBlock) \
  CodeRegion_FOREACH_BasicBlock(RegionFlow_codeRegion(this), basicBlock)
#define RegionFlow_ENDEACH_BasicBlock \
  CodeRegion_ENDEACH_BasicBlock;
#endif//$XCC_h

#ifdef $XCC_h
/**
 * This RegionFlow CFGraph.
 */
static inline CFGraph
RegionFlow_cfgraph(RegionFlow this)
{
  return RegionFlow_CFGRAPH(this);
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * The NSCC Hierarchy of this RegionFlow.
 */
static inline Hierarchy
RegionFlow_hierarchy(RegionFlow this)
{
  return RegionFlow_HIERARCHY(this);
}
#endif//$XCC_h

#ifdef $XCC_h
/**
 * Make the NSCC Hierarchy of this RegionFlow.
 *
 * As main side-effects, create the missing LoopScope(s) and sets the CFGNode flags.
 * Also sets the LOOPSCOPE fields of BasicBlock(s).
 */
Hierarchy
RegionFlow_makeHierarchy(RegionFlow this);
#endif//$XCC_h

static void
RegionFlow_processHierarchyComponent(RegionFlow this, HierarchyComponent component,
                                      LoopScope parent_scope, Configure configure)
{
  int blockCount = 0;
  LoopScope loopScope = NULL;
  BasicBlock headerBlock = NULL;
  Procedure procedure = RegionFlow_PROCEDURE(this);
  CFGNode cfgNode = HierarchyComponent_node(component);
  HierarchyComponentType type = HierarchyComponent_type(component);
  bool isSimpleLoop = (type == HierarchyComponentType_Loop);
  bool isInnerLoop = (cfgNode != NULL);
  // Get component header block.
  if (cfgNode != NULL && type != HierarchyComponentType_Node) {
    *CFGNode__FLAGS(cfgNode) |= CFGNodeFlag_Widen;
    headerBlock = CFGNode_basicBlock(cfgNode);
  }
  // Lookup for corresponding LoopScope.
  loopScope = Procedure_makeLoopScope(procedure, headerBlock, configure);
  // Traversal of component: prepare BasicBlock and set CFGNode flags.
  HierarchyComponent_CHILDREN_FOREACH_HierarchyComponent(component, child) {
    if (HierarchyComponent_type(child) == HierarchyComponentType_Node) {
      CFGNode cfgNode = HierarchyComponent_node(child);
      BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
      CFGNodeFlags nodeFlags = CFGNodeFlag_Grow;
      if (basicBlock != NULL) {
        Operation control_operation = BasicBlock_getControlOperation(basicBlock);
        BasicBlock_setLoopScope(basicBlock, loopScope);
        ++blockCount;
        if (BasicBlock_isStart(basicBlock)) nodeFlags |= CFGNodeFlag_Start;
        if (BasicBlock_isEntry(basicBlock)) nodeFlags |= CFGNodeFlag_Start;
        if (BasicBlock_hasGNUASM(basicBlock)) nodeFlags |= CFGNodeFlag_Stop;
        else if (control_operation == NULL || Operation_isGoTo(control_operation));
        else nodeFlags |= CFGNodeFlag_Stop;
      } else nodeFlags = CFGNodeFlag_Stop;
      *CFGNode__FLAGS(cfgNode) = nodeFlags;
    } else {
      isInnerLoop = false;
    }
  } HierarchyComponent_CHILDREN_ENDEACH_HierarchyComponent;
  // Update the component LoopScope.
  if (headerBlock != NULL) {
    LoopScope_update(loopScope, parent_scope, blockCount, isSimpleLoop, isInnerLoop);
  }
  if (isInnerLoop) {
    *CFGNode__FLAGS(cfgNode) |= CFGNodeFlag_Inner;
  } else {
    // Non-inner loop: recursive processing.
    HierarchyComponent_CHILDREN_FOREACH_HierarchyComponent(component, child) {
      if (HierarchyComponent_type(child) != HierarchyComponentType_Node) {
        RegionFlow_processHierarchyComponent(this, child, loopScope, configure);
      }
    } HierarchyComponent_CHILDREN_ENDEACH_HierarchyComponent;
  }
}

Hierarchy
RegionFlow_makeHierarchy(RegionFlow this)
{
  Hierarchy hierarchy = RegionFlow_HIERARCHY(this);
  if (hierarchy == NULL) {
    Memory memory = RegionFlow_MEMORY(this);
    Procedure procedure = RegionFlow_PROCEDURE(this);
    CodeRegion codeRegion = RegionFlow_CODEREGION(this);
    Configure configure = CodeRegion_configure(codeRegion);
    LoopScope rootInfo = Procedure_makeLoopScope(procedure, NULL, configure);
    // Make the NSSC Hierarchy.
    DGraph dgraph = CFGraph_DGRAPH(RegionFlow_CFGRAPH(this));
    DGraphNode entry = (DGraphNode)RegionFlow_ROOTNODE(this);
    // Prevent ROOTNODE to be included in the CFG.
    BasicBlock headerBlock = CFGNode_basicBlock((CFGNode)entry);
    if (BasicBlock_index(headerBlock) == 0) {
      headerBlock = BasicBlockEdge_destBlock(BasicBlock_leaveFirst(headerBlock)); // Is real header
      entry = (DGraphNode) BasicBlock_cfgNode(headerBlock);
    }
    hierarchy = Hierarchy_new(memory, dgraph, entry);
    Hierarchy_makeNSCC(hierarchy);
    {
      // Copy user-supplied LoopScope into rootInfo (ugly Open64 hack).
      HierarchyComponent root_component = Hierarchy_root(hierarchy);
      HierarchyComponent_CHILDREN_FOREACH_HierarchyComponent(root_component, child) {
        if (HierarchyComponent_type(child) == HierarchyComponentType_Node) {
          CFGNode cfgNode = HierarchyComponent_node(child);
          BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
          if (basicBlock != NULL) {
            LoopScope loopScope = Procedure_findLoopScope(procedure, basicBlock);
            if (loopScope != NULL) {
              LoopScope_transfer(rootInfo, loopScope);
              //Dangling pointer //Procedure_removeLoopScope(procedure, basicBlock);
            }
          }
        }
      } HierarchyComponent_CHILDREN_ENDEACH_HierarchyComponent;
      // Recursive HierarchyComponent processing.
      RegionFlow_processHierarchyComponent(this, root_component, rootInfo, configure);
    }
    *RegionFlow__HIERARCHY(this) = hierarchy;
  }
  return hierarchy;
}

#ifdef $XCC_h
/**
 * Kill the Hierarchy of this RegionFlow.
 *
 * The Hierarchy object is deleted but the CFGNode(s) flags remain.
 */
static inline void
RegionFlow_killHierarchy(RegionFlow this)
{
  *RegionFlow__HIERARCHY(this) = Hierarchy_delete(RegionFlow_HIERARCHY(this));
}
#endif//$XCC_h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Reorder the BasicBlocks of this RegionFlow
 * and reroute their branches accordingly.
 *
 * Reordering of this RegionFlow BasicBlocks relies on the BasicBlock_traceId
 * fields. When two fields have the same value the two BasicBlocks are assumed to
 * have been in the same BlockTrace and no control-flow fixups are made.
 * Otherwise the control-flow is checked for fall-through arcs and GOTOs are
 * inverted or added if necessary.
 */
void
RegionFlow_reorderBasicBlocks(RegionFlow this);
#endif//$XCC_h

void
RegionFlow_reorderBasicBlocks(RegionFlow this)
{
  intptr_t prev_traceId = -1;
  BasicBlock prev_block = NULL;
  RegionFlow_FOREACH_BasicBlock(this, curr_block) {
    intptr_t curr_traceId = BasicBlock_traceId(curr_block);
    if (prev_block != NULL && !BasicBlock_getJumpOperation(prev_block)) {
      if (curr_traceId != prev_traceId || curr_traceId == 0) {
        // Detected a boundary between two traces or ouside a trace.
        RegionFlow_fixBasicBlockGoTo(this, prev_block, curr_block);
      }
    }
    prev_traceId = curr_traceId;
    prev_block = curr_block;
  } RegionFlow_ENDEACH_BasicBlock;
  // Need to fix the last block here 
  if (prev_block != NULL && !BasicBlock_getJumpOperation(prev_block)) {
    RegionFlow_fixBasicBlockGoTo(this, prev_block, NULL);
  }
}

#ifdef $XCC__h
/**
 * Reroute a CFGArc through an unconditional GOTO operation.
 */
bool
RegionFlow_rerouteCFGArc(RegionFlow this, CFGArc arc);
#endif//$XCC__h

bool
RegionFlow_rerouteCFGArc(RegionFlow this, CFGArc arc)
{
  CFGraph cfgraph = RegionFlow_CFGRAPH(this);
  Procedure procedure = RegionFlow_PROCEDURE(this);
  CodeRegion codeRegion = RegionFlow_CODEREGION(this);
  CFGNode tail_node = CFGArc_TAIL(arc);
  BasicBlock tail_block = CFGNode_basicBlock(tail_node);
  CFGNode head_node = CFGArc_HEAD(arc);
  BasicBlock head_block = CFGNode_basicBlock(head_node);
  Label head_label = BasicBlock_makeLabel(head_block);
  if (BasicBlock_getControlOperation(tail_block) == NULL) {
    Selector tail_selector = BasicBlock_selector(tail_block);
    Selector_selectGOTO(tail_selector, head_label);
    *CFGArc__FLAGS(arc) &= ~CFGArcFlag_Fall;
    return false;
  } else {
    CFGArcFlags arc_flags = CFGArc_FLAGS(arc);
    float arc_probability = CFGArc_PROBABILITY(arc);
    float tail_frequency = BasicBlock_frequency(tail_block);
    BasicBlock fall_block =
        CodeRegion_makeBasicBlock(codeRegion, tail_frequency*arc_probability);
    Selector fall_selector = BasicBlock_selector(fall_block);
    CFGNode fall_node = CFGraph_makeNode(cfgraph, fall_block);
    CFGArc fall_arc = CFGraph_makeArc(cfgraph, tail_node, fall_node,
                                      arc_probability, arc_flags);
    CFGArc_detachTailNode(arc);
    CFGArc_attachTailNode(arc, fall_node);
    *CFGArc__PROBABILITY(arc) = 1.0;
    *CFGArc__FLAGS(arc) &= ~CFGArcFlag_Fall;
    Procedure_chainBasicBlocks(procedure, tail_block, fall_block);
    BasicBlock_setTraceId(fall_block, BasicBlock_traceId(tail_block));
    Selector_selectGOTO(fall_selector, head_label);
    return true;
  }
}

#ifdef $XCC__h
/**
 * Fix the goto operation betwen the two BasicBlocks.
 */
void
RegionFlow_fixBasicBlockGoTo(RegionFlow this,
                              BasicBlock prev_block, BasicBlock curr_block);
#endif//$XCC__h

void
RegionFlow_fixBasicBlockGoTo(RegionFlow this,
                              BasicBlock prev_block, BasicBlock curr_block)
{
  Procedure procedure = RegionFlow_PROCEDURE(this);
  CFGNode prev_node = BasicBlock_cfgNode(prev_block);
  BasicBlock head_block1 = NULL, head_block2 = NULL;
  CFGArc arc1 = NULL, arc2 = NULL;
  int leaveCount = 0;
  CFGNode_FORLEAVE_CFGArc(prev_node, arc) {
    ++leaveCount;
    if (leaveCount == 1) {
      head_block1 = CFGNode_basicBlock(CFGArc_HEAD(arc));
      arc1 = arc;
    } else if (leaveCount == 2) {
      head_block2 = CFGNode_basicBlock(CFGArc_HEAD(arc));
      arc2 = arc;
    } else if (leaveCount > 2) break;
  } CFGNode_ENDLEAVE_CFGArc;
  if (leaveCount == 1 && head_block1 != NULL) {
    // One successor for prev_block.
    if (head_block1 != curr_block) {
      // Must unconditionally GOTO head_block1.
      Operation goto_operation = CFGNode_getGoToOperation(prev_node);
      if (goto_operation != NULL) {
        Label head_label1 = BasicBlock_makeLabel(head_block1);
        Procedure_updateTargetedOperation(procedure, goto_operation, head_label1);
      } else {
        RegionFlow_rerouteCFGArc(this, arc1);
      }
    } else if (curr_block != NULL) {
      if (BasicBlock_regionId(prev_block) == BasicBlock_regionId(curr_block)) {
        // Discard useless GOTO to sequential successor.
        CFGNode_discardGoToOperation(prev_node);
      }
    }
  } else if (leaveCount == 2 && head_block1 != NULL && head_block2 != NULL) {
    // Two successors for prev_block.
    Operation goto_operation = CFGNode_getGoToOperation(prev_node);
    Label goto_label = Operation_getTargetLabel(goto_operation);
    Except_CHECK(head_block1 != head_block2);
    if (head_block1 != curr_block && head_block2 != curr_block) {
      // Must unconditionally GOTO head_block1 or head_block2.
      if (BasicBlock_hasLabel(head_block1, goto_label)) {
        // Conditional GOTO head_block1, must reroute arc2.
        RegionFlow_rerouteCFGArc(this, arc2);
      } else if (BasicBlock_hasLabel(head_block2, goto_label)) {
        // Conditional GOTO head_block2, must reroute arc1.
        RegionFlow_rerouteCFGArc(this, arc1);
      } else Except_NEVER(true);
    } else if (head_block1 == curr_block) {
      // Ensure that the GOTO is to head_block2.
      if (!BasicBlock_hasLabel(head_block2, goto_label)) {
        CFGNode_invertGoToOperation(prev_node);
      }
    } else if (head_block2 == curr_block) {
      // Ensure that the GOTO is to head_block1.
      if (!BasicBlock_hasLabel(head_block1, goto_label)) {
        CFGNode_invertGoToOperation(prev_node);
      }
    } else Except_NEVER(true);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
/**
 * Finalize this RegionFlow before destruction.
 *
 * Transfer back to the BasicBlockEdge(s) the CFGArc information.
 */
void
RegionFlow_finalize(RegionFlow this);
#endif//$XCC_h

void
RegionFlow_finalize(RegionFlow this)
{
  CodeRegion codeRegion = RegionFlow_CODEREGION(this);
  RegionFlow_FOREACH_BasicBlock(this, basicBlock) {
    CFGNode cfgNode = BasicBlock_cfgNode(basicBlock);
    CFGNode_FORLEAVE_CFGArc(cfgNode, cfgArc) {
      CFGNode head_node = CFGArc_HEAD(cfgArc);
      BasicBlock head_block = CFGNode_basicBlock(head_node);
      if (head_block != NULL) {
        float probability = CFGArc_probability(cfgArc);
        BasicBlock_makeEdge(basicBlock, head_block, probability);
      }
    } CFGNode_ENDLEAVE_CFGArc;
  } RegionFlow_ENDEACH_BasicBlock;
  CodeRegion_setRegionFlow(codeRegion, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _NTRACE
static bool
RegionFlow_prettyHierarchyNode(DGraphNode node, FILE *file)
{
  CFGNode cfgNode = (CFGNode)node;
  BasicBlock basicBlock = CFGNode_basicBlock(cfgNode);
  if (basicBlock != NULL) {
    fprintf(file, "%d:Block", BasicBlock_identity(basicBlock));
  }
  fprintf(file, "_%d", DGraphNode_INDEX(node));
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Pretty-print this RegionFlow.
 */
bool
RegionFlow_pretty(RegionFlow this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
RegionFlow_pretty(RegionFlow this, FILE *file)
{
  float contributed = 0.0;
  CFGraph cfgraph = RegionFlow_CFGRAPH(this);
  CFGNode rootNode = CFGraph_rootNode(cfgraph);
  CFGNode sinkNode = CFGraph_sinkNode(cfgraph);
  Procedure procedure = RegionFlow_PROCEDURE(this);
  Hierarchy hierarchy = RegionFlow_HIERARCHY(this);
  LoopScope loopScope = Procedure_findLoopScope(procedure, NULL);
  const char *procedureName = Procedure_name(procedure);
  if (file == NULL) file = stderr;
  if (hierarchy != NULL) {
    fprintf(file, "------- Hierarchy -----\n");
    Hierarchy_pretty(hierarchy, file, RegionFlow_prettyHierarchyNode);
    fprintf(file, "\n");
  }
  fprintf(file, "------- BasicBlocks(%s) -----\n", procedureName);
  if (loopScope != NULL) LoopScope_pretty(loopScope, file);
  fprintf(file, "   0:Start");
  CFGNode_pretty(rootNode, file);
  fprintf(file, "\n");
  RegionFlow_FOREACH_BasicBlock(this, basicBlock) {
    CFGNode cfgNode = BasicBlock_cfgNode(basicBlock);
    fprintf(file, "%4d:Block", BasicBlock_identity(basicBlock));
    CFGNode_pretty(cfgNode, file);
    BasicBlock_pretty(basicBlock, file);
    contributed += BasicBlock_contributed(basicBlock);
  } RegionFlow_ENDEACH_BasicBlock;
  if (sinkNode != NULL) {
    fprintf(file, "  -1:Stop");
    CFGNode_pretty(sinkNode, file);
    fprintf(file, "\n");
  }
  fprintf(file, "------- contributed=%g -----\n", contributed);
  return true;
}
#endif//_NTRACE

#ifdef $XCC_h
/**
 * Pretty-print this RegionFlow in VCG format.
 */
bool
RegionFlow_prettyVCG(RegionFlow this, FILE *file);
#endif//$XCC_h

#ifndef _NTRACE
bool
RegionFlow_prettyVCG(RegionFlow this, FILE *file)
{
  CFGraph cfgraph = RegionFlow_CFGRAPH(this);
  Procedure procedure = RegionFlow_PROCEDURE(this);
  return CFGraph_prettyVCG(cfgraph, file, Procedure_name(procedure));
}
#endif//_NTRACE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the RegionFlow module.
 */
#define RegionFlow_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the RegionFlow module.
 */
#define RegionFlow_FINI()
#endif//$XCC__h

#if XCC__C
static void
RegionFlow_TEST(void)
{
#include "XFA/RegionFlow_.c"
}

int
main(int argc, char **argv)
{
  XFA_Test(RegionFlow_TEST);
  return 0;
}
#endif

