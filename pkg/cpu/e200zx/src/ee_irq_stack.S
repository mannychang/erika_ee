/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2010  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation, 
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */
/*
 * IRQ low-level code, based on the mico32 version.
 * Author: 2010 Fabio Checconi
 *         2014 Francesco Esposito (Added configuration dependency)
 */

#include "eecfg.h"
#include "cpu/e200zx/inc/ee_regs.h"
#ifdef __ASM_CONVERTED_TO_C__
#include "cpu/e200zx/inc/ee_e200zx_irq.h"
#endif

#ifndef	EE_GNU__
/* IRQ tos defined in eecfg.c */
/* CW-VAR-DEFINITION: extern struct EE_TOS EE_e200z7_IRQ_tos; */
  .extern EE_e200z7_IRQ_tos
#endif

/* CW-VAR-DEFINITION: EE_ADDR EE_e200zx_tmp_tos; */
  .section .bss
  .global   EE_e200zx_tmp_tos
  .type     EE_e200zx_tmp_tos, @object
  .balign   4
EE_e200zx_tmp_tos:
  .space    4
  .size	EE_e200zx_tmp_tos, .-EE_e200zx_tmp_tos


/* Start code section
    I need to preserve TAB caracter .text directive because otherwise
    codewarrior AWK translation script would keep that line in
    c file. (This approach is REALLY error prone it would be better use directly
    c files with embedded asm). */
	.text

/* CW-FUNC-DEFINITION: void EE_e200zx_call_ISR(EE_e200z7_ISR_handler fun, EE_UREG nesting) */
  .global EE_e200zx_call_ISR
  .type   EE_e200zx_call_ISR, @function
EE_e200zx_call_ISR:
  /*
   * r3 == fun            (parameter)
   * r4 == nesting_level  (parameter)
   * r5 == internal temporary register
   */

  /* Function Frame */
  stwu  sp, -16(sp)
  mfspr r0, SPRG_LR
  stw   r0, 20(sp)

  /* if (nesting_level == 1) */
#ifdef	__ALLOW_NESTED_IRQ__

#ifdef	__VLE__
  e_cmpi  cr0, r4, 1
#else
  cmpi    cr0, r4, 1
#endif /* VLE */
  bne   skip_change
#endif /* __ALLOW_NESTED_IRQ__ */

  /* change_to_IRQ_stacks(); */
  /* Save old Stack Pointer in EE_e200zx_tmp_tos global */
  addis r5, 0, EE_e200zx_tmp_tos@ha
  stw   sp,    EE_e200zx_tmp_tos@l(r5)

  /* Switch to IRQ stack */
  addis r5, 0, EE_e200z7_IRQ_tos@ha
  lwz   sp,    EE_e200z7_IRQ_tos@l(r5)

  /* Call fun handling the nesting */
  /* EE_std_enableIRQ_nested(); */
#ifdef	__ALLOW_NESTED_IRQ__
  wrteei	1
#endif
  /* call fun(); */
  mtspr	SPRG_LR, r3
  blrl
  /* EE_std_disableIRQ_nested(); */
#ifdef	__ALLOW_NESTED_IRQ__
  wrteei	0
#endif

  /* change_back_from_IRQ_stacks(); */
  /* Restore old Stack pointer */
  addis r5, 0, EE_e200zx_tmp_tos@ha
  lwz   sp,    EE_e200zx_tmp_tos@l(r5)

  /* branch to the end (not execute else branch) */
  b frame_free

skip_change:
  /* I don't need to change the stack just call fun */
  /* EE_std_enableIRQ_nested(); */
#ifdef	__ALLOW_NESTED_IRQ__
  wrteei	1
#endif
  /* call fun(); */
  mtspr	SPRG_LR, r3
  blrl
  /* EE_std_disableIRQ_nested(); */
#ifdef	__ALLOW_NESTED_IRQ__
  wrteei	0
#endif

frame_free:
  /* Free the function frame */
  lwz   r0,       20(sp)
  addi  sp, sp,   16
  mtspr SPRG_LR,  r0
  /* return */
  blr

  .size EE_e200zx_call_ISR, .-EE_e200zx_call_ISR


  .global EE_e200zx_call_function_in_new_stack
  .type   EE_e200zx_call_function_in_new_stack, @function
EE_e200zx_call_function_in_new_stack:
  /*
   * r3 == fun parameter  (parameter)
   * r4 == fun            (parameter)
   * r5 == new stack      (parameter) + internal temporary register
   * r0 == internal temporary register
   */

  /* !!! 2 Function Frame on the NEW stack ( to better hold the info ) !!! */
  /* Temporary hold old SP in r0 */
  addi  r0, sp, 0

  /* Switch to the NEW stack */
  addi  sp, r5, 0

  /* Prepare the Backlink to the fake first frame */
  addi  r5, sp, -16
  /* 2 Function Frame On the NEW Stack, with the last back link already set */
  stwu  r5, -32(sp)
  /* Save the OLD stack on the Frame */
  stw   r0, 16(sp)
  /* Save the Linker Register */
  mfspr r0, SPRG_LR
  stw   r0, 20(sp)

  /* call fun(); */
  mtspr	SPRG_LR, r4
  blrl

#if 0
  /* Equivalent way to execute Branch without change LR */
  mtspr SPRG_CTR,r4  /* fun in CTR */
  bctrl              /* Call the Func */
#endif

  /* Free the function frame: */
  /* Restore the LR */
  lwz   r0,       20(sp)
  mtspr SPRG_LR,  r0
  /* Load the OLD SP */
  lwz   sp,       16(sp)
  /* return */
  blr

  .size EE_e200zx_call_function_in_new_stack, .-EE_e200zx_call_function_in_new_stack

