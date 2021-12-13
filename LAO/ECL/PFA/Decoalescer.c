#ifdef $XCC_h
/*
 * !!!!	Decoalescer.xcc
 *
 * Fabrice Rastello (Fabrice.Rastello@ens-lyon.fr).
 * Francois de Ferriere (Francois.de-Ferriere@st.com).
 * Benoit Dupont de Dinechin (Benoit.Dupont-de-Dinechin@st.com).
 *
 * Copyright 2009 - 2010 INRIA Rhone-Alpes
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
 * @ingroup PFA
 * @brief Decoalescer out-of-SSA algorithm.
 */
#endif//$XCC_h


#include "PFA/PFA_.h"

#ifdef $XCC__h
#include "PFA/Decoalescer.h"
#endif//$XCC__h

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC_h
struct Decoalescer_;
#endif//$XCC_h

#ifdef $XCC__h
struct Decoalescer_ {
  //@args       Memory memory, CodeRegion codeRegion, Dominance dominance
  Coalescer_ COALESCER[1];		// SSA Coalescer.
  //@access MEMORY		Coalescer_MEMORY(Decoalescer_COALESCER(this))
  //@access CODEREGION		Coalescer_CODEREGION(Decoalescer_COALESCER(this))
  //@access SSAFORM		CodeRegion_ssaForm(Decoalescer_CODEREGION(this))
  //@access DOMINANCE		Coalescer_DOMINANCE(Decoalescer_COALESCER(this))
};
#endif//$XCC__h

Decoalescer
Decoalescer_Ctor(Decoalescer this,
                 Memory memory, CodeRegion codeRegion, Dominance dominance)
{
  Optimize optimize = CodeRegion_optimize(codeRegion);
  OptimizeFlags coalescingFlags = Optimize_COALESCING(optimize);
  SSAForm ssaForm = CodeRegion_ssaForm(Decoalescer_CODEREGION(this));
  TemporaryTable temporaryTable = CodeRegion_globalTable(codeRegion);
  Coalescer coalescer = Decoalescer_COALESCER(this);
  Coalescer_Ctor(coalescer, memory, codeRegion, temporaryTable, dominance, coalescingFlags);
  if (Coalescer_CONSUMING(coalescer)) {
    CodeRegion_makeConsumers(codeRegion, temporaryTable);
  }
  Coalescer_complete(coalescer, true);
  PFA_TRACE && fprintf(PFA_TRACE, "*** Before Decoalescer\n")
            && CodeRegion_pretty(codeRegion, PFA_TRACE)
            && SSAForm_pretty(ssaForm, PFA_TRACE);
  // TBD: Decoalescer_Budimlic(this, codeRegion);
  // FdF 20071116: While Budimlic is not implemented, just create SSA webs.
  // FdF 20090218: Apply minimum filtering on strong interferences.
  SSAForm_weblinksInit(ssaForm, temporaryTable);
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      if (Operator_isPhi(operator) || Operator_isSeqCopy(operator) || Operator_isParCopy(operator)) {
#if 1
        Decoalescer_coalesceFiltering(this, operation, false);
#else
        Decoalescer_coalesceFiltering(this, operation, true);
#endif	
      }
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  CodeRegion_FOREACH_BasicBlock(codeRegion, basicBlock) {
    BasicBlock_FOREACH_Operation(basicBlock, operation) {
      Operator operator = Operation_operator(operation);
      if (Operator_isPhi(operator)) {
        Temporary *operands = Operation_operands(operation);
        *Operation__PHICOLOR(operation) = Variable_color(Temporary_VARIABLE(operands[0]));
      } else break;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
  // Use Decoalescing algorithm.
  Decoalescer_decoalesceWebs(this);
  Decoalescer_generateCopies(this);
  if (coalescingFlags & OptimizeCoalescing_Congruence) {
    Coalescer_coalesceCongruences(coalescer);
  } else {
    Coalescer_markCoalescedCopies(coalescer);
  }
  PFA_TRACE && (GETENV("SSA_TRACE"))
            && fprintf(PFA_TRACE, " *** After Decoalescer CSSA\n")
            && CodeRegion_pretty(codeRegion, PFA_TRACE);
  return this;
}

void
Decoalescer_Dtor(Decoalescer this)
{
  Memory memory = Decoalescer_MEMORY(this);
  Coalescer_Dtor(Decoalescer_COALESCER(this));
}

size_t
Decoalescer_Size(Memory memory, CodeRegion codeRegion, Dominance dominance)
{
  return sizeof(Decoalescer_);
}

#ifdef $XCC__c
{
  fprintf(stderr, "sizeof(Decoalescer_) = %zu\n", sizeof(Decoalescer_));
}
#endif//$XCC__c

#ifdef $XCC_h
Decoalescer
Decoalescer_new(Memory parent, CodeRegion codeRegion, Dominance dominance);
#endif//$XCC_h

Decoalescer
Decoalescer_new(Memory parent, CodeRegion codeRegion, Dominance dominance)
{
  Memory memory = Memory_new(parent, true);
  size_t size = Decoalescer_Size(memory, codeRegion, dominance);
  void *alloc = Memory_alloc(memory, size);
  return Decoalescer_Ctor(alloc, memory, codeRegion, dominance);
}

#ifdef $XCC_h
Decoalescer
Decoalescer_delete(Decoalescer this);
#endif//$XCC_h

Decoalescer
Decoalescer_delete(Decoalescer this)
{
  if (this != NULL) {
    Memory memory = Decoalescer_MEMORY(this);
#ifndef _NDTOR
    Decoalescer_Dtor(this);
    Memory_free_(memory, this);
#endif//_NDTOR
    Memory_delete(memory);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
static Variable
Variable_color(Variable V);
#endif//$XCC__h

static Variable
Variable_color(Variable V)
{
  if (Variable_isIsolated(V))
    return NULL;
  return Variable_findWebRoot(V);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/*
 * Decoalescer_coalesceFiltering --  Coalesce variables with no strong interferences, to be processed by dforest.
 */
void
Decoalescer_coalesceFiltering(Decoalescer this, Operation operation, bool weakInterference);
#endif//$XCC__h

void
Decoalescer_coalesceFiltering(Decoalescer this, Operation operation, bool weakInterference)
{
  Operator operator = Operation_operator(operation);
  Coalescer coalescer = Decoalescer_COALESCER(this);
  if (Operator_isPhi(operator)) {
    Temporary *operands = Operation_operands(operation);
    Variable variable_0 = Temporary_VARIABLE(operands[0]);
    Variable webroot_0 = Variable_findWebRoot(variable_0);
    int32_t operandCount = Operation_operandCount(operation), i;
    for (i = 1; i < operandCount; i++) {
      Variable variable_i = Temporary_VARIABLE(operands[i]);
      Variable webroot_i = Variable_findWebRoot(variable_i);
      if ((webroot_0 != webroot_i) &&
          // Check strong interferences
          ((Variable_PINNED(webroot_0) == Register__UNDEF) ||
           (Variable_PINNED(webroot_i) == Register__UNDEF)) &&
          // Check weak interferences
          (!weakInterference || !Coalescer_interferesVV(coalescer, variable_0, variable_i))) {
        PFA_TRACE && fprintf(PFA_TRACE, "Coalesce V%d with V%d\n", Temporary_IDENTITY(operands[0]), Temporary_IDENTITY(operands[i]));
        webroot_0 = Variable_linkWebRoots(webroot_0, webroot_i);
        Variable_clearIsolated(variable_i);
        Variable_clearIsolated(variable_0);
      }
    }
  }
  else if (Operator_isSeqCopy(operator) || Operator_isParCopy(operator)) {
    int32_t argCount = Operation_argCount(operation), index;
    for (index = 0; index < argCount; index++) {
      Temporary temporary_x = Operation_getResult(operation, index);
      Temporary temporary_y = Operation_getArgument(operation, index);
      if (   Temporary_isVariable(temporary_x)
             && Temporary_isVariable(temporary_y)) {
        Variable variable_x = Temporary_VARIABLE(temporary_x);
        Variable variable_y = Temporary_VARIABLE(temporary_y);
        Variable webroot_x = Variable_findWebRoot(variable_x);
        Variable webroot_y = Variable_findWebRoot(variable_y);
        if ((webroot_x != webroot_y) &&
            // Check strong interferences
            ((Variable_PINNED(webroot_x) == Register__UNDEF) ||
             (Variable_PINNED(webroot_y) == Register__UNDEF)) &&
            // Check weak interferences
            (!weakInterference || !Coalescer_interferesVV(coalescer, variable_x, variable_y))) {
          PFA_TRACE && fprintf(PFA_TRACE, "Coalesce V%d with V%d\n", Temporary_IDENTITY(temporary_x), Temporary_IDENTITY(temporary_y));
          Variable_linkWebRoots(webroot_x, webroot_y);
          Variable_clearIsolated(variable_x);
          Variable_clearIsolated(variable_y);
        }
      }
    }
  }
}

#ifdef $XCC__h
/**
 * Main processing for Decoalescing.
 * /!\ Work on congruence classes not on weblinks.
 */
void
Decoalescer_decoalesceWebs(Decoalescer this);
#endif//$XCC__h

/* Pseudo Code from F.Rastello.

Repair D-Forest:
================

Goal:
----
The aim of this phase is to traverse the dominance forest (notice that
the dominance forest does not have to be explicitely built) so as to
check that within each congruence class (represented by a color) there
remains no interference. If there remains any then one of the
interfering variable is removed from the congruence class (uncolored).

Notations:
---------
- type var_t: the type of a variable
- set VARS: set of variables
- type col_t: the type of a color. colorless (bottom) is represented by NULL
- set COLORS: set of congruence classes (colors)
- col_t col(VAR): to each variable V a color col(V)
- var_t cur_dom(COLORS): for each color C cur_dom(C) represents
  the variable of color C that dominates the current operation. Can be equal to NULL.
- var_r idom(VAR): for each variable V idom(V) represents the variable
  of color col(V) which definition immediatly dominates the definition
  of V (NULL if none exists).
- uint w(VAR): to each variable V a weight w(V) that relates the cost of
uncoloring it (the number of uses roughly gives the number of moves)

Color of a Variable:
-------------------
- Variables have been colored during the coalescing phase. The dforest
phase is not allowed to give another color to a variable other than NULL.
- We need to define properly what is the color of a Phi. The color of
a Phi is the color (different than NULL) of its operands.

colt_t col(op_t Phi)
  let Phi: a0=phi(a1,...,an)
  colPhi:=NULL
  for i:=1:n
    if col(ai)!=NULL
      colPhi:=col(ai)
      break
    endif
  endfor
  return colPhi
  

uncoloring variables that interfere:
------------------------------------
 dforest:
 { Initialisation }
 foreach c in COLORS cur_dom(c) := NULL
 
 foreach L in BBLOCS in DFS pre-order of the dominance tree
   { Traversal of all definitions in L }
   foreach Op in Operations(L) in topological order (including phis)
     foreach V in DefinedVariables(Op)
       if ( col(c:=col(V))==NULL ) then next { V }

       { update cur_dom if necessary }
       U := cur_dom(c)
       while ( (U != NULL) && (!dominates(U,V) || col(U)==NULL) )
         U := idom (U)
       endwhile
       cur_dom(c):=U

       { See if U or V should be removed; update cur_dom & idom if necessary }
       if U==NULL || !interfere(U,V)
         { No interference: none removed; update cur_dom & idom}
         idom(V):=U
         cur_dom(c):=V
       elseif (w(U) >= w(V))
         { Interference & V is preferable for removal: remove V }
         col(V):=NULL
       else
         { Interference & U is preferable for removal: remove U; update cur_dom & idom }
         col(U):=NULL
         cur_dom(c):=V
         idom(V):=idom(U)
       endif
     endforeach { V }
   endforeach { OP }

   { Initialize cur_phi }
   foreach c in COLORS cur_phi(c) := NULL

   { Traversal of all implicit definitions related to phi in successor blocs }
   foreach L' successor of L
     let j such that L is the j-th predecessor of L'

     foreach Phi: a0=phi(a1,...,an) in Operations(L')
       let aj be the operand coming from L
       colPhi:=col(Phi)
       if colPhi==NULL next

       { update cur_dom if necessary }
       U := cur_dom(colPhi)
       while ( (U != NULL) && (!dominates(bbloc(def(U)),L) || col(U)==NULL) )
         U := idom (U)
       endwhile
       cur_dom(colPhi):=U

       { Check if Phi interferes with its related cur_dom U }
       if U != NULL && U in liveOut(L) && U != aj
         { U interfere with Phi }
         if w(U)>=w(Phi)
           { Interference & Phi is preferable for removal }
           col(Phi):=NULL
           next
         else
           { Interference & U is preferable for removal }
           col(U):=NULL
         endif
       endif

       { Check if Phi interferes with another Phi }
       let Phi'=cur_phi(colPhi)
       if (Phi' != NULL)
         if (L is a predecessor of bbloc(Phi') )
           let a'k := the operand of Phi' coming from L
         else
           cur_phi(colPhi) := NULL;
           Phi' := NULL
         endif
       endif
       if (Phi' != NULL && a'k != aj)
         { Interfere; isolate Phi }
         col(Phi):=NULL
       else { update cur_phi }
         cur_phi(colPhi) := Phi
       endif
     endforeach
   endforeach
 endforeach
*/

void
Decoalescer_decoalesceWebs(Decoalescer this)
{
  Memory memory = Decoalescer_MEMORY(this);
  SSAForm ssaForm = Decoalescer_SSAFORM(this);
  Dominance dominance = SSAForm_DOMINANCE(ssaForm);
  Coalescer coalescer = Decoalescer_COALESCER(this);
  LiveCheck liveCheck = Coalescer_LIVECHECK(coalescer);
  TemporarySet_ *liveOutSets = Coalescer_LIVEOUTSETS(coalescer);
  //
  // foreach L in BBLOCS in DFS pre-order of the dominance tree
  Dominance_FORDFSORDER_BasicBlock(dominance, L) {
    // { Traversal of all definitions in L }
    // foreach Op in Operations(L) in topological order (including phis)
    PFA_TRACE && fprintf(PFA_TRACE, "Decoalescing: Traversing BB %d\n", BasicBlock_INDEX(L));
    BasicBlock_FOREACH_Operation(L, operation) {
      // foreach V in DefinedVariables(Op)
      Operation_RESULTS_FOREACH_Temporary(operation, result) {
        Variable V;
        Variable U = NULL;
        Variable c;
        if (!Temporary_isVariable(result)/* || !Variable_isConsuming(Temporary_VARIABLE(result))*/) continue;
        V = Temporary_VARIABLE(result);
        PFA_TRACE && fprintf(PFA_TRACE, "Testing interference of ")
                  && Variable_pretty(V, PFA_TRACE, false)
                  && fprintf(PFA_TRACE,"\n");
        // if ( col(c:=col(V))==NULL ) then next { V }
        c = Variable_color(V);
        if (c == NULL) continue;
        0 && PFA_TRACE && fprintf(PFA_TRACE, "... in ")
                  && Variable_pretty(c, PFA_TRACE, false)
                  && fprintf(PFA_TRACE, "\n");
        // { update cur_dom if necessary }
        // U := cur_dom(c)
        U = Variable_CURDOM(c);
        PFA_TRACE && fprintf(PFA_TRACE, "... with curdom  ");
        // while ( (U != NULL) && (!dominates(U,V) || col(U)==NULL) )
        while (   (U != NULL)
               && (   (Variable_color(U) == NULL)
                   || !Coalescer_dominatesVV(coalescer, U, V))) {
          // U := idom (U)
          PFA_TRACE && Variable_pretty(U, PFA_TRACE, false)
                    && fprintf(PFA_TRACE, " -> ");
          U = Variable_IDOM(U);
        } // endwhile
        PFA_TRACE && (U ? Variable_pretty(U, PFA_TRACE, false) : fprintf(PFA_TRACE, "NULL"))
                       && fprintf(PFA_TRACE,"\n");
        //
        // { See if U or V should be removed; update cur_dom & idom if necessary }
        // if U==NULL || !interfere(U,V)

        // { No interference: none removed; update cur_dom & idom}
        // idom(V):=U
        *Variable__IDOM(V) = U;
        if (Coalescer_EQUIVALUE == 0) {
          if (   (U == NULL)
                 || !Coalescer_intersectsVV(coalescer, U, V)) {
            // Nothing to do
          }
          else { // { U & V Interfere }
            bool isolateV;
            // See which one of U or V is preferable to isolate
            Except_CHECK( (Variable_PINNED(U) == Register__UNDEF) ||
                          (Variable_PINNED(V) == Register__UNDEF)    );
            // Simple strategy: isolate V whenever possible
            isolateV = ( ( Variable_PINNED(U) != Register__UNDEF ) ||
                         ( ( Variable_PINNED(V) == Register__UNDEF ) ) );
            //REVERSE	    isolateV = !( ( Variable_PINNED(V) != Register__UNDEF ) ||
            //REVERSE			 ( ( Variable_PINNED(U) == Register__UNDEF ) ) );
            if (isolateV) {
              // { Interference & V is preferable for removal: remove V }
              // col(V):=NULL
              Variable_raiseIsolated(V);
              PFA_TRACE && fprintf(PFA_TRACE, "Decoalescer_isolateVariable for variable: ");
              PFA_TRACE && Temporary_pretty(Variable_TEMPORARY(V), PFA_TRACE);
              PFA_TRACE && fprintf(PFA_TRACE, "\n");
            }
            // else
            else {
              // { Interference & U is preferable for removal: remove U;
              //   update cur_dom & idom }
              // col(U):=NULL
              Variable_raiseIsolated(U);
              PFA_TRACE && fprintf(PFA_TRACE, "Decoalescer_isolateVariable for variable: ");
              PFA_TRACE && Temporary_pretty(Variable_TEMPORARY(U), PFA_TRACE);
              PFA_TRACE && fprintf(PFA_TRACE, "\n");
            } // endif
          }
        }
        else {
          Variable cur_anc = U;
          while (cur_anc != NULL) {
            // Find the first intersecting (with V) variable that has
            // the same value as U. Denoted as cur_anc.
            while ((cur_anc != NULL) &&
                   !((Variable_EQUIVALUE(cur_anc) == Variable_EQUIVALUE(U)) &&
                     (Variable_color(cur_anc) != NULL) &&
                     Coalescer_intersectsVV(coalescer, cur_anc, V))) {
              cur_anc = Variable_EANC(cur_anc);
            }
            if (cur_anc == NULL)
              break;
            // No interference if they have the same value
            if (Variable_EQUIVALUE(V) == Variable_EQUIVALUE(U)) {
              *Variable__EANC(V) = cur_anc;
              // No more variables to check
              break;
            }
            else {
              // V and cur_anc interfere
              bool isolateV;
              // See which one of cur_anc or V is preferable to isolate
              Except_CHECK( (Variable_PINNED(cur_anc) == Register__UNDEF) ||
                            (Variable_PINNED(V) == Register__UNDEF)    );
              // Simple strategy: isolate V whenever possible
              isolateV = ( ( Variable_PINNED(cur_anc) != Register__UNDEF ) ||
                           ( ( Variable_PINNED(V) == Register__UNDEF ) ) );
              //REVERSE	      isolateV = !( ( Variable_PINNED(V) != Register__UNDEF ) ||
              //REVERSE			   ( ( Variable_PINNED(cur_anc) == Register__UNDEF ) ) );
              if (isolateV) {
                // { Interference & V is preferable for removal: remove V }
                // col(V):=NULL
                Variable_raiseIsolated(V);
                PFA_TRACE && fprintf(PFA_TRACE, "Decoalescer_isolateVariable for variable: ");
                PFA_TRACE && Temporary_pretty(Variable_TEMPORARY(V), PFA_TRACE);
                PFA_TRACE && fprintf(PFA_TRACE, "\n");
              }
              // else
              else {
                // { Interference & cur_anc is preferable for removal: remove cur_anc;
                //   update cur_dom & idom }
                // col(cur_anc):=NULL
                Variable_raiseIsolated(cur_anc);
                PFA_TRACE && fprintf(PFA_TRACE, "Decoalescer_isolateVariable for variable: ");
                PFA_TRACE && Temporary_pretty(Variable_TEMPORARY(cur_anc), PFA_TRACE);
                PFA_TRACE && fprintf(PFA_TRACE, "\n");
              } // endif
              if (isolateV)
                break;
            }
          }
        }
        // cur_dom(c):=V
        *Variable__CURDOM(c) = V;
        // endforeach { V }
      } Operation_RESULTS_ENDEACH_Temporary;
      // endforeach { OP }
    } BasicBlock_ENDEACH_Operation;
    //
    // { Initialize cur_phi }
    // foreach c in COLORS cur_phi(c) := NULL
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(L, edge) {
      BasicBlock Lp = BasicBlockEdge_destBlock(edge);
      if (BasicBlock_index(Lp) == 0) continue;
      BasicBlock_FOREACH_Operation(Lp, phi) {
        Variable colPhi;
        if (!Operator_isPhi(Operation_operator(phi)))
          break;
        colPhi = Operation_PHICOLOR(phi);
        if (colPhi != NULL)
          *Variable__CURPHI(colPhi) = NULL;
      } BasicBlock_ENDEACH_Operation;
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    // { Traversal of all implicit definitions related to phi in successor blocs }
    // foreach L' successor of L
    BasicBlock_LEAVE_FOREACH_BasicBlockEdge(L, edge) {
      BasicBlock Lp = BasicBlockEdge_destBlock(edge);
      // let j such that L is the j-th predecessor of L'
      int32_t j = BasicBlockEdge_ARGINDEX(edge);
      if (BasicBlock_index(Lp) == 0) continue;
      PFA_TRACE && fprintf(PFA_TRACE, "    Analysing PHIs in successor %d\n",
                                      BasicBlock_INDEX(Lp));
      //
      // foreach Phi: a0=phi(a1,...,an) in Operations(L')
      BasicBlock_FOREACH_Operation(Lp, phi) {
        Variable U = NULL;
        BasicBlock bb_U = NULL;
        Temporary argument_j;
        Variable aj, a_pk;
        Operation phi_p;
        Variable colPhi;
        if (!Operator_isPhi(Operation_operator(phi))) break;
        // let aj be the operand coming from L
        argument_j = Operation_getArgument(phi, j);
        aj = Temporary_VARIABLE(argument_j);
        // colPhi:=col(Phi)
        colPhi = Operation_PHICOLOR(phi);
        // if colPhi==NULL next
        if (colPhi == NULL) continue;
        //
        // { update cur_dom if necessary }
        // U := cur_dom(colPhi)
        U = Variable_CURDOM(colPhi);
        // while ( (U != NULL) && (!dominates(bbloc(def(U)),L) || col(U)==NULL) )
        while (   (U != NULL)
               && (   (Variable_color(U) == NULL)
                   || !Coalescer_dominatesVB(coalescer, U, L))) {
          // U := idom (U)
          U = Variable_IDOM(U);
        } // endwhile
        // cur_dom(colPhi):=U
        *Variable__CURDOM(colPhi) = U;
        //
        // { Check if Phi interferes with its related cur_dom U }
        // if U != NULL && U in liveOut(L) && U != aj
 	if (Coalescer_EQUIVALUE == 0) {
          if (   (U != NULL) && (U != aj)
                 && LiveCheck_isLiveOutBasicBlock(liveCheck, L, U, LiveCheckMode_Copy)) {
            // { U interfere with Phi }
            bool isolatePhi;
            // Simple strategy: if U is pinned, isolate Phi. 
            isolatePhi = ( (Variable_PINNED(U) != Register__UNDEF) );
            if (isolatePhi) {
              // { Interference & Phi is preferable for removal }
              // col(Phi):=NULL
              *Operation__PHICOLOR(phi) = NULL;
              PFA_TRACE && fprintf(PFA_TRACE, "Decoalescer_isolatePHI for operation: ");
              PFA_TRACE && Operation_pretty(phi, PFA_TRACE);
              // next
              continue;
            }
            // else
            else {
              // { Interference & U is preferable for removal }
              // col(U):=NULL
              Variable_raiseIsolated(U);
              PFA_TRACE && fprintf(PFA_TRACE, "Decoalescer_isolateVariable for variable: ");
              PFA_TRACE && Temporary_pretty(Variable_TEMPORARY(U), PFA_TRACE);
              PFA_TRACE && fprintf(PFA_TRACE, "\n");
            } // endif
          } // endif
        }
        else {
          Variable cur_anc = U;
          while (cur_anc != NULL) {
            // Find the first intersecting (with V) variable that has
            // the same value as U. Denoted as cur_anc.
            while ((cur_anc != NULL) &&
                   !((Variable_EQUIVALUE(cur_anc) == Variable_EQUIVALUE(U)) &&
                     (Variable_color(cur_anc) != NULL) &&
                     LiveCheck_isLiveOutBasicBlock(liveCheck, L, cur_anc, LiveCheckMode_Copy))) {
              cur_anc = Variable_EANC(cur_anc);
            }
            if (cur_anc == NULL)
              break;
            {
              bool isolatePhi;
              // Simple strategy: if U is pinned, isolate Phi. 
              isolatePhi = ( (Variable_PINNED(U) != Register__UNDEF) );
              if (isolatePhi) {
                // { Interference & Phi is preferable for removal }
                // col(Phi):=NULL
                *Operation__PHICOLOR(phi) = NULL;
                PFA_TRACE && fprintf(PFA_TRACE, "Decoalescer_isolatePHI for operation: ");
                PFA_TRACE && Operation_pretty(phi, PFA_TRACE);
              }
              // else
              else {
                // { Interference & U is preferable for removal }
                // col(U):=NULL
                Variable_raiseIsolated(U);
                PFA_TRACE && fprintf(PFA_TRACE, "Decoalescer_isolateVariable for variable: ");
                PFA_TRACE && Temporary_pretty(Variable_TEMPORARY(U), PFA_TRACE);
                PFA_TRACE && fprintf(PFA_TRACE, "\n");
                // PHI and cur_anc interfere
              }
              if (isolatePhi)
                break;
            }
          }
        }

        //
        // { Check if Phi interferes with another Phi }
        // let Phi'= cur_phi(colPhi)
        // if (Phi' != NULL)
        //   if (L is a predecessor of bbloc(Phi') )
        //     let a'k = the operand of Phi' coming from L
        //   else
        //     cur_phi(colPhi) = NULL;
        //     Phi' = NULL
        //   endif
        // endif
        phi_p = Variable_CURPHI(colPhi);
        a_pk = NULL;
        if (phi_p != NULL) {
          BasicBlock_LEAVE_FOREACH_BasicBlockEdge(L, edge_p) {
            BasicBlock Lp = BasicBlockEdge_destBlock(edge_p);
            if (BasicBlock_index(Lp) == 0) continue;
            if (Lp == Operation_BASICBLOCK(phi_p)) {
              int32_t k = BasicBlockEdge_ARGINDEX(edge_p);
              Temporary argument_pk = Operation_getArgument(phi_p, k);
              a_pk = Temporary_VARIABLE(argument_pk);
              break;
            }
          } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
          if (a_pk == NULL) {
            *Variable__CURPHI(colPhi) = NULL;
            phi_p = NULL;
          }
        }
        // if (Phi' != NULL && a'k != aj)
        if ((phi_p != NULL) && (a_pk != aj)) {
          // { Interfere; isolate Phi }
          // col(Phi):=NULL
          *Operation__PHICOLOR(phi) = NULL;
          PFA_TRACE && fprintf(PFA_TRACE, "Decoalescer_isolatePHI for operation: ");
          PFA_TRACE && Operation_pretty(phi, PFA_TRACE);
        }
        // else { update cur_phi }
        else {
          // cur_phi(colPhi) := Phi
          *Variable__CURPHI(colPhi) = phi;
        } // endif
        // endforeach
      } BasicBlock_ENDEACH_Operation;
      // endforeach
    } BasicBlock_LEAVE_ENDEACH_BasicBlockEdge;
    // endforeach
  } Dominance_ENDDFSORDER_BasicBlock;
}

#ifdef $XCC__h
/*
 * Decoalescer_explicitPhi --       Main processing for Generate_Copies
 */
void
Decoalescer_explicitPhi(Coalescer this, Operation phi);
#endif//$XCC__h

void
Decoalescer_explicitPhi(Coalescer this, Operation phi)
{
  CodeRegion codeRegion = Coalescer_CODEREGION(this);
  BasicBlock L0 = Operation_basicBlock(phi);
  Variable colorPhi;
  // Set colorPhi: the color of the phi variable
  colorPhi = Operation_PHICOLOR(phi);
  PFA_TRACE && fprintf(PFA_TRACE, "\t\t\t\tEXPLICIT") && Operation_pretty(phi, PFA_TRACE);
  // Foreach Lj, j-th predecessor of L:
  Operation_ARGUMENTS_FOREACH_Temporary(phi, temp_aj) {
    Operation pcj;
    Variable aj = Temporary_VARIABLE(temp_aj);
    Variable color_aj = Variable_color(aj);
    BasicBlock Lj = BasicBlockEdge_origBlock(BasicBlock_getPhiEnterEdge(L0, Operation_ARGUMENTS_INDEX));
    if (colorPhi != color_aj || (colorPhi == NULL)) { // split
      Variable ajp = NULL;
      // Create ajp and coalesce to colorPhi.
      // Check if the copy already exists.
      pcj = BasicBlock_makeOperationLCOPY(Lj);
      Operation_RESULTS_FOREACH_Temporary(pcj, result) {
        Variable V_dst = Temporary_VARIABLE(result);
        Variable colorDst = Variable_color(V_dst);
        if (colorPhi && (colorDst == colorPhi)) {
          Except_CHECK(Variable_EQUIVALUE(aj) == Variable_EQUIVALUE(V_dst));
          ajp = V_dst;
          break;
        }
      } Operation_RESULTS_ENDEACH_Temporary;
      if (ajp == NULL) {
        // Create the copy.
        int resIndex;
        Operation copy;
        Temporary temp_ajp = CodeRegion_makeVariableTemporary(codeRegion, Variable_ORIGIN(aj));
        ajp = Temporary_VARIABLE(temp_ajp);
        TemporaryTable_insert(Coalescer_TEMPORARYTABLE(this), temp_ajp);
        if (pcj != NULL) {
          copy = pcj;
          resIndex = Operation_addtoParCopy(pcj, temp_ajp, temp_aj);
        } else {
          copy = BasicBlock_makeLateOperationCOPY(Lj, temp_ajp, temp_aj);
          resIndex = 0;
        }
        Variable_setOperationResult(ajp, copy, resIndex);
        colorPhi = colorPhi ? Variable_linkWebRoots(colorPhi, ajp) : ajp;
        if (Coalescer_CONSUMING(this)) Variable_raiseConsuming(ajp);
        Variable_setOperationArgument(aj, copy, resIndex);
      }
      PFA_TRACE && fprintf(PFA_TRACE, "\t\t\t\t\tA%d: Split V%d -> V%d\n", Operation_ARGUMENTS_INDEX+1, Variable_IDENTITY(aj), Variable_IDENTITY(ajp));
      // Update the phi operand
      Variable_setOperationArgument(ajp, phi, Operation_ARGUMENTS_INDEX);
      // Update liveness sets
      if (Coalescer_USELIVESETS>0) TemporarySet_insert(Coalescer_LIVEOUTSETS(this)+BasicBlock_index(Lj), ajp);
      // Update interference graph
      if (Coalescer_INTERGRAPH>0) {
        Except_CHECK(Coalescer_USELIVESETS>0);
        // LiveCheckMode_Mixed.
        // ajp interferes with all variables liveout of the parallel copy
        Coalescer_addInterferenceVS(this, ajp, Coalescer_LIVEOUTSETS(this)+BasicBlock_index(Lj));
        // FIXME: addInterference with branching bool 
      }
    } else { // merge
      // Update the liveness sets
      if (Coalescer_USELIVESETS>0) {
        TemporarySet_insert(Coalescer_LIVEOUTSETS(this)+BasicBlock_index(Lj), aj);
        // Update the interference graph
        if (Coalescer_INTERGRAPH>0) {
          // LiveCheckMode_Mixed.
          // Additional interference with variables defined by the pcopy.
          // Already interferes with LIVEOUTSET + branch bool.
          Coalescer_addInterferenceVS(this, aj, Coalescer_LIVEOUTSETS(this)+BasicBlock_index(Lj));
          // FIXME: restrict to PCOPY defs.
        }
      }
      PFA_TRACE && fprintf(PFA_TRACE, "\t\t\t\t\tA%d: Merge V%d\n", Operation_ARGUMENTS_INDEX+1, Variable_IDENTITY(aj));
    }
  } Operation_ARGUMENTS_ENDEACH_Temporary;
  // L0 (j == 0)
  {
    Temporary temp_a0 = Operation_getResult(phi, 0);
    Variable a0 = Temporary_VARIABLE(temp_a0);
    Variable color_a0 = Variable_color(a0);
    if (color_a0 != colorPhi || (colorPhi == NULL)) { //split
      // Create a0p and coalesce to colorPhi
      int resIndex;
      Operation copy;
      Operation pc0 = NULL;
      Temporary temp_a0p = CodeRegion_makeVariableTemporary(codeRegion, Variable_ORIGIN(a0));
      Variable a0p = Temporary_VARIABLE(temp_a0p);
      TemporaryTable_insert(Coalescer_TEMPORARYTABLE(this), temp_a0p);
      // Create the copy
      pc0 = BasicBlock_makeOperationECOPY(L0);
      copy = pc0;
      resIndex = Operation_addtoParCopy(pc0, temp_a0p, temp_a0);
      Variable_setOperationResult(a0, copy, resIndex);
      Variable_setOperationArgument(a0p, copy, resIndex);
      Variable_setOperationResult(a0p, phi, 0);
      colorPhi = colorPhi ? Variable_linkWebRoots(colorPhi, a0p) : a0p;
      PFA_TRACE && fprintf(PFA_TRACE, "\t\t\t\t\tA0: Split V%d -> V%d\n", Variable_IDENTITY(a0), Variable_IDENTITY(a0p));
      if (Coalescer_CONSUMING(this)) Variable_raiseConsuming(a0p);
      // Update the liveness sets
      if (Coalescer_USELIVESETS>0) {
        // LiveCheckMode_Mixed.
        TemporarySet_insert(Coalescer_LIVEINSETS(this)+BasicBlock_index(L0), a0p);
      }
      // update interference graph
      if (Coalescer_INTERGRAPH>0) {
        // LiveCheckMode_Mixed.
        // a0p interferes with all variables livein of the parallel copy
        Coalescer_addInterferenceVS(this, a0p, Coalescer_LIVEINSETS(this)+BasicBlock_index(L0));
      }
    } else { //merge
      // update liveness sets
      if (Coalescer_USELIVESETS>0) {
        // LiveCheckMode_Mixed.
        TemporarySet_insert(Coalescer_LIVEINSETS(this)+BasicBlock_index(L0), a0);
        // update interference graph
        if (Coalescer_INTERGRAPH>0) {
          // LiveCheckMode_Mixed.
          // Additional interference with variables used by the pcopy.
          // Already interferes with LIVEINSET.
          // FIXME: use pcopy uses instead this trick
          if (Coalescer_PHIDEFSETS(this)) {
            Coalescer_addInterferenceVS(this, a0, Coalescer_PHIDEFSETS(this)+BasicBlock_index(L0));
          } else {
            Coalescer_addInterferenceVS(this, a0, Coalescer_LIVEINSETS(this)+BasicBlock_index(L0));
          }
        }
      }
      PFA_TRACE && fprintf(PFA_TRACE, "\t\t\t\t\tA0: Merge V%d\n", Variable_IDENTITY(a0));
    }
  }
}
/*@XCC_.h
 * Decoalescer_generateCopies --       Main processing for Generate_Copies.
 */

#ifdef $XCC__h
void
Decoalescer_generateCopies(Decoalescer this);
#endif//$XCC__h

void
Decoalescer_generateCopies(Decoalescer this)
{
  CodeRegion codeRegion = Decoalescer_CODEREGION(this);
  Coalescer coalescer = Decoalescer_COALESCER(this);
  CodeRegion_FOREACH_BasicBlock(codeRegion, L) {
    BasicBlock_FOREACH_Operation(L, phi) {
      if (Operator_isPhi(Operation_operator(phi))) {
        Decoalescer_explicitPhi(coalescer, phi);
        Operation_clearFlags(phi, OperationFlag_CopyMode);
      } else break;
    } BasicBlock_ENDEACH_Operation;
  } CodeRegion_ENDEACH_BasicBlock;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef $XCC__h
/**
 * Initialize the Decoalescer module.
 */
#define Decoalescer_INIT()
#endif//$XCC__h

#ifdef $XCC__h
/**
 * Finalize the Decoalescer module.
 */
#define Decoalescer_FINI()
#endif//$XCC__h

#if XCC__C
static void
Decoalescer_TEST(void)
{
#include "PFA/Decoalescer_.c"
}

int
main(int argc, char **argv)
{
  PFA_Test(Decoalescer_TEST);
  return 0;
}
#endif

