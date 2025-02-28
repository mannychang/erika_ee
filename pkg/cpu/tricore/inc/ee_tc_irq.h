/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2012  Evidence Srl
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

 /** @file  ee_tc_irq.h
  *  @brief Header with definition for Interrupt handling
  *  @author Errico Guidieri
  *  @date 2012
  */
#ifndef INCLUDE_EE_TC_IRQ_H__
#define INCLUDE_EE_TC_IRQ_H__

/* I need base API */
#include "cpu/tricore/inc/ee_tc_cpu.h"

/* Needed by common irq stub */
#ifdef __ALLOW_NESTED_IRQ__

#define EE_std_enableIRQ_nested() EE_tc_enableIRQ()
#define EE_std_disableIRQ_nested() EE_tc_disableIRQ()

#else   /* else __ALLOW_NESTED_IRQ__*/

#define EE_std_enableIRQ_nested() ((void)0)
#define EE_std_disableIRQ_nested() ((void)0)

#endif  /* end __ALLOW_NESTED_IRQ__*/

/* ISR Management Macros */
#if (defined(EE_ERIKA_ISR_HANDLING_OFF))
/* If the user want generate his own Interrupt Vector give to him
   enriched macros for code generation */
#include "cpu/tricore/inc/ee_tc_irq_internal.h"

/* Define an ISR (category 1). */
#define ISR1(f)                          \
void EE_PREPROC_JOIN(ISR1_,f) ( void );  \
void f ( void )                          \
{                                        \
  EE_std_enableIRQ_nested();             \
  EE_PREPROC_JOIN(ISR1_,f)();            \
  EE_std_disableIRQ_nested();            \
}                                        \
void EE_PREPROC_JOIN(ISR1_,f) ( void )

/* Define an ISR (category 2). */
#define ISR2(f)                          \
void EE_PREPROC_JOIN(ISR2_,f) ( void );  \
void f ( void )                          \
{                                        \
  EE_ISR2_WRAPPER_BODY(                  \
    EE_PREPROC_JOIN(ISR2_,f)             \
  );                                     \
}                                        \
void EE_PREPROC_JOIN(ISR2_,f) ( void )

#else /* EE_ERIKA_ISR_HANDLING_OFF */

/* Declare an ISR (category 1) */
#define ISR1(f) void f ( void )

/* Define an ISR (category 2). Used only for client code. The wrapper is moved
   inside ee_tricore_intvec.c to isulate better user code from Kernel code.
   In TriCore Architecture ISR ID and ISR Priority are the same.
   I Use ISR ID as interrupt vector entry because is tied to handler name
   and I can easily reconstruct that here */
#define ISR2(f) ISR1(f)

/* OSEK Standard Macro for ISR declaration */
#define ISR(f) void f ( void )

#endif /* EE_ERIKA_ISR_HANDLING_OFF */

/* Used To resolve Interrupt Vector */
#if (!defined(EE_ERIKA_ISR_HANDLING_OFF)) && defined (__TASKING__)

#define EE_TC_ISR2_WRAP(f) EE_PREPROC_JOIN(ISR2_,f)

#if defined(EE_TC_1_ISR) && (EE_TC_1_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_1_ISR)
#endif /* EE_TC_1_ISR && (EE_TC_1_ISR_CAT == 1) */
#if defined(EE_TC_1_ISR) && (EE_TC_1_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_1_ISR))
#endif /* EE_TC_1_ISR && (EE_TC_1_ISR_CAT == 2) */
#if defined(EE_TC_2_ISR) && (EE_TC_2_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_2_ISR)
#endif /* EE_TC_2_ISR && (EE_TC_2_ISR_CAT == 1) */
#if defined(EE_TC_2_ISR) && (EE_TC_2_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_2_ISR))
#endif /* EE_TC_2_ISR && (EE_TC_2_ISR_CAT == 2) */
#if defined(EE_TC_3_ISR) && (EE_TC_3_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_3_ISR)
#endif /* EE_TC_3_ISR && (EE_TC_3_ISR_CAT == 1) */
#if defined(EE_TC_3_ISR) && (EE_TC_3_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_3_ISR))
#endif /* EE_TC_3_ISR && (EE_TC_3_ISR_CAT == 2) */
#if defined(EE_TC_4_ISR) && (EE_TC_4_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_4_ISR)
#endif /* EE_TC_4_ISR && (EE_TC_4_ISR_CAT == 1) */
#if defined(EE_TC_4_ISR) && (EE_TC_4_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_4_ISR))
#endif /* EE_TC_4_ISR && (EE_TC_4_ISR_CAT == 2) */
#if defined(EE_TC_5_ISR) && (EE_TC_5_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_5_ISR)
#endif /* EE_TC_5_ISR && (EE_TC_5_ISR_CAT == 1) */
#if defined(EE_TC_5_ISR) && (EE_TC_5_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_5_ISR))
#endif /* EE_TC_5_ISR && (EE_TC_5_ISR_CAT == 2) */
#if defined(EE_TC_6_ISR) && (EE_TC_6_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_6_ISR)
#endif /* EE_TC_6_ISR && (EE_TC_6_ISR_CAT == 1) */
#if defined(EE_TC_6_ISR) && (EE_TC_6_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_6_ISR))
#endif /* EE_TC_6_ISR && (EE_TC_6_ISR_CAT == 2) */
#if defined(EE_TC_7_ISR) && (EE_TC_7_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_7_ISR)
#endif /* EE_TC_7_ISR && (EE_TC_7_ISR_CAT == 1) */
#if defined(EE_TC_7_ISR) && (EE_TC_7_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_7_ISR))
#endif /* EE_TC_7_ISR && (EE_TC_7_ISR_CAT == 2) */
#if defined(EE_TC_8_ISR) && (EE_TC_8_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_8_ISR)
#endif /* EE_TC_8_ISR && (EE_TC_8_ISR_CAT == 1) */
#if defined(EE_TC_8_ISR) && (EE_TC_8_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_8_ISR))
#endif /* EE_TC_8_ISR && (EE_TC_8_ISR_CAT == 2) */
#if defined(EE_TC_9_ISR) && (EE_TC_9_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_9_ISR)
#endif /* EE_TC_9_ISR && (EE_TC_9_ISR_CAT == 1) */
#if defined(EE_TC_9_ISR) && (EE_TC_9_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_9_ISR))
#endif /* EE_TC_9_ISR && (EE_TC_9_ISR_CAT == 2) */
#if defined(EE_TC_10_ISR) && (EE_TC_10_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_10_ISR)
#endif /* EE_TC_10_ISR && (EE_TC_10_ISR_CAT == 1) */
#if defined(EE_TC_10_ISR) && (EE_TC_10_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_10_ISR))
#endif /* EE_TC_10_ISR && (EE_TC_10_ISR_CAT == 2) */
#if defined(EE_TC_11_ISR) && (EE_TC_11_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_11_ISR)
#endif /* EE_TC_11_ISR && (EE_TC_11_ISR_CAT == 1) */
#if defined(EE_TC_11_ISR) && (EE_TC_11_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_11_ISR))
#endif /* EE_TC_11_ISR && (EE_TC_11_ISR_CAT == 2) */
#if defined(EE_TC_12_ISR) && (EE_TC_12_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_12_ISR)
#endif /* EE_TC_12_ISR && (EE_TC_12_ISR_CAT == 1) */
#if defined(EE_TC_12_ISR) && (EE_TC_12_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_12_ISR))
#endif /* EE_TC_12_ISR && (EE_TC_12_ISR_CAT == 2) */
#if defined(EE_TC_13_ISR) && (EE_TC_13_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_13_ISR)
#endif /* EE_TC_13_ISR && (EE_TC_13_ISR_CAT == 1) */
#if defined(EE_TC_13_ISR) && (EE_TC_13_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_13_ISR))
#endif /* EE_TC_13_ISR && (EE_TC_13_ISR_CAT == 2) */
#if defined(EE_TC_14_ISR) && (EE_TC_14_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_14_ISR)
#endif /* EE_TC_14_ISR && (EE_TC_14_ISR_CAT == 1) */
#if defined(EE_TC_14_ISR) && (EE_TC_14_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_14_ISR))
#endif /* EE_TC_14_ISR && (EE_TC_14_ISR_CAT == 2) */
#if defined(EE_TC_15_ISR) && (EE_TC_15_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_15_ISR)
#endif /* EE_TC_15_ISR && (EE_TC_15_ISR_CAT == 1) */
#if defined(EE_TC_15_ISR) && (EE_TC_15_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_15_ISR))
#endif /* EE_TC_15_ISR && (EE_TC_15_ISR_CAT == 2) */
#if defined(EE_TC_16_ISR) && (EE_TC_16_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_16_ISR)
#endif /* EE_TC_16_ISR && (EE_TC_16_ISR_CAT == 1) */
#if defined(EE_TC_16_ISR) && (EE_TC_16_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_16_ISR))
#endif /* EE_TC_16_ISR && (EE_TC_16_ISR_CAT == 2) */
#if defined(EE_TC_17_ISR) && (EE_TC_17_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_17_ISR)
#endif /* EE_TC_17_ISR && (EE_TC_17_ISR_CAT == 1) */
#if defined(EE_TC_17_ISR) && (EE_TC_17_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_17_ISR))
#endif /* EE_TC_17_ISR && (EE_TC_17_ISR_CAT == 2) */
#if defined(EE_TC_18_ISR) && (EE_TC_18_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_18_ISR)
#endif /* EE_TC_18_ISR && (EE_TC_18_ISR_CAT == 1) */
#if defined(EE_TC_18_ISR) && (EE_TC_18_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_18_ISR))
#endif /* EE_TC_18_ISR && (EE_TC_18_ISR_CAT == 2) */
#if defined(EE_TC_19_ISR) && (EE_TC_19_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_19_ISR)
#endif /* EE_TC_19_ISR && (EE_TC_19_ISR_CAT == 1) */
#if defined(EE_TC_19_ISR) && (EE_TC_19_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_19_ISR))
#endif /* EE_TC_19_ISR && (EE_TC_19_ISR_CAT == 2) */
#if defined(EE_TC_20_ISR) && (EE_TC_20_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_20_ISR)
#endif /* EE_TC_20_ISR && (EE_TC_20_ISR_CAT == 1) */
#if defined(EE_TC_20_ISR) && (EE_TC_20_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_20_ISR))
#endif /* EE_TC_20_ISR && (EE_TC_20_ISR_CAT == 2) */
#if defined(EE_TC_21_ISR) && (EE_TC_21_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_21_ISR)
#endif /* EE_TC_21_ISR && (EE_TC_21_ISR_CAT == 1) */
#if defined(EE_TC_21_ISR) && (EE_TC_21_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_21_ISR))
#endif /* EE_TC_21_ISR && (EE_TC_21_ISR_CAT == 2) */
#if defined(EE_TC_22_ISR) && (EE_TC_22_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_22_ISR)
#endif /* EE_TC_22_ISR && (EE_TC_22_ISR_CAT == 1) */
#if defined(EE_TC_22_ISR) && (EE_TC_22_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_22_ISR))
#endif /* EE_TC_22_ISR && (EE_TC_22_ISR_CAT == 2) */
#if defined(EE_TC_23_ISR) && (EE_TC_23_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_23_ISR)
#endif /* EE_TC_23_ISR && (EE_TC_23_ISR_CAT == 1) */
#if defined(EE_TC_23_ISR) && (EE_TC_23_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_23_ISR))
#endif /* EE_TC_23_ISR && (EE_TC_23_ISR_CAT == 2) */
#if defined(EE_TC_24_ISR) && (EE_TC_24_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_24_ISR)
#endif /* EE_TC_24_ISR && (EE_TC_24_ISR_CAT == 1) */
#if defined(EE_TC_24_ISR) && (EE_TC_24_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_24_ISR))
#endif /* EE_TC_24_ISR && (EE_TC_24_ISR_CAT == 2) */
#if defined(EE_TC_25_ISR) && (EE_TC_25_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_25_ISR)
#endif /* EE_TC_25_ISR && (EE_TC_25_ISR_CAT == 1) */
#if defined(EE_TC_25_ISR) && (EE_TC_25_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_25_ISR))
#endif /* EE_TC_25_ISR && (EE_TC_25_ISR_CAT == 2) */
#if defined(EE_TC_26_ISR) && (EE_TC_26_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_26_ISR)
#endif /* EE_TC_26_ISR && (EE_TC_26_ISR_CAT == 1) */
#if defined(EE_TC_26_ISR) && (EE_TC_26_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_26_ISR))
#endif /* EE_TC_26_ISR && (EE_TC_26_ISR_CAT == 2) */
#if defined(EE_TC_27_ISR) && (EE_TC_27_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_27_ISR)
#endif /* EE_TC_27_ISR && (EE_TC_27_ISR_CAT == 1) */
#if defined(EE_TC_27_ISR) && (EE_TC_27_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_27_ISR))
#endif /* EE_TC_27_ISR && (EE_TC_27_ISR_CAT == 2) */
#if defined(EE_TC_28_ISR) && (EE_TC_28_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_28_ISR)
#endif /* EE_TC_28_ISR && (EE_TC_28_ISR_CAT == 1) */
#if defined(EE_TC_28_ISR) && (EE_TC_28_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_28_ISR))
#endif /* EE_TC_28_ISR && (EE_TC_28_ISR_CAT == 2) */
#if defined(EE_TC_29_ISR) && (EE_TC_29_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_29_ISR)
#endif /* EE_TC_29_ISR && (EE_TC_29_ISR_CAT == 1) */
#if defined(EE_TC_29_ISR) && (EE_TC_29_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_29_ISR))
#endif /* EE_TC_29_ISR && (EE_TC_29_ISR_CAT == 2) */
#if defined(EE_TC_30_ISR) && (EE_TC_30_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_30_ISR)
#endif /* EE_TC_30_ISR && (EE_TC_30_ISR_CAT == 1) */
#if defined(EE_TC_30_ISR) && (EE_TC_30_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_30_ISR))
#endif /* EE_TC_30_ISR && (EE_TC_30_ISR_CAT == 2) */
#if defined(EE_TC_31_ISR) && (EE_TC_31_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_31_ISR)
#endif /* EE_TC_31_ISR && (EE_TC_31_ISR_CAT == 1) */
#if defined(EE_TC_31_ISR) && (EE_TC_31_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_31_ISR))
#endif /* EE_TC_31_ISR && (EE_TC_31_ISR_CAT == 2) */
#if defined(EE_TC_32_ISR) && (EE_TC_32_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_32_ISR)
#endif /* EE_TC_32_ISR && (EE_TC_32_ISR_CAT == 1) */
#if defined(EE_TC_32_ISR) && (EE_TC_32_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_32_ISR))
#endif /* EE_TC_32_ISR && (EE_TC_32_ISR_CAT == 2) */
#if defined(EE_TC_33_ISR) && (EE_TC_33_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_33_ISR)
#endif /* EE_TC_33_ISR && (EE_TC_33_ISR_CAT == 1) */
#if defined(EE_TC_33_ISR) && (EE_TC_33_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_33_ISR))
#endif /* EE_TC_33_ISR && (EE_TC_33_ISR_CAT == 2) */
#if defined(EE_TC_34_ISR) && (EE_TC_34_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_34_ISR)
#endif /* EE_TC_34_ISR && (EE_TC_34_ISR_CAT == 1) */
#if defined(EE_TC_34_ISR) && (EE_TC_34_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_34_ISR))
#endif /* EE_TC_34_ISR && (EE_TC_34_ISR_CAT == 2) */
#if defined(EE_TC_35_ISR) && (EE_TC_35_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_35_ISR)
#endif /* EE_TC_35_ISR && (EE_TC_35_ISR_CAT == 1) */
#if defined(EE_TC_35_ISR) && (EE_TC_35_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_35_ISR))
#endif /* EE_TC_35_ISR && (EE_TC_35_ISR_CAT == 2) */
#if defined(EE_TC_36_ISR) && (EE_TC_36_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_36_ISR)
#endif /* EE_TC_36_ISR && (EE_TC_36_ISR_CAT == 1) */
#if defined(EE_TC_36_ISR) && (EE_TC_36_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_36_ISR))
#endif /* EE_TC_36_ISR && (EE_TC_36_ISR_CAT == 2) */
#if defined(EE_TC_37_ISR) && (EE_TC_37_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_37_ISR)
#endif /* EE_TC_37_ISR && (EE_TC_37_ISR_CAT == 1) */
#if defined(EE_TC_37_ISR) && (EE_TC_37_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_37_ISR))
#endif /* EE_TC_37_ISR && (EE_TC_37_ISR_CAT == 2) */
#if defined(EE_TC_38_ISR) && (EE_TC_38_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_38_ISR)
#endif /* EE_TC_38_ISR && (EE_TC_38_ISR_CAT == 1) */
#if defined(EE_TC_38_ISR) && (EE_TC_38_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_38_ISR))
#endif /* EE_TC_38_ISR && (EE_TC_38_ISR_CAT == 2) */
#if defined(EE_TC_39_ISR) && (EE_TC_39_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_39_ISR)
#endif /* EE_TC_39_ISR && (EE_TC_39_ISR_CAT == 1) */
#if defined(EE_TC_39_ISR) && (EE_TC_39_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_39_ISR))
#endif /* EE_TC_39_ISR && (EE_TC_39_ISR_CAT == 2) */
#if defined(EE_TC_40_ISR) && (EE_TC_40_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_40_ISR)
#endif /* EE_TC_40_ISR && (EE_TC_40_ISR_CAT == 1) */
#if defined(EE_TC_40_ISR) && (EE_TC_40_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_40_ISR))
#endif /* EE_TC_40_ISR && (EE_TC_40_ISR_CAT == 2) */
#if defined(EE_TC_41_ISR) && (EE_TC_41_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_41_ISR)
#endif /* EE_TC_41_ISR && (EE_TC_41_ISR_CAT == 1) */
#if defined(EE_TC_41_ISR) && (EE_TC_41_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_41_ISR))
#endif /* EE_TC_41_ISR && (EE_TC_41_ISR_CAT == 2) */
#if defined(EE_TC_42_ISR) && (EE_TC_42_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_42_ISR)
#endif /* EE_TC_42_ISR && (EE_TC_42_ISR_CAT == 1) */
#if defined(EE_TC_42_ISR) && (EE_TC_42_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_42_ISR))
#endif /* EE_TC_42_ISR && (EE_TC_42_ISR_CAT == 2) */
#if defined(EE_TC_43_ISR) && (EE_TC_43_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_43_ISR)
#endif /* EE_TC_43_ISR && (EE_TC_43_ISR_CAT == 1) */
#if defined(EE_TC_43_ISR) && (EE_TC_43_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_43_ISR))
#endif /* EE_TC_43_ISR && (EE_TC_43_ISR_CAT == 2) */
#if defined(EE_TC_44_ISR) && (EE_TC_44_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_44_ISR)
#endif /* EE_TC_44_ISR && (EE_TC_44_ISR_CAT == 1) */
#if defined(EE_TC_44_ISR) && (EE_TC_44_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_44_ISR))
#endif /* EE_TC_44_ISR && (EE_TC_44_ISR_CAT == 2) */
#if defined(EE_TC_45_ISR) && (EE_TC_45_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_45_ISR)
#endif /* EE_TC_45_ISR && (EE_TC_45_ISR_CAT == 1) */
#if defined(EE_TC_45_ISR) && (EE_TC_45_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_45_ISR))
#endif /* EE_TC_45_ISR && (EE_TC_45_ISR_CAT == 2) */
#if defined(EE_TC_46_ISR) && (EE_TC_46_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_46_ISR)
#endif /* EE_TC_46_ISR && (EE_TC_46_ISR_CAT == 1) */
#if defined(EE_TC_46_ISR) && (EE_TC_46_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_46_ISR))
#endif /* EE_TC_46_ISR && (EE_TC_46_ISR_CAT == 2) */
#if defined(EE_TC_47_ISR) && (EE_TC_47_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_47_ISR)
#endif /* EE_TC_47_ISR && (EE_TC_47_ISR_CAT == 1) */
#if defined(EE_TC_47_ISR) && (EE_TC_47_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_47_ISR))
#endif /* EE_TC_47_ISR && (EE_TC_47_ISR_CAT == 2) */
#if defined(EE_TC_48_ISR) && (EE_TC_48_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_48_ISR)
#endif /* EE_TC_48_ISR && (EE_TC_48_ISR_CAT == 1) */
#if defined(EE_TC_48_ISR) && (EE_TC_48_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_48_ISR))
#endif /* EE_TC_48_ISR && (EE_TC_48_ISR_CAT == 2) */
#if defined(EE_TC_49_ISR) && (EE_TC_49_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_49_ISR)
#endif /* EE_TC_49_ISR && (EE_TC_49_ISR_CAT == 1) */
#if defined(EE_TC_49_ISR) && (EE_TC_49_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_49_ISR))
#endif /* EE_TC_49_ISR && (EE_TC_49_ISR_CAT == 2) */
#if defined(EE_TC_50_ISR) && (EE_TC_50_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_50_ISR)
#endif /* EE_TC_50_ISR && (EE_TC_50_ISR_CAT == 1) */
#if defined(EE_TC_50_ISR) && (EE_TC_50_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_50_ISR))
#endif /* EE_TC_50_ISR && (EE_TC_50_ISR_CAT == 2) */
#if defined(EE_TC_51_ISR) && (EE_TC_51_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_51_ISR)
#endif /* EE_TC_51_ISR && (EE_TC_51_ISR_CAT == 1) */
#if defined(EE_TC_51_ISR) && (EE_TC_51_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_51_ISR))
#endif /* EE_TC_51_ISR && (EE_TC_51_ISR_CAT == 2) */
#if defined(EE_TC_52_ISR) && (EE_TC_52_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_52_ISR)
#endif /* EE_TC_52_ISR && (EE_TC_52_ISR_CAT == 1) */
#if defined(EE_TC_52_ISR) && (EE_TC_52_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_52_ISR))
#endif /* EE_TC_52_ISR && (EE_TC_52_ISR_CAT == 2) */
#if defined(EE_TC_53_ISR) && (EE_TC_53_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_53_ISR)
#endif /* EE_TC_53_ISR && (EE_TC_53_ISR_CAT == 1) */
#if defined(EE_TC_53_ISR) && (EE_TC_53_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_53_ISR))
#endif /* EE_TC_53_ISR && (EE_TC_53_ISR_CAT == 2) */
#if defined(EE_TC_54_ISR) && (EE_TC_54_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_54_ISR)
#endif /* EE_TC_54_ISR && (EE_TC_54_ISR_CAT == 1) */
#if defined(EE_TC_54_ISR) && (EE_TC_54_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_54_ISR))
#endif /* EE_TC_54_ISR && (EE_TC_54_ISR_CAT == 2) */
#if defined(EE_TC_55_ISR) && (EE_TC_55_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_55_ISR)
#endif /* EE_TC_55_ISR && (EE_TC_55_ISR_CAT == 1) */
#if defined(EE_TC_55_ISR) && (EE_TC_55_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_55_ISR))
#endif /* EE_TC_55_ISR && (EE_TC_55_ISR_CAT == 2) */
#if defined(EE_TC_56_ISR) && (EE_TC_56_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_56_ISR)
#endif /* EE_TC_56_ISR && (EE_TC_56_ISR_CAT == 1) */
#if defined(EE_TC_56_ISR) && (EE_TC_56_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_56_ISR))
#endif /* EE_TC_56_ISR && (EE_TC_56_ISR_CAT == 2) */
#if defined(EE_TC_57_ISR) && (EE_TC_57_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_57_ISR)
#endif /* EE_TC_57_ISR && (EE_TC_57_ISR_CAT == 1) */
#if defined(EE_TC_57_ISR) && (EE_TC_57_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_57_ISR))
#endif /* EE_TC_57_ISR && (EE_TC_57_ISR_CAT == 2) */
#if defined(EE_TC_58_ISR) && (EE_TC_58_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_58_ISR)
#endif /* EE_TC_58_ISR && (EE_TC_58_ISR_CAT == 1) */
#if defined(EE_TC_58_ISR) && (EE_TC_58_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_58_ISR))
#endif /* EE_TC_58_ISR && (EE_TC_58_ISR_CAT == 2) */
#if defined(EE_TC_59_ISR) && (EE_TC_59_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_59_ISR)
#endif /* EE_TC_59_ISR && (EE_TC_59_ISR_CAT == 1) */
#if defined(EE_TC_59_ISR) && (EE_TC_59_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_59_ISR))
#endif /* EE_TC_59_ISR && (EE_TC_59_ISR_CAT == 2) */
#if defined(EE_TC_60_ISR) && (EE_TC_60_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_60_ISR)
#endif /* EE_TC_60_ISR && (EE_TC_60_ISR_CAT == 1) */
#if defined(EE_TC_60_ISR) && (EE_TC_60_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_60_ISR))
#endif /* EE_TC_60_ISR && (EE_TC_60_ISR_CAT == 2) */
#if defined(EE_TC_61_ISR) && (EE_TC_61_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_61_ISR)
#endif /* EE_TC_61_ISR && (EE_TC_61_ISR_CAT == 1) */
#if defined(EE_TC_61_ISR) && (EE_TC_61_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_61_ISR))
#endif /* EE_TC_61_ISR && (EE_TC_61_ISR_CAT == 2) */
#if defined(EE_TC_62_ISR) && (EE_TC_62_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_62_ISR)
#endif /* EE_TC_62_ISR && (EE_TC_62_ISR_CAT == 1) */
#if defined(EE_TC_62_ISR) && (EE_TC_62_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_62_ISR))
#endif /* EE_TC_62_ISR && (EE_TC_62_ISR_CAT == 2) */
#if defined(EE_TC_63_ISR) && (EE_TC_63_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_63_ISR)
#endif /* EE_TC_63_ISR && (EE_TC_63_ISR_CAT == 1) */
#if defined(EE_TC_63_ISR) && (EE_TC_63_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_63_ISR))
#endif /* EE_TC_63_ISR && (EE_TC_63_ISR_CAT == 2) */
#if defined(EE_TC_64_ISR) && (EE_TC_64_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_64_ISR)
#endif /* EE_TC_64_ISR && (EE_TC_64_ISR_CAT == 1) */
#if defined(EE_TC_64_ISR) && (EE_TC_64_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_64_ISR))
#endif /* EE_TC_64_ISR && (EE_TC_64_ISR_CAT == 2) */
#if defined(EE_TC_65_ISR) && (EE_TC_65_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_65_ISR)
#endif /* EE_TC_65_ISR && (EE_TC_65_ISR_CAT == 1) */
#if defined(EE_TC_65_ISR) && (EE_TC_65_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_65_ISR))
#endif /* EE_TC_65_ISR && (EE_TC_65_ISR_CAT == 2) */
#if defined(EE_TC_66_ISR) && (EE_TC_66_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_66_ISR)
#endif /* EE_TC_66_ISR && (EE_TC_66_ISR_CAT == 1) */
#if defined(EE_TC_66_ISR) && (EE_TC_66_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_66_ISR))
#endif /* EE_TC_66_ISR && (EE_TC_66_ISR_CAT == 2) */
#if defined(EE_TC_67_ISR) && (EE_TC_67_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_67_ISR)
#endif /* EE_TC_67_ISR && (EE_TC_67_ISR_CAT == 1) */
#if defined(EE_TC_67_ISR) && (EE_TC_67_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_67_ISR))
#endif /* EE_TC_67_ISR && (EE_TC_67_ISR_CAT == 2) */
#if defined(EE_TC_68_ISR) && (EE_TC_68_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_68_ISR)
#endif /* EE_TC_68_ISR && (EE_TC_68_ISR_CAT == 1) */
#if defined(EE_TC_68_ISR) && (EE_TC_68_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_68_ISR))
#endif /* EE_TC_68_ISR && (EE_TC_68_ISR_CAT == 2) */
#if defined(EE_TC_69_ISR) && (EE_TC_69_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_69_ISR)
#endif /* EE_TC_69_ISR && (EE_TC_69_ISR_CAT == 1) */
#if defined(EE_TC_69_ISR) && (EE_TC_69_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_69_ISR))
#endif /* EE_TC_69_ISR && (EE_TC_69_ISR_CAT == 2) */
#if defined(EE_TC_70_ISR) && (EE_TC_70_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_70_ISR)
#endif /* EE_TC_70_ISR && (EE_TC_70_ISR_CAT == 1) */
#if defined(EE_TC_70_ISR) && (EE_TC_70_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_70_ISR))
#endif /* EE_TC_70_ISR && (EE_TC_70_ISR_CAT == 2) */
#if defined(EE_TC_71_ISR) && (EE_TC_71_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_71_ISR)
#endif /* EE_TC_71_ISR && (EE_TC_71_ISR_CAT == 1) */
#if defined(EE_TC_71_ISR) && (EE_TC_71_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_71_ISR))
#endif /* EE_TC_71_ISR && (EE_TC_71_ISR_CAT == 2) */
#if defined(EE_TC_72_ISR) && (EE_TC_72_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_72_ISR)
#endif /* EE_TC_72_ISR && (EE_TC_72_ISR_CAT == 1) */
#if defined(EE_TC_72_ISR) && (EE_TC_72_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_72_ISR))
#endif /* EE_TC_72_ISR && (EE_TC_72_ISR_CAT == 2) */
#if defined(EE_TC_73_ISR) && (EE_TC_73_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_73_ISR)
#endif /* EE_TC_73_ISR && (EE_TC_73_ISR_CAT == 1) */
#if defined(EE_TC_73_ISR) && (EE_TC_73_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_73_ISR))
#endif /* EE_TC_73_ISR && (EE_TC_73_ISR_CAT == 2) */
#if defined(EE_TC_74_ISR) && (EE_TC_74_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_74_ISR)
#endif /* EE_TC_74_ISR && (EE_TC_74_ISR_CAT == 1) */
#if defined(EE_TC_74_ISR) && (EE_TC_74_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_74_ISR))
#endif /* EE_TC_74_ISR && (EE_TC_74_ISR_CAT == 2) */
#if defined(EE_TC_75_ISR) && (EE_TC_75_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_75_ISR)
#endif /* EE_TC_75_ISR && (EE_TC_75_ISR_CAT == 1) */
#if defined(EE_TC_75_ISR) && (EE_TC_75_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_75_ISR))
#endif /* EE_TC_75_ISR && (EE_TC_75_ISR_CAT == 2) */
#if defined(EE_TC_76_ISR) && (EE_TC_76_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_76_ISR)
#endif /* EE_TC_76_ISR && (EE_TC_76_ISR_CAT == 1) */
#if defined(EE_TC_76_ISR) && (EE_TC_76_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_76_ISR))
#endif /* EE_TC_76_ISR && (EE_TC_76_ISR_CAT == 2) */
#if defined(EE_TC_77_ISR) && (EE_TC_77_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_77_ISR)
#endif /* EE_TC_77_ISR && (EE_TC_77_ISR_CAT == 1) */
#if defined(EE_TC_77_ISR) && (EE_TC_77_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_77_ISR))
#endif /* EE_TC_77_ISR && (EE_TC_77_ISR_CAT == 2) */
#if defined(EE_TC_78_ISR) && (EE_TC_78_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_78_ISR)
#endif /* EE_TC_78_ISR && (EE_TC_78_ISR_CAT == 1) */
#if defined(EE_TC_78_ISR) && (EE_TC_78_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_78_ISR))
#endif /* EE_TC_78_ISR && (EE_TC_78_ISR_CAT == 2) */
#if defined(EE_TC_79_ISR) && (EE_TC_79_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_79_ISR)
#endif /* EE_TC_79_ISR && (EE_TC_79_ISR_CAT == 1) */
#if defined(EE_TC_79_ISR) && (EE_TC_79_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_79_ISR))
#endif /* EE_TC_79_ISR && (EE_TC_79_ISR_CAT == 2) */
#if defined(EE_TC_80_ISR) && (EE_TC_80_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_80_ISR)
#endif /* EE_TC_80_ISR && (EE_TC_80_ISR_CAT == 1) */
#if defined(EE_TC_80_ISR) && (EE_TC_80_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_80_ISR))
#endif /* EE_TC_80_ISR && (EE_TC_80_ISR_CAT == 2) */
#if defined(EE_TC_81_ISR) && (EE_TC_81_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_81_ISR)
#endif /* EE_TC_81_ISR && (EE_TC_81_ISR_CAT == 1) */
#if defined(EE_TC_81_ISR) && (EE_TC_81_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_81_ISR))
#endif /* EE_TC_81_ISR && (EE_TC_81_ISR_CAT == 2) */
#if defined(EE_TC_82_ISR) && (EE_TC_82_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_82_ISR)
#endif /* EE_TC_82_ISR && (EE_TC_82_ISR_CAT == 1) */
#if defined(EE_TC_82_ISR) && (EE_TC_82_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_82_ISR))
#endif /* EE_TC_82_ISR && (EE_TC_82_ISR_CAT == 2) */
#if defined(EE_TC_83_ISR) && (EE_TC_83_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_83_ISR)
#endif /* EE_TC_83_ISR && (EE_TC_83_ISR_CAT == 1) */
#if defined(EE_TC_83_ISR) && (EE_TC_83_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_83_ISR))
#endif /* EE_TC_83_ISR && (EE_TC_83_ISR_CAT == 2) */
#if defined(EE_TC_84_ISR) && (EE_TC_84_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_84_ISR)
#endif /* EE_TC_84_ISR && (EE_TC_84_ISR_CAT == 1) */
#if defined(EE_TC_84_ISR) && (EE_TC_84_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_84_ISR))
#endif /* EE_TC_84_ISR && (EE_TC_84_ISR_CAT == 2) */
#if defined(EE_TC_85_ISR) && (EE_TC_85_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_85_ISR)
#endif /* EE_TC_85_ISR && (EE_TC_85_ISR_CAT == 1) */
#if defined(EE_TC_85_ISR) && (EE_TC_85_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_85_ISR))
#endif /* EE_TC_85_ISR && (EE_TC_85_ISR_CAT == 2) */
#if defined(EE_TC_86_ISR) && (EE_TC_86_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_86_ISR)
#endif /* EE_TC_86_ISR && (EE_TC_86_ISR_CAT == 1) */
#if defined(EE_TC_86_ISR) && (EE_TC_86_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_86_ISR))
#endif /* EE_TC_86_ISR && (EE_TC_86_ISR_CAT == 2) */
#if defined(EE_TC_87_ISR) && (EE_TC_87_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_87_ISR)
#endif /* EE_TC_87_ISR && (EE_TC_87_ISR_CAT == 1) */
#if defined(EE_TC_87_ISR) && (EE_TC_87_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_87_ISR))
#endif /* EE_TC_87_ISR && (EE_TC_87_ISR_CAT == 2) */
#if defined(EE_TC_88_ISR) && (EE_TC_88_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_88_ISR)
#endif /* EE_TC_88_ISR && (EE_TC_88_ISR_CAT == 1) */
#if defined(EE_TC_88_ISR) && (EE_TC_88_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_88_ISR))
#endif /* EE_TC_88_ISR && (EE_TC_88_ISR_CAT == 2) */
#if defined(EE_TC_89_ISR) && (EE_TC_89_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_89_ISR)
#endif /* EE_TC_89_ISR && (EE_TC_89_ISR_CAT == 1) */
#if defined(EE_TC_89_ISR) && (EE_TC_89_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_89_ISR))
#endif /* EE_TC_89_ISR && (EE_TC_89_ISR_CAT == 2) */
#if defined(EE_TC_90_ISR) && (EE_TC_90_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_90_ISR)
#endif /* EE_TC_90_ISR && (EE_TC_90_ISR_CAT == 1) */
#if defined(EE_TC_90_ISR) && (EE_TC_90_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_90_ISR))
#endif /* EE_TC_90_ISR && (EE_TC_90_ISR_CAT == 2) */
#if defined(EE_TC_91_ISR) && (EE_TC_91_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_91_ISR)
#endif /* EE_TC_91_ISR && (EE_TC_91_ISR_CAT == 1) */
#if defined(EE_TC_91_ISR) && (EE_TC_91_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_91_ISR))
#endif /* EE_TC_91_ISR && (EE_TC_91_ISR_CAT == 2) */
#if defined(EE_TC_92_ISR) && (EE_TC_92_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_92_ISR)
#endif /* EE_TC_92_ISR && (EE_TC_92_ISR_CAT == 1) */
#if defined(EE_TC_92_ISR) && (EE_TC_92_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_92_ISR))
#endif /* EE_TC_92_ISR && (EE_TC_92_ISR_CAT == 2) */
#if defined(EE_TC_93_ISR) && (EE_TC_93_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_93_ISR)
#endif /* EE_TC_93_ISR && (EE_TC_93_ISR_CAT == 1) */
#if defined(EE_TC_93_ISR) && (EE_TC_93_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_93_ISR))
#endif /* EE_TC_93_ISR && (EE_TC_93_ISR_CAT == 2) */
#if defined(EE_TC_94_ISR) && (EE_TC_94_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_94_ISR)
#endif /* EE_TC_94_ISR && (EE_TC_94_ISR_CAT == 1) */
#if defined(EE_TC_94_ISR) && (EE_TC_94_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_94_ISR))
#endif /* EE_TC_94_ISR && (EE_TC_94_ISR_CAT == 2) */
#if defined(EE_TC_95_ISR) && (EE_TC_95_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_95_ISR)
#endif /* EE_TC_95_ISR && (EE_TC_95_ISR_CAT == 1) */
#if defined(EE_TC_95_ISR) && (EE_TC_95_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_95_ISR))
#endif /* EE_TC_95_ISR && (EE_TC_95_ISR_CAT == 2) */
#if defined(EE_TC_96_ISR) && (EE_TC_96_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_96_ISR)
#endif /* EE_TC_96_ISR && (EE_TC_96_ISR_CAT == 1) */
#if defined(EE_TC_96_ISR) && (EE_TC_96_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_96_ISR))
#endif /* EE_TC_96_ISR && (EE_TC_96_ISR_CAT == 2) */
#if defined(EE_TC_97_ISR) && (EE_TC_97_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_97_ISR)
#endif /* EE_TC_97_ISR && (EE_TC_97_ISR_CAT == 1) */
#if defined(EE_TC_97_ISR) && (EE_TC_97_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_97_ISR))
#endif /* EE_TC_97_ISR && (EE_TC_97_ISR_CAT == 2) */
#if defined(EE_TC_98_ISR) && (EE_TC_98_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_98_ISR)
#endif /* EE_TC_98_ISR && (EE_TC_98_ISR_CAT == 1) */
#if defined(EE_TC_98_ISR) && (EE_TC_98_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_98_ISR))
#endif /* EE_TC_98_ISR && (EE_TC_98_ISR_CAT == 2) */
#if defined(EE_TC_99_ISR) && (EE_TC_99_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_99_ISR)
#endif /* EE_TC_99_ISR && (EE_TC_99_ISR_CAT == 1) */
#if defined(EE_TC_99_ISR) && (EE_TC_99_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_99_ISR))
#endif /* EE_TC_99_ISR && (EE_TC_99_ISR_CAT == 2) */
#if defined(EE_TC_100_ISR) && (EE_TC_100_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_100_ISR)
#endif /* EE_TC_100_ISR && (EE_TC_100_ISR_CAT == 1) */
#if defined(EE_TC_100_ISR) && (EE_TC_100_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_100_ISR))
#endif /* EE_TC_100_ISR && (EE_TC_100_ISR_CAT == 2) */
#if defined(EE_TC_101_ISR) && (EE_TC_101_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_101_ISR)
#endif /* EE_TC_101_ISR && (EE_TC_101_ISR_CAT == 1) */
#if defined(EE_TC_101_ISR) && (EE_TC_101_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_101_ISR))
#endif /* EE_TC_101_ISR && (EE_TC_101_ISR_CAT == 2) */
#if defined(EE_TC_102_ISR) && (EE_TC_102_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_102_ISR)
#endif /* EE_TC_102_ISR && (EE_TC_102_ISR_CAT == 1) */
#if defined(EE_TC_102_ISR) && (EE_TC_102_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_102_ISR))
#endif /* EE_TC_102_ISR && (EE_TC_102_ISR_CAT == 2) */
#if defined(EE_TC_103_ISR) && (EE_TC_103_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_103_ISR)
#endif /* EE_TC_103_ISR && (EE_TC_103_ISR_CAT == 1) */
#if defined(EE_TC_103_ISR) && (EE_TC_103_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_103_ISR))
#endif /* EE_TC_103_ISR && (EE_TC_103_ISR_CAT == 2) */
#if defined(EE_TC_104_ISR) && (EE_TC_104_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_104_ISR)
#endif /* EE_TC_104_ISR && (EE_TC_104_ISR_CAT == 1) */
#if defined(EE_TC_104_ISR) && (EE_TC_104_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_104_ISR))
#endif /* EE_TC_104_ISR && (EE_TC_104_ISR_CAT == 2) */
#if defined(EE_TC_105_ISR) && (EE_TC_105_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_105_ISR)
#endif /* EE_TC_105_ISR && (EE_TC_105_ISR_CAT == 1) */
#if defined(EE_TC_105_ISR) && (EE_TC_105_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_105_ISR))
#endif /* EE_TC_105_ISR && (EE_TC_105_ISR_CAT == 2) */
#if defined(EE_TC_106_ISR) && (EE_TC_106_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_106_ISR)
#endif /* EE_TC_106_ISR && (EE_TC_106_ISR_CAT == 1) */
#if defined(EE_TC_106_ISR) && (EE_TC_106_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_106_ISR))
#endif /* EE_TC_106_ISR && (EE_TC_106_ISR_CAT == 2) */
#if defined(EE_TC_107_ISR) && (EE_TC_107_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_107_ISR)
#endif /* EE_TC_107_ISR && (EE_TC_107_ISR_CAT == 1) */
#if defined(EE_TC_107_ISR) && (EE_TC_107_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_107_ISR))
#endif /* EE_TC_107_ISR && (EE_TC_107_ISR_CAT == 2) */
#if defined(EE_TC_108_ISR) && (EE_TC_108_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_108_ISR)
#endif /* EE_TC_108_ISR && (EE_TC_108_ISR_CAT == 1) */
#if defined(EE_TC_108_ISR) && (EE_TC_108_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_108_ISR))
#endif /* EE_TC_108_ISR && (EE_TC_108_ISR_CAT == 2) */
#if defined(EE_TC_109_ISR) && (EE_TC_109_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_109_ISR)
#endif /* EE_TC_109_ISR && (EE_TC_109_ISR_CAT == 1) */
#if defined(EE_TC_109_ISR) && (EE_TC_109_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_109_ISR))
#endif /* EE_TC_109_ISR && (EE_TC_109_ISR_CAT == 2) */
#if defined(EE_TC_110_ISR) && (EE_TC_110_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_110_ISR)
#endif /* EE_TC_110_ISR && (EE_TC_110_ISR_CAT == 1) */
#if defined(EE_TC_110_ISR) && (EE_TC_110_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_110_ISR))
#endif /* EE_TC_110_ISR && (EE_TC_110_ISR_CAT == 2) */
#if defined(EE_TC_111_ISR) && (EE_TC_111_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_111_ISR)
#endif /* EE_TC_111_ISR && (EE_TC_111_ISR_CAT == 1) */
#if defined(EE_TC_111_ISR) && (EE_TC_111_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_111_ISR))
#endif /* EE_TC_111_ISR && (EE_TC_111_ISR_CAT == 2) */
#if defined(EE_TC_112_ISR) && (EE_TC_112_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_112_ISR)
#endif /* EE_TC_112_ISR && (EE_TC_112_ISR_CAT == 1) */
#if defined(EE_TC_112_ISR) && (EE_TC_112_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_112_ISR))
#endif /* EE_TC_112_ISR && (EE_TC_112_ISR_CAT == 2) */
#if defined(EE_TC_113_ISR) && (EE_TC_113_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_113_ISR)
#endif /* EE_TC_113_ISR && (EE_TC_113_ISR_CAT == 1) */
#if defined(EE_TC_113_ISR) && (EE_TC_113_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_113_ISR))
#endif /* EE_TC_113_ISR && (EE_TC_113_ISR_CAT == 2) */
#if defined(EE_TC_114_ISR) && (EE_TC_114_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_114_ISR)
#endif /* EE_TC_114_ISR && (EE_TC_114_ISR_CAT == 1) */
#if defined(EE_TC_114_ISR) && (EE_TC_114_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_114_ISR))
#endif /* EE_TC_114_ISR && (EE_TC_114_ISR_CAT == 2) */
#if defined(EE_TC_115_ISR) && (EE_TC_115_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_115_ISR)
#endif /* EE_TC_115_ISR && (EE_TC_115_ISR_CAT == 1) */
#if defined(EE_TC_115_ISR) && (EE_TC_115_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_115_ISR))
#endif /* EE_TC_115_ISR && (EE_TC_115_ISR_CAT == 2) */
#if defined(EE_TC_116_ISR) && (EE_TC_116_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_116_ISR)
#endif /* EE_TC_116_ISR && (EE_TC_116_ISR_CAT == 1) */
#if defined(EE_TC_116_ISR) && (EE_TC_116_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_116_ISR))
#endif /* EE_TC_116_ISR && (EE_TC_116_ISR_CAT == 2) */
#if defined(EE_TC_117_ISR) && (EE_TC_117_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_117_ISR)
#endif /* EE_TC_117_ISR && (EE_TC_117_ISR_CAT == 1) */
#if defined(EE_TC_117_ISR) && (EE_TC_117_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_117_ISR))
#endif /* EE_TC_117_ISR && (EE_TC_117_ISR_CAT == 2) */
#if defined(EE_TC_118_ISR) && (EE_TC_118_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_118_ISR)
#endif /* EE_TC_118_ISR && (EE_TC_118_ISR_CAT == 1) */
#if defined(EE_TC_118_ISR) && (EE_TC_118_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_118_ISR))
#endif /* EE_TC_118_ISR && (EE_TC_118_ISR_CAT == 2) */
#if defined(EE_TC_119_ISR) && (EE_TC_119_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_119_ISR)
#endif /* EE_TC_119_ISR && (EE_TC_119_ISR_CAT == 1) */
#if defined(EE_TC_119_ISR) && (EE_TC_119_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_119_ISR))
#endif /* EE_TC_119_ISR && (EE_TC_119_ISR_CAT == 2) */
#if defined(EE_TC_120_ISR) && (EE_TC_120_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_120_ISR)
#endif /* EE_TC_120_ISR && (EE_TC_120_ISR_CAT == 1) */
#if defined(EE_TC_120_ISR) && (EE_TC_120_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_120_ISR))
#endif /* EE_TC_120_ISR && (EE_TC_120_ISR_CAT == 2) */
#if defined(EE_TC_121_ISR) && (EE_TC_121_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_121_ISR)
#endif /* EE_TC_121_ISR && (EE_TC_121_ISR_CAT == 1) */
#if defined(EE_TC_121_ISR) && (EE_TC_121_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_121_ISR))
#endif /* EE_TC_121_ISR && (EE_TC_121_ISR_CAT == 2) */
#if defined(EE_TC_122_ISR) && (EE_TC_122_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_122_ISR)
#endif /* EE_TC_122_ISR && (EE_TC_122_ISR_CAT == 1) */
#if defined(EE_TC_122_ISR) && (EE_TC_122_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_122_ISR))
#endif /* EE_TC_122_ISR && (EE_TC_122_ISR_CAT == 2) */
#if defined(EE_TC_123_ISR) && (EE_TC_123_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_123_ISR)
#endif /* EE_TC_123_ISR && (EE_TC_123_ISR_CAT == 1) */
#if defined(EE_TC_123_ISR) && (EE_TC_123_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_123_ISR))
#endif /* EE_TC_123_ISR && (EE_TC_123_ISR_CAT == 2) */
#if defined(EE_TC_124_ISR) && (EE_TC_124_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_124_ISR)
#endif /* EE_TC_124_ISR && (EE_TC_124_ISR_CAT == 1) */
#if defined(EE_TC_124_ISR) && (EE_TC_124_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_124_ISR))
#endif /* EE_TC_124_ISR && (EE_TC_124_ISR_CAT == 2) */
#if defined(EE_TC_125_ISR) && (EE_TC_125_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_125_ISR)
#endif /* EE_TC_125_ISR && (EE_TC_125_ISR_CAT == 1) */
#if defined(EE_TC_125_ISR) && (EE_TC_125_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_125_ISR))
#endif /* EE_TC_125_ISR && (EE_TC_125_ISR_CAT == 2) */
#if defined(EE_TC_126_ISR) && (EE_TC_126_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_126_ISR)
#endif /* EE_TC_126_ISR && (EE_TC_126_ISR_CAT == 1) */
#if defined(EE_TC_126_ISR) && (EE_TC_126_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_126_ISR))
#endif /* EE_TC_126_ISR && (EE_TC_126_ISR_CAT == 2) */
#if defined(EE_TC_127_ISR) && (EE_TC_127_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_127_ISR)
#endif /* EE_TC_127_ISR && (EE_TC_127_ISR_CAT == 1) */
#if defined(EE_TC_127_ISR) && (EE_TC_127_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_127_ISR))
#endif /* EE_TC_127_ISR && (EE_TC_127_ISR_CAT == 2) */
#if defined(EE_TC_128_ISR) && (EE_TC_128_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_128_ISR)
#endif /* EE_TC_128_ISR && (EE_TC_128_ISR_CAT == 1) */
#if defined(EE_TC_128_ISR) && (EE_TC_128_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_128_ISR))
#endif /* EE_TC_128_ISR && (EE_TC_128_ISR_CAT == 2) */
#if defined(EE_TC_129_ISR) && (EE_TC_129_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_129_ISR)
#endif /* EE_TC_129_ISR && (EE_TC_129_ISR_CAT == 1) */
#if defined(EE_TC_129_ISR) && (EE_TC_129_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_129_ISR))
#endif /* EE_TC_129_ISR && (EE_TC_129_ISR_CAT == 2) */
#if defined(EE_TC_130_ISR) && (EE_TC_130_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_130_ISR)
#endif /* EE_TC_130_ISR && (EE_TC_130_ISR_CAT == 1) */
#if defined(EE_TC_130_ISR) && (EE_TC_130_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_130_ISR))
#endif /* EE_TC_130_ISR && (EE_TC_130_ISR_CAT == 2) */
#if defined(EE_TC_131_ISR) && (EE_TC_131_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_131_ISR)
#endif /* EE_TC_131_ISR && (EE_TC_131_ISR_CAT == 1) */
#if defined(EE_TC_131_ISR) && (EE_TC_131_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_131_ISR))
#endif /* EE_TC_131_ISR && (EE_TC_131_ISR_CAT == 2) */
#if defined(EE_TC_132_ISR) && (EE_TC_132_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_132_ISR)
#endif /* EE_TC_132_ISR && (EE_TC_132_ISR_CAT == 1) */
#if defined(EE_TC_132_ISR) && (EE_TC_132_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_132_ISR))
#endif /* EE_TC_132_ISR && (EE_TC_132_ISR_CAT == 2) */
#if defined(EE_TC_133_ISR) && (EE_TC_133_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_133_ISR)
#endif /* EE_TC_133_ISR && (EE_TC_133_ISR_CAT == 1) */
#if defined(EE_TC_133_ISR) && (EE_TC_133_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_133_ISR))
#endif /* EE_TC_133_ISR && (EE_TC_133_ISR_CAT == 2) */
#if defined(EE_TC_134_ISR) && (EE_TC_134_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_134_ISR)
#endif /* EE_TC_134_ISR && (EE_TC_134_ISR_CAT == 1) */
#if defined(EE_TC_134_ISR) && (EE_TC_134_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_134_ISR))
#endif /* EE_TC_134_ISR && (EE_TC_134_ISR_CAT == 2) */
#if defined(EE_TC_135_ISR) && (EE_TC_135_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_135_ISR)
#endif /* EE_TC_135_ISR && (EE_TC_135_ISR_CAT == 1) */
#if defined(EE_TC_135_ISR) && (EE_TC_135_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_135_ISR))
#endif /* EE_TC_135_ISR && (EE_TC_135_ISR_CAT == 2) */
#if defined(EE_TC_136_ISR) && (EE_TC_136_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_136_ISR)
#endif /* EE_TC_136_ISR && (EE_TC_136_ISR_CAT == 1) */
#if defined(EE_TC_136_ISR) && (EE_TC_136_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_136_ISR))
#endif /* EE_TC_136_ISR && (EE_TC_136_ISR_CAT == 2) */
#if defined(EE_TC_137_ISR) && (EE_TC_137_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_137_ISR)
#endif /* EE_TC_137_ISR && (EE_TC_137_ISR_CAT == 1) */
#if defined(EE_TC_137_ISR) && (EE_TC_137_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_137_ISR))
#endif /* EE_TC_137_ISR && (EE_TC_137_ISR_CAT == 2) */
#if defined(EE_TC_138_ISR) && (EE_TC_138_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_138_ISR)
#endif /* EE_TC_138_ISR && (EE_TC_138_ISR_CAT == 1) */
#if defined(EE_TC_138_ISR) && (EE_TC_138_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_138_ISR))
#endif /* EE_TC_138_ISR && (EE_TC_138_ISR_CAT == 2) */
#if defined(EE_TC_139_ISR) && (EE_TC_139_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_139_ISR)
#endif /* EE_TC_139_ISR && (EE_TC_139_ISR_CAT == 1) */
#if defined(EE_TC_139_ISR) && (EE_TC_139_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_139_ISR))
#endif /* EE_TC_139_ISR && (EE_TC_139_ISR_CAT == 2) */
#if defined(EE_TC_140_ISR) && (EE_TC_140_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_140_ISR)
#endif /* EE_TC_140_ISR && (EE_TC_140_ISR_CAT == 1) */
#if defined(EE_TC_140_ISR) && (EE_TC_140_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_140_ISR))
#endif /* EE_TC_140_ISR && (EE_TC_140_ISR_CAT == 2) */
#if defined(EE_TC_141_ISR) && (EE_TC_141_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_141_ISR)
#endif /* EE_TC_141_ISR && (EE_TC_141_ISR_CAT == 1) */
#if defined(EE_TC_141_ISR) && (EE_TC_141_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_141_ISR))
#endif /* EE_TC_141_ISR && (EE_TC_141_ISR_CAT == 2) */
#if defined(EE_TC_142_ISR) && (EE_TC_142_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_142_ISR)
#endif /* EE_TC_142_ISR && (EE_TC_142_ISR_CAT == 1) */
#if defined(EE_TC_142_ISR) && (EE_TC_142_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_142_ISR))
#endif /* EE_TC_142_ISR && (EE_TC_142_ISR_CAT == 2) */
#if defined(EE_TC_143_ISR) && (EE_TC_143_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_143_ISR)
#endif /* EE_TC_143_ISR && (EE_TC_143_ISR_CAT == 1) */
#if defined(EE_TC_143_ISR) && (EE_TC_143_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_143_ISR))
#endif /* EE_TC_143_ISR && (EE_TC_143_ISR_CAT == 2) */
#if defined(EE_TC_144_ISR) && (EE_TC_144_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_144_ISR)
#endif /* EE_TC_144_ISR && (EE_TC_144_ISR_CAT == 1) */
#if defined(EE_TC_144_ISR) && (EE_TC_144_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_144_ISR))
#endif /* EE_TC_144_ISR && (EE_TC_144_ISR_CAT == 2) */
#if defined(EE_TC_145_ISR) && (EE_TC_145_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_145_ISR)
#endif /* EE_TC_145_ISR && (EE_TC_145_ISR_CAT == 1) */
#if defined(EE_TC_145_ISR) && (EE_TC_145_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_145_ISR))
#endif /* EE_TC_145_ISR && (EE_TC_145_ISR_CAT == 2) */
#if defined(EE_TC_146_ISR) && (EE_TC_146_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_146_ISR)
#endif /* EE_TC_146_ISR && (EE_TC_146_ISR_CAT == 1) */
#if defined(EE_TC_146_ISR) && (EE_TC_146_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_146_ISR))
#endif /* EE_TC_146_ISR && (EE_TC_146_ISR_CAT == 2) */
#if defined(EE_TC_147_ISR) && (EE_TC_147_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_147_ISR)
#endif /* EE_TC_147_ISR && (EE_TC_147_ISR_CAT == 1) */
#if defined(EE_TC_147_ISR) && (EE_TC_147_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_147_ISR))
#endif /* EE_TC_147_ISR && (EE_TC_147_ISR_CAT == 2) */
#if defined(EE_TC_148_ISR) && (EE_TC_148_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_148_ISR)
#endif /* EE_TC_148_ISR && (EE_TC_148_ISR_CAT == 1) */
#if defined(EE_TC_148_ISR) && (EE_TC_148_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_148_ISR))
#endif /* EE_TC_148_ISR && (EE_TC_148_ISR_CAT == 2) */
#if defined(EE_TC_149_ISR) && (EE_TC_149_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_149_ISR)
#endif /* EE_TC_149_ISR && (EE_TC_149_ISR_CAT == 1) */
#if defined(EE_TC_149_ISR) && (EE_TC_149_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_149_ISR))
#endif /* EE_TC_149_ISR && (EE_TC_149_ISR_CAT == 2) */
#if defined(EE_TC_150_ISR) && (EE_TC_150_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_150_ISR)
#endif /* EE_TC_150_ISR && (EE_TC_150_ISR_CAT == 1) */
#if defined(EE_TC_150_ISR) && (EE_TC_150_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_150_ISR))
#endif /* EE_TC_150_ISR && (EE_TC_150_ISR_CAT == 2) */
#if defined(EE_TC_151_ISR) && (EE_TC_151_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_151_ISR)
#endif /* EE_TC_151_ISR && (EE_TC_151_ISR_CAT == 1) */
#if defined(EE_TC_151_ISR) && (EE_TC_151_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_151_ISR))
#endif /* EE_TC_151_ISR && (EE_TC_151_ISR_CAT == 2) */
#if defined(EE_TC_152_ISR) && (EE_TC_152_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_152_ISR)
#endif /* EE_TC_152_ISR && (EE_TC_152_ISR_CAT == 1) */
#if defined(EE_TC_152_ISR) && (EE_TC_152_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_152_ISR))
#endif /* EE_TC_152_ISR && (EE_TC_152_ISR_CAT == 2) */
#if defined(EE_TC_153_ISR) && (EE_TC_153_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_153_ISR)
#endif /* EE_TC_153_ISR && (EE_TC_153_ISR_CAT == 1) */
#if defined(EE_TC_153_ISR) && (EE_TC_153_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_153_ISR))
#endif /* EE_TC_153_ISR && (EE_TC_153_ISR_CAT == 2) */
#if defined(EE_TC_154_ISR) && (EE_TC_154_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_154_ISR)
#endif /* EE_TC_154_ISR && (EE_TC_154_ISR_CAT == 1) */
#if defined(EE_TC_154_ISR) && (EE_TC_154_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_154_ISR))
#endif /* EE_TC_154_ISR && (EE_TC_154_ISR_CAT == 2) */
#if defined(EE_TC_155_ISR) && (EE_TC_155_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_155_ISR)
#endif /* EE_TC_155_ISR && (EE_TC_155_ISR_CAT == 1) */
#if defined(EE_TC_155_ISR) && (EE_TC_155_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_155_ISR))
#endif /* EE_TC_155_ISR && (EE_TC_155_ISR_CAT == 2) */
#if defined(EE_TC_156_ISR) && (EE_TC_156_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_156_ISR)
#endif /* EE_TC_156_ISR && (EE_TC_156_ISR_CAT == 1) */
#if defined(EE_TC_156_ISR) && (EE_TC_156_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_156_ISR))
#endif /* EE_TC_156_ISR && (EE_TC_156_ISR_CAT == 2) */
#if defined(EE_TC_157_ISR) && (EE_TC_157_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_157_ISR)
#endif /* EE_TC_157_ISR && (EE_TC_157_ISR_CAT == 1) */
#if defined(EE_TC_157_ISR) && (EE_TC_157_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_157_ISR))
#endif /* EE_TC_157_ISR && (EE_TC_157_ISR_CAT == 2) */
#if defined(EE_TC_158_ISR) && (EE_TC_158_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_158_ISR)
#endif /* EE_TC_158_ISR && (EE_TC_158_ISR_CAT == 1) */
#if defined(EE_TC_158_ISR) && (EE_TC_158_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_158_ISR))
#endif /* EE_TC_158_ISR && (EE_TC_158_ISR_CAT == 2) */
#if defined(EE_TC_159_ISR) && (EE_TC_159_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_159_ISR)
#endif /* EE_TC_159_ISR && (EE_TC_159_ISR_CAT == 1) */
#if defined(EE_TC_159_ISR) && (EE_TC_159_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_159_ISR))
#endif /* EE_TC_159_ISR && (EE_TC_159_ISR_CAT == 2) */
#if defined(EE_TC_160_ISR) && (EE_TC_160_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_160_ISR)
#endif /* EE_TC_160_ISR && (EE_TC_160_ISR_CAT == 1) */
#if defined(EE_TC_160_ISR) && (EE_TC_160_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_160_ISR))
#endif /* EE_TC_160_ISR && (EE_TC_160_ISR_CAT == 2) */
#if defined(EE_TC_161_ISR) && (EE_TC_161_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_161_ISR)
#endif /* EE_TC_161_ISR && (EE_TC_161_ISR_CAT == 1) */
#if defined(EE_TC_161_ISR) && (EE_TC_161_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_161_ISR))
#endif /* EE_TC_161_ISR && (EE_TC_161_ISR_CAT == 2) */
#if defined(EE_TC_162_ISR) && (EE_TC_162_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_162_ISR)
#endif /* EE_TC_162_ISR && (EE_TC_162_ISR_CAT == 1) */
#if defined(EE_TC_162_ISR) && (EE_TC_162_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_162_ISR))
#endif /* EE_TC_162_ISR && (EE_TC_162_ISR_CAT == 2) */
#if defined(EE_TC_163_ISR) && (EE_TC_163_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_163_ISR)
#endif /* EE_TC_163_ISR && (EE_TC_163_ISR_CAT == 1) */
#if defined(EE_TC_163_ISR) && (EE_TC_163_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_163_ISR))
#endif /* EE_TC_163_ISR && (EE_TC_163_ISR_CAT == 2) */
#if defined(EE_TC_164_ISR) && (EE_TC_164_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_164_ISR)
#endif /* EE_TC_164_ISR && (EE_TC_164_ISR_CAT == 1) */
#if defined(EE_TC_164_ISR) && (EE_TC_164_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_164_ISR))
#endif /* EE_TC_164_ISR && (EE_TC_164_ISR_CAT == 2) */
#if defined(EE_TC_165_ISR) && (EE_TC_165_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_165_ISR)
#endif /* EE_TC_165_ISR && (EE_TC_165_ISR_CAT == 1) */
#if defined(EE_TC_165_ISR) && (EE_TC_165_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_165_ISR))
#endif /* EE_TC_165_ISR && (EE_TC_165_ISR_CAT == 2) */
#if defined(EE_TC_166_ISR) && (EE_TC_166_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_166_ISR)
#endif /* EE_TC_166_ISR && (EE_TC_166_ISR_CAT == 1) */
#if defined(EE_TC_166_ISR) && (EE_TC_166_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_166_ISR))
#endif /* EE_TC_166_ISR && (EE_TC_166_ISR_CAT == 2) */
#if defined(EE_TC_167_ISR) && (EE_TC_167_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_167_ISR)
#endif /* EE_TC_167_ISR && (EE_TC_167_ISR_CAT == 1) */
#if defined(EE_TC_167_ISR) && (EE_TC_167_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_167_ISR))
#endif /* EE_TC_167_ISR && (EE_TC_167_ISR_CAT == 2) */
#if defined(EE_TC_168_ISR) && (EE_TC_168_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_168_ISR)
#endif /* EE_TC_168_ISR && (EE_TC_168_ISR_CAT == 1) */
#if defined(EE_TC_168_ISR) && (EE_TC_168_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_168_ISR))
#endif /* EE_TC_168_ISR && (EE_TC_168_ISR_CAT == 2) */
#if defined(EE_TC_169_ISR) && (EE_TC_169_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_169_ISR)
#endif /* EE_TC_169_ISR && (EE_TC_169_ISR_CAT == 1) */
#if defined(EE_TC_169_ISR) && (EE_TC_169_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_169_ISR))
#endif /* EE_TC_169_ISR && (EE_TC_169_ISR_CAT == 2) */
#if defined(EE_TC_170_ISR) && (EE_TC_170_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_170_ISR)
#endif /* EE_TC_170_ISR && (EE_TC_170_ISR_CAT == 1) */
#if defined(EE_TC_170_ISR) && (EE_TC_170_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_170_ISR))
#endif /* EE_TC_170_ISR && (EE_TC_170_ISR_CAT == 2) */
#if defined(EE_TC_171_ISR) && (EE_TC_171_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_171_ISR)
#endif /* EE_TC_171_ISR && (EE_TC_171_ISR_CAT == 1) */
#if defined(EE_TC_171_ISR) && (EE_TC_171_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_171_ISR))
#endif /* EE_TC_171_ISR && (EE_TC_171_ISR_CAT == 2) */
#if defined(EE_TC_172_ISR) && (EE_TC_172_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_172_ISR)
#endif /* EE_TC_172_ISR && (EE_TC_172_ISR_CAT == 1) */
#if defined(EE_TC_172_ISR) && (EE_TC_172_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_172_ISR))
#endif /* EE_TC_172_ISR && (EE_TC_172_ISR_CAT == 2) */
#if defined(EE_TC_173_ISR) && (EE_TC_173_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_173_ISR)
#endif /* EE_TC_173_ISR && (EE_TC_173_ISR_CAT == 1) */
#if defined(EE_TC_173_ISR) && (EE_TC_173_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_173_ISR))
#endif /* EE_TC_173_ISR && (EE_TC_173_ISR_CAT == 2) */
#if defined(EE_TC_174_ISR) && (EE_TC_174_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_174_ISR)
#endif /* EE_TC_174_ISR && (EE_TC_174_ISR_CAT == 1) */
#if defined(EE_TC_174_ISR) && (EE_TC_174_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_174_ISR))
#endif /* EE_TC_174_ISR && (EE_TC_174_ISR_CAT == 2) */
#if defined(EE_TC_175_ISR) && (EE_TC_175_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_175_ISR)
#endif /* EE_TC_175_ISR && (EE_TC_175_ISR_CAT == 1) */
#if defined(EE_TC_175_ISR) && (EE_TC_175_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_175_ISR))
#endif /* EE_TC_175_ISR && (EE_TC_175_ISR_CAT == 2) */
#if defined(EE_TC_176_ISR) && (EE_TC_176_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_176_ISR)
#endif /* EE_TC_176_ISR && (EE_TC_176_ISR_CAT == 1) */
#if defined(EE_TC_176_ISR) && (EE_TC_176_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_176_ISR))
#endif /* EE_TC_176_ISR && (EE_TC_176_ISR_CAT == 2) */
#if defined(EE_TC_177_ISR) && (EE_TC_177_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_177_ISR)
#endif /* EE_TC_177_ISR && (EE_TC_177_ISR_CAT == 1) */
#if defined(EE_TC_177_ISR) && (EE_TC_177_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_177_ISR))
#endif /* EE_TC_177_ISR && (EE_TC_177_ISR_CAT == 2) */
#if defined(EE_TC_178_ISR) && (EE_TC_178_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_178_ISR)
#endif /* EE_TC_178_ISR && (EE_TC_178_ISR_CAT == 1) */
#if defined(EE_TC_178_ISR) && (EE_TC_178_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_178_ISR))
#endif /* EE_TC_178_ISR && (EE_TC_178_ISR_CAT == 2) */
#if defined(EE_TC_179_ISR) && (EE_TC_179_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_179_ISR)
#endif /* EE_TC_179_ISR && (EE_TC_179_ISR_CAT == 1) */
#if defined(EE_TC_179_ISR) && (EE_TC_179_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_179_ISR))
#endif /* EE_TC_179_ISR && (EE_TC_179_ISR_CAT == 2) */
#if defined(EE_TC_180_ISR) && (EE_TC_180_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_180_ISR)
#endif /* EE_TC_180_ISR && (EE_TC_180_ISR_CAT == 1) */
#if defined(EE_TC_180_ISR) && (EE_TC_180_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_180_ISR))
#endif /* EE_TC_180_ISR && (EE_TC_180_ISR_CAT == 2) */
#if defined(EE_TC_181_ISR) && (EE_TC_181_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_181_ISR)
#endif /* EE_TC_181_ISR && (EE_TC_181_ISR_CAT == 1) */
#if defined(EE_TC_181_ISR) && (EE_TC_181_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_181_ISR))
#endif /* EE_TC_181_ISR && (EE_TC_181_ISR_CAT == 2) */
#if defined(EE_TC_182_ISR) && (EE_TC_182_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_182_ISR)
#endif /* EE_TC_182_ISR && (EE_TC_182_ISR_CAT == 1) */
#if defined(EE_TC_182_ISR) && (EE_TC_182_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_182_ISR))
#endif /* EE_TC_182_ISR && (EE_TC_182_ISR_CAT == 2) */
#if defined(EE_TC_183_ISR) && (EE_TC_183_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_183_ISR)
#endif /* EE_TC_183_ISR && (EE_TC_183_ISR_CAT == 1) */
#if defined(EE_TC_183_ISR) && (EE_TC_183_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_183_ISR))
#endif /* EE_TC_183_ISR && (EE_TC_183_ISR_CAT == 2) */
#if defined(EE_TC_184_ISR) && (EE_TC_184_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_184_ISR)
#endif /* EE_TC_184_ISR && (EE_TC_184_ISR_CAT == 1) */
#if defined(EE_TC_184_ISR) && (EE_TC_184_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_184_ISR))
#endif /* EE_TC_184_ISR && (EE_TC_184_ISR_CAT == 2) */
#if defined(EE_TC_185_ISR) && (EE_TC_185_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_185_ISR)
#endif /* EE_TC_185_ISR && (EE_TC_185_ISR_CAT == 1) */
#if defined(EE_TC_185_ISR) && (EE_TC_185_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_185_ISR))
#endif /* EE_TC_185_ISR && (EE_TC_185_ISR_CAT == 2) */
#if defined(EE_TC_186_ISR) && (EE_TC_186_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_186_ISR)
#endif /* EE_TC_186_ISR && (EE_TC_186_ISR_CAT == 1) */
#if defined(EE_TC_186_ISR) && (EE_TC_186_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_186_ISR))
#endif /* EE_TC_186_ISR && (EE_TC_186_ISR_CAT == 2) */
#if defined(EE_TC_187_ISR) && (EE_TC_187_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_187_ISR)
#endif /* EE_TC_187_ISR && (EE_TC_187_ISR_CAT == 1) */
#if defined(EE_TC_187_ISR) && (EE_TC_187_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_187_ISR))
#endif /* EE_TC_187_ISR && (EE_TC_187_ISR_CAT == 2) */
#if defined(EE_TC_188_ISR) && (EE_TC_188_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_188_ISR)
#endif /* EE_TC_188_ISR && (EE_TC_188_ISR_CAT == 1) */
#if defined(EE_TC_188_ISR) && (EE_TC_188_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_188_ISR))
#endif /* EE_TC_188_ISR && (EE_TC_188_ISR_CAT == 2) */
#if defined(EE_TC_189_ISR) && (EE_TC_189_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_189_ISR)
#endif /* EE_TC_189_ISR && (EE_TC_189_ISR_CAT == 1) */
#if defined(EE_TC_189_ISR) && (EE_TC_189_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_189_ISR))
#endif /* EE_TC_189_ISR && (EE_TC_189_ISR_CAT == 2) */
#if defined(EE_TC_190_ISR) && (EE_TC_190_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_190_ISR)
#endif /* EE_TC_190_ISR && (EE_TC_190_ISR_CAT == 1) */
#if defined(EE_TC_190_ISR) && (EE_TC_190_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_190_ISR))
#endif /* EE_TC_190_ISR && (EE_TC_190_ISR_CAT == 2) */
#if defined(EE_TC_191_ISR) && (EE_TC_191_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_191_ISR)
#endif /* EE_TC_191_ISR && (EE_TC_191_ISR_CAT == 1) */
#if defined(EE_TC_191_ISR) && (EE_TC_191_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_191_ISR))
#endif /* EE_TC_191_ISR && (EE_TC_191_ISR_CAT == 2) */
#if defined(EE_TC_192_ISR) && (EE_TC_192_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_192_ISR)
#endif /* EE_TC_192_ISR && (EE_TC_192_ISR_CAT == 1) */
#if defined(EE_TC_192_ISR) && (EE_TC_192_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_192_ISR))
#endif /* EE_TC_192_ISR && (EE_TC_192_ISR_CAT == 2) */
#if defined(EE_TC_193_ISR) && (EE_TC_193_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_193_ISR)
#endif /* EE_TC_193_ISR && (EE_TC_193_ISR_CAT == 1) */
#if defined(EE_TC_193_ISR) && (EE_TC_193_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_193_ISR))
#endif /* EE_TC_193_ISR && (EE_TC_193_ISR_CAT == 2) */
#if defined(EE_TC_194_ISR) && (EE_TC_194_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_194_ISR)
#endif /* EE_TC_194_ISR && (EE_TC_194_ISR_CAT == 1) */
#if defined(EE_TC_194_ISR) && (EE_TC_194_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_194_ISR))
#endif /* EE_TC_194_ISR && (EE_TC_194_ISR_CAT == 2) */
#if defined(EE_TC_195_ISR) && (EE_TC_195_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_195_ISR)
#endif /* EE_TC_195_ISR && (EE_TC_195_ISR_CAT == 1) */
#if defined(EE_TC_195_ISR) && (EE_TC_195_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_195_ISR))
#endif /* EE_TC_195_ISR && (EE_TC_195_ISR_CAT == 2) */
#if defined(EE_TC_196_ISR) && (EE_TC_196_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_196_ISR)
#endif /* EE_TC_196_ISR && (EE_TC_196_ISR_CAT == 1) */
#if defined(EE_TC_196_ISR) && (EE_TC_196_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_196_ISR))
#endif /* EE_TC_196_ISR && (EE_TC_196_ISR_CAT == 2) */
#if defined(EE_TC_197_ISR) && (EE_TC_197_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_197_ISR)
#endif /* EE_TC_197_ISR && (EE_TC_197_ISR_CAT == 1) */
#if defined(EE_TC_197_ISR) && (EE_TC_197_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_197_ISR))
#endif /* EE_TC_197_ISR && (EE_TC_197_ISR_CAT == 2) */
#if defined(EE_TC_198_ISR) && (EE_TC_198_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_198_ISR)
#endif /* EE_TC_198_ISR && (EE_TC_198_ISR_CAT == 1) */
#if defined(EE_TC_198_ISR) && (EE_TC_198_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_198_ISR))
#endif /* EE_TC_198_ISR && (EE_TC_198_ISR_CAT == 2) */
#if defined(EE_TC_199_ISR) && (EE_TC_199_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_199_ISR)
#endif /* EE_TC_199_ISR && (EE_TC_199_ISR_CAT == 1) */
#if defined(EE_TC_199_ISR) && (EE_TC_199_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_199_ISR))
#endif /* EE_TC_199_ISR && (EE_TC_199_ISR_CAT == 2) */
#if defined(EE_TC_200_ISR) && (EE_TC_200_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_200_ISR)
#endif /* EE_TC_200_ISR && (EE_TC_200_ISR_CAT == 1) */
#if defined(EE_TC_200_ISR) && (EE_TC_200_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_200_ISR))
#endif /* EE_TC_200_ISR && (EE_TC_200_ISR_CAT == 2) */
#if defined(EE_TC_201_ISR) && (EE_TC_201_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_201_ISR)
#endif /* EE_TC_201_ISR && (EE_TC_201_ISR_CAT == 1) */
#if defined(EE_TC_201_ISR) && (EE_TC_201_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_201_ISR))
#endif /* EE_TC_201_ISR && (EE_TC_201_ISR_CAT == 2) */
#if defined(EE_TC_202_ISR) && (EE_TC_202_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_202_ISR)
#endif /* EE_TC_202_ISR && (EE_TC_202_ISR_CAT == 1) */
#if defined(EE_TC_202_ISR) && (EE_TC_202_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_202_ISR))
#endif /* EE_TC_202_ISR && (EE_TC_202_ISR_CAT == 2) */
#if defined(EE_TC_203_ISR) && (EE_TC_203_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_203_ISR)
#endif /* EE_TC_203_ISR && (EE_TC_203_ISR_CAT == 1) */
#if defined(EE_TC_203_ISR) && (EE_TC_203_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_203_ISR))
#endif /* EE_TC_203_ISR && (EE_TC_203_ISR_CAT == 2) */
#if defined(EE_TC_204_ISR) && (EE_TC_204_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_204_ISR)
#endif /* EE_TC_204_ISR && (EE_TC_204_ISR_CAT == 1) */
#if defined(EE_TC_204_ISR) && (EE_TC_204_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_204_ISR))
#endif /* EE_TC_204_ISR && (EE_TC_204_ISR_CAT == 2) */
#if defined(EE_TC_205_ISR) && (EE_TC_205_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_205_ISR)
#endif /* EE_TC_205_ISR && (EE_TC_205_ISR_CAT == 1) */
#if defined(EE_TC_205_ISR) && (EE_TC_205_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_205_ISR))
#endif /* EE_TC_205_ISR && (EE_TC_205_ISR_CAT == 2) */
#if defined(EE_TC_206_ISR) && (EE_TC_206_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_206_ISR)
#endif /* EE_TC_206_ISR && (EE_TC_206_ISR_CAT == 1) */
#if defined(EE_TC_206_ISR) && (EE_TC_206_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_206_ISR))
#endif /* EE_TC_206_ISR && (EE_TC_206_ISR_CAT == 2) */
#if defined(EE_TC_207_ISR) && (EE_TC_207_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_207_ISR)
#endif /* EE_TC_207_ISR && (EE_TC_207_ISR_CAT == 1) */
#if defined(EE_TC_207_ISR) && (EE_TC_207_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_207_ISR))
#endif /* EE_TC_207_ISR && (EE_TC_207_ISR_CAT == 2) */
#if defined(EE_TC_208_ISR) && (EE_TC_208_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_208_ISR)
#endif /* EE_TC_208_ISR && (EE_TC_208_ISR_CAT == 1) */
#if defined(EE_TC_208_ISR) && (EE_TC_208_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_208_ISR))
#endif /* EE_TC_208_ISR && (EE_TC_208_ISR_CAT == 2) */
#if defined(EE_TC_209_ISR) && (EE_TC_209_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_209_ISR)
#endif /* EE_TC_209_ISR && (EE_TC_209_ISR_CAT == 1) */
#if defined(EE_TC_209_ISR) && (EE_TC_209_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_209_ISR))
#endif /* EE_TC_209_ISR && (EE_TC_209_ISR_CAT == 2) */
#if defined(EE_TC_210_ISR) && (EE_TC_210_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_210_ISR)
#endif /* EE_TC_210_ISR && (EE_TC_210_ISR_CAT == 1) */
#if defined(EE_TC_210_ISR) && (EE_TC_210_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_210_ISR))
#endif /* EE_TC_210_ISR && (EE_TC_210_ISR_CAT == 2) */
#if defined(EE_TC_211_ISR) && (EE_TC_211_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_211_ISR)
#endif /* EE_TC_211_ISR && (EE_TC_211_ISR_CAT == 1) */
#if defined(EE_TC_211_ISR) && (EE_TC_211_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_211_ISR))
#endif /* EE_TC_211_ISR && (EE_TC_211_ISR_CAT == 2) */
#if defined(EE_TC_212_ISR) && (EE_TC_212_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_212_ISR)
#endif /* EE_TC_212_ISR && (EE_TC_212_ISR_CAT == 1) */
#if defined(EE_TC_212_ISR) && (EE_TC_212_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_212_ISR))
#endif /* EE_TC_212_ISR && (EE_TC_212_ISR_CAT == 2) */
#if defined(EE_TC_213_ISR) && (EE_TC_213_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_213_ISR)
#endif /* EE_TC_213_ISR && (EE_TC_213_ISR_CAT == 1) */
#if defined(EE_TC_213_ISR) && (EE_TC_213_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_213_ISR))
#endif /* EE_TC_213_ISR && (EE_TC_213_ISR_CAT == 2) */
#if defined(EE_TC_214_ISR) && (EE_TC_214_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_214_ISR)
#endif /* EE_TC_214_ISR && (EE_TC_214_ISR_CAT == 1) */
#if defined(EE_TC_214_ISR) && (EE_TC_214_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_214_ISR))
#endif /* EE_TC_214_ISR && (EE_TC_214_ISR_CAT == 2) */
#if defined(EE_TC_215_ISR) && (EE_TC_215_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_215_ISR)
#endif /* EE_TC_215_ISR && (EE_TC_215_ISR_CAT == 1) */
#if defined(EE_TC_215_ISR) && (EE_TC_215_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_215_ISR))
#endif /* EE_TC_215_ISR && (EE_TC_215_ISR_CAT == 2) */
#if defined(EE_TC_216_ISR) && (EE_TC_216_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_216_ISR)
#endif /* EE_TC_216_ISR && (EE_TC_216_ISR_CAT == 1) */
#if defined(EE_TC_216_ISR) && (EE_TC_216_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_216_ISR))
#endif /* EE_TC_216_ISR && (EE_TC_216_ISR_CAT == 2) */
#if defined(EE_TC_217_ISR) && (EE_TC_217_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_217_ISR)
#endif /* EE_TC_217_ISR && (EE_TC_217_ISR_CAT == 1) */
#if defined(EE_TC_217_ISR) && (EE_TC_217_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_217_ISR))
#endif /* EE_TC_217_ISR && (EE_TC_217_ISR_CAT == 2) */
#if defined(EE_TC_218_ISR) && (EE_TC_218_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_218_ISR)
#endif /* EE_TC_218_ISR && (EE_TC_218_ISR_CAT == 1) */
#if defined(EE_TC_218_ISR) && (EE_TC_218_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_218_ISR))
#endif /* EE_TC_218_ISR && (EE_TC_218_ISR_CAT == 2) */
#if defined(EE_TC_219_ISR) && (EE_TC_219_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_219_ISR)
#endif /* EE_TC_219_ISR && (EE_TC_219_ISR_CAT == 1) */
#if defined(EE_TC_219_ISR) && (EE_TC_219_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_219_ISR))
#endif /* EE_TC_219_ISR && (EE_TC_219_ISR_CAT == 2) */
#if defined(EE_TC_220_ISR) && (EE_TC_220_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_220_ISR)
#endif /* EE_TC_220_ISR && (EE_TC_220_ISR_CAT == 1) */
#if defined(EE_TC_220_ISR) && (EE_TC_220_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_220_ISR))
#endif /* EE_TC_220_ISR && (EE_TC_220_ISR_CAT == 2) */
#if defined(EE_TC_221_ISR) && (EE_TC_221_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_221_ISR)
#endif /* EE_TC_221_ISR && (EE_TC_221_ISR_CAT == 1) */
#if defined(EE_TC_221_ISR) && (EE_TC_221_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_221_ISR))
#endif /* EE_TC_221_ISR && (EE_TC_221_ISR_CAT == 2) */
#if defined(EE_TC_222_ISR) && (EE_TC_222_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_222_ISR)
#endif /* EE_TC_222_ISR && (EE_TC_222_ISR_CAT == 1) */
#if defined(EE_TC_222_ISR) && (EE_TC_222_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_222_ISR))
#endif /* EE_TC_222_ISR && (EE_TC_222_ISR_CAT == 2) */
#if defined(EE_TC_223_ISR) && (EE_TC_223_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_223_ISR)
#endif /* EE_TC_223_ISR && (EE_TC_223_ISR_CAT == 1) */
#if defined(EE_TC_223_ISR) && (EE_TC_223_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_223_ISR))
#endif /* EE_TC_223_ISR && (EE_TC_223_ISR_CAT == 2) */
#if defined(EE_TC_224_ISR) && (EE_TC_224_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_224_ISR)
#endif /* EE_TC_224_ISR && (EE_TC_224_ISR_CAT == 1) */
#if defined(EE_TC_224_ISR) && (EE_TC_224_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_224_ISR))
#endif /* EE_TC_224_ISR && (EE_TC_224_ISR_CAT == 2) */
#if defined(EE_TC_225_ISR) && (EE_TC_225_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_225_ISR)
#endif /* EE_TC_225_ISR && (EE_TC_225_ISR_CAT == 1) */
#if defined(EE_TC_225_ISR) && (EE_TC_225_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_225_ISR))
#endif /* EE_TC_225_ISR && (EE_TC_225_ISR_CAT == 2) */
#if defined(EE_TC_226_ISR) && (EE_TC_226_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_226_ISR)
#endif /* EE_TC_226_ISR && (EE_TC_226_ISR_CAT == 1) */
#if defined(EE_TC_226_ISR) && (EE_TC_226_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_226_ISR))
#endif /* EE_TC_226_ISR && (EE_TC_226_ISR_CAT == 2) */
#if defined(EE_TC_227_ISR) && (EE_TC_227_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_227_ISR)
#endif /* EE_TC_227_ISR && (EE_TC_227_ISR_CAT == 1) */
#if defined(EE_TC_227_ISR) && (EE_TC_227_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_227_ISR))
#endif /* EE_TC_227_ISR && (EE_TC_227_ISR_CAT == 2) */
#if defined(EE_TC_228_ISR) && (EE_TC_228_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_228_ISR)
#endif /* EE_TC_228_ISR && (EE_TC_228_ISR_CAT == 1) */
#if defined(EE_TC_228_ISR) && (EE_TC_228_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_228_ISR))
#endif /* EE_TC_228_ISR && (EE_TC_228_ISR_CAT == 2) */
#if defined(EE_TC_229_ISR) && (EE_TC_229_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_229_ISR)
#endif /* EE_TC_229_ISR && (EE_TC_229_ISR_CAT == 1) */
#if defined(EE_TC_229_ISR) && (EE_TC_229_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_229_ISR))
#endif /* EE_TC_229_ISR && (EE_TC_229_ISR_CAT == 2) */
#if defined(EE_TC_230_ISR) && (EE_TC_230_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_230_ISR)
#endif /* EE_TC_230_ISR && (EE_TC_230_ISR_CAT == 1) */
#if defined(EE_TC_230_ISR) && (EE_TC_230_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_230_ISR))
#endif /* EE_TC_230_ISR && (EE_TC_230_ISR_CAT == 2) */
#if defined(EE_TC_231_ISR) && (EE_TC_231_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_231_ISR)
#endif /* EE_TC_231_ISR && (EE_TC_231_ISR_CAT == 1) */
#if defined(EE_TC_231_ISR) && (EE_TC_231_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_231_ISR))
#endif /* EE_TC_231_ISR && (EE_TC_231_ISR_CAT == 2) */
#if defined(EE_TC_232_ISR) && (EE_TC_232_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_232_ISR)
#endif /* EE_TC_232_ISR && (EE_TC_232_ISR_CAT == 1) */
#if defined(EE_TC_232_ISR) && (EE_TC_232_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_232_ISR))
#endif /* EE_TC_232_ISR && (EE_TC_232_ISR_CAT == 2) */
#if defined(EE_TC_233_ISR) && (EE_TC_233_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_233_ISR)
#endif /* EE_TC_233_ISR && (EE_TC_233_ISR_CAT == 1) */
#if defined(EE_TC_233_ISR) && (EE_TC_233_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_233_ISR))
#endif /* EE_TC_233_ISR && (EE_TC_233_ISR_CAT == 2) */
#if defined(EE_TC_234_ISR) && (EE_TC_234_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_234_ISR)
#endif /* EE_TC_234_ISR && (EE_TC_234_ISR_CAT == 1) */
#if defined(EE_TC_234_ISR) && (EE_TC_234_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_234_ISR))
#endif /* EE_TC_234_ISR && (EE_TC_234_ISR_CAT == 2) */
#if defined(EE_TC_235_ISR) && (EE_TC_235_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_235_ISR)
#endif /* EE_TC_235_ISR && (EE_TC_235_ISR_CAT == 1) */
#if defined(EE_TC_235_ISR) && (EE_TC_235_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_235_ISR))
#endif /* EE_TC_235_ISR && (EE_TC_235_ISR_CAT == 2) */
#if defined(EE_TC_236_ISR) && (EE_TC_236_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_236_ISR)
#endif /* EE_TC_236_ISR && (EE_TC_236_ISR_CAT == 1) */
#if defined(EE_TC_236_ISR) && (EE_TC_236_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_236_ISR))
#endif /* EE_TC_236_ISR && (EE_TC_236_ISR_CAT == 2) */
#if defined(EE_TC_237_ISR) && (EE_TC_237_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_237_ISR)
#endif /* EE_TC_237_ISR && (EE_TC_237_ISR_CAT == 1) */
#if defined(EE_TC_237_ISR) && (EE_TC_237_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_237_ISR))
#endif /* EE_TC_237_ISR && (EE_TC_237_ISR_CAT == 2) */
#if defined(EE_TC_238_ISR) && (EE_TC_238_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_238_ISR)
#endif /* EE_TC_238_ISR && (EE_TC_238_ISR_CAT == 1) */
#if defined(EE_TC_238_ISR) && (EE_TC_238_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_238_ISR))
#endif /* EE_TC_238_ISR && (EE_TC_238_ISR_CAT == 2) */
#if defined(EE_TC_239_ISR) && (EE_TC_239_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_239_ISR)
#endif /* EE_TC_239_ISR && (EE_TC_239_ISR_CAT == 1) */
#if defined(EE_TC_239_ISR) && (EE_TC_239_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_239_ISR))
#endif /* EE_TC_239_ISR && (EE_TC_239_ISR_CAT == 2) */
#if defined(EE_TC_240_ISR) && (EE_TC_240_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_240_ISR)
#endif /* EE_TC_240_ISR && (EE_TC_240_ISR_CAT == 1) */
#if defined(EE_TC_240_ISR) && (EE_TC_240_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_240_ISR))
#endif /* EE_TC_240_ISR && (EE_TC_240_ISR_CAT == 2) */
#if defined(EE_TC_241_ISR) && (EE_TC_241_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_241_ISR)
#endif /* EE_TC_241_ISR && (EE_TC_241_ISR_CAT == 1) */
#if defined(EE_TC_241_ISR) && (EE_TC_241_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_241_ISR))
#endif /* EE_TC_241_ISR && (EE_TC_241_ISR_CAT == 2) */
#if defined(EE_TC_242_ISR) && (EE_TC_242_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_242_ISR)
#endif /* EE_TC_242_ISR && (EE_TC_242_ISR_CAT == 1) */
#if defined(EE_TC_242_ISR) && (EE_TC_242_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_242_ISR))
#endif /* EE_TC_242_ISR && (EE_TC_242_ISR_CAT == 2) */
#if defined(EE_TC_243_ISR) && (EE_TC_243_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_243_ISR)
#endif /* EE_TC_243_ISR && (EE_TC_243_ISR_CAT == 1) */
#if defined(EE_TC_243_ISR) && (EE_TC_243_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_243_ISR))
#endif /* EE_TC_243_ISR && (EE_TC_243_ISR_CAT == 2) */
#if defined(EE_TC_244_ISR) && (EE_TC_244_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_244_ISR)
#endif /* EE_TC_244_ISR && (EE_TC_244_ISR_CAT == 1) */
#if defined(EE_TC_244_ISR) && (EE_TC_244_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_244_ISR))
#endif /* EE_TC_244_ISR && (EE_TC_244_ISR_CAT == 2) */
#if defined(EE_TC_245_ISR) && (EE_TC_245_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_245_ISR)
#endif /* EE_TC_245_ISR && (EE_TC_245_ISR_CAT == 1) */
#if defined(EE_TC_245_ISR) && (EE_TC_245_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_245_ISR))
#endif /* EE_TC_245_ISR && (EE_TC_245_ISR_CAT == 2) */
#if defined(EE_TC_246_ISR) && (EE_TC_246_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_246_ISR)
#endif /* EE_TC_246_ISR && (EE_TC_246_ISR_CAT == 1) */
#if defined(EE_TC_246_ISR) && (EE_TC_246_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_246_ISR))
#endif /* EE_TC_246_ISR && (EE_TC_246_ISR_CAT == 2) */
#if defined(EE_TC_247_ISR) && (EE_TC_247_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_247_ISR)
#endif /* EE_TC_247_ISR && (EE_TC_247_ISR_CAT == 1) */
#if defined(EE_TC_247_ISR) && (EE_TC_247_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_247_ISR))
#endif /* EE_TC_247_ISR && (EE_TC_247_ISR_CAT == 2) */
#if defined(EE_TC_248_ISR) && (EE_TC_248_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_248_ISR)
#endif /* EE_TC_248_ISR && (EE_TC_248_ISR_CAT == 1) */
#if defined(EE_TC_248_ISR) && (EE_TC_248_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_248_ISR))
#endif /* EE_TC_248_ISR && (EE_TC_248_ISR_CAT == 2) */
#if defined(EE_TC_249_ISR) && (EE_TC_249_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_249_ISR)
#endif /* EE_TC_249_ISR && (EE_TC_249_ISR_CAT == 1) */
#if defined(EE_TC_249_ISR) && (EE_TC_249_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_249_ISR))
#endif /* EE_TC_249_ISR && (EE_TC_249_ISR_CAT == 2) */
#if defined(EE_TC_250_ISR) && (EE_TC_250_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_250_ISR)
#endif /* EE_TC_250_ISR && (EE_TC_250_ISR_CAT == 1) */
#if defined(EE_TC_250_ISR) && (EE_TC_250_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_250_ISR))
#endif /* EE_TC_250_ISR && (EE_TC_250_ISR_CAT == 2) */
#if defined(EE_TC_251_ISR) && (EE_TC_251_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_251_ISR)
#endif /* EE_TC_251_ISR && (EE_TC_251_ISR_CAT == 1) */
#if defined(EE_TC_251_ISR) && (EE_TC_251_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_251_ISR))
#endif /* EE_TC_251_ISR && (EE_TC_251_ISR_CAT == 2) */
#if defined(EE_TC_252_ISR) && (EE_TC_252_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_252_ISR)
#endif /* EE_TC_252_ISR && (EE_TC_252_ISR_CAT == 1) */
#if defined(EE_TC_252_ISR) && (EE_TC_252_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_252_ISR))
#endif /* EE_TC_252_ISR && (EE_TC_252_ISR_CAT == 2) */
#if defined(EE_TC_253_ISR) && (EE_TC_253_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_253_ISR)
#endif /* EE_TC_253_ISR && (EE_TC_253_ISR_CAT == 1) */
#if defined(EE_TC_253_ISR) && (EE_TC_253_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_253_ISR))
#endif /* EE_TC_253_ISR && (EE_TC_253_ISR_CAT == 2) */
#if defined(EE_TC_254_ISR) && (EE_TC_254_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_254_ISR)
#endif /* EE_TC_254_ISR && (EE_TC_254_ISR_CAT == 1) */
#if defined(EE_TC_254_ISR) && (EE_TC_254_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_254_ISR))
#endif /* EE_TC_254_ISR && (EE_TC_254_ISR_CAT == 2) */
#if defined(EE_TC_255_ISR) && (EE_TC_255_ISR_CAT == 1)
EE_COMPILER_EXTERN(EE_TC_255_ISR)
#endif /* EE_TC_255_ISR && (EE_TC_255_ISR_CAT == 1) */
#if defined(EE_TC_255_ISR) && (EE_TC_255_ISR_CAT == 2)
EE_COMPILER_EXTERN(EE_TC_ISR2_WRAP(EE_TC_255_ISR))
#endif /* EE_TC_255_ISR && (EE_TC_255_ISR_CAT == 2) */

#endif /* !EE_ERIKA_ISR_HANDLING_OFF && __TASKING__ */

#endif /* INCLUDE_EE_TC_IRQ_H__ */
