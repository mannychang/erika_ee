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

/**
    @file ee_tc_intvec.c
    @brief Interrupts vector implementation.
    @author Errico Guidieri
    @date 2012
 */

/* Infinion modifications, intigration to Infineon Build environment:
 * Updates for compilation issues with Dcc Compiler:
 * Author: 
 *         Ashok Abbi, <Ashok.Abbi@infineon.com> 18.07.2013
 */
 
#include "ee_internal.h"

#if defined(EE_MAX_ISR2) && (EE_MAX_ISR2 > 0)
/* Header with definition for ISR2 Wrappers generation */
#include "cpu/tricore/inc/ee_tc_irq_internal.h"

/* If MemMap.h support is enabled (i.e. because memory protection): use it */
#ifdef EE_SUPPORT_MEMMAP_H
#define OS_START_SEC_CODE
#include "MemMap.h"
#endif /* EE_SUPPORT_MEMMAP_H */
/* ISR2 Global Handler Declaration */
void EE_tc_isr2_global_wrapper(EE_tc_ISR_handler f);
/* If MemMap.h support is enabled (i.e. because memory protection): use it */
#ifdef EE_SUPPORT_MEMMAP_H
#define OS_STOP_SEC_CODE
#include "MemMap.h"
#endif /* EE_SUPPORT_MEMMAP_H */
#else
/* In any case I need the Header with definitions for ISRs */
#include "cpu/tricore/inc/ee_tc_irq.h"
#endif /* EE_MAX_ISR2 > 0 */

#ifdef __TASKING__

/* With tasking I can generate the Interrupt vector and the
   ISR2 wrappers in one step */
#define EE_ISR2_DEFINITION(f,p)                                       \
/* Declare The ISR User handler */                                    \
__asm volatile(".extern " EE_PREPROC_STRING(f));                      \
/* ISR2 User Handler Wrapper */                                       \
void __interrupt_fast(p) EE_VECTOR_TABLE                              \
  EE_COMPILER_EXPORT EE_PREPROC_JOIN(ISR2_,f) ( void )                \
{                                                                     \
  EE_tc_svlcx();                                                      \
  __asm volatile ("movh.a a4, #@his(" EE_PREPROC_STRING(f) ")");      \
  __asm volatile ("lea a4, [a4]@los(" EE_PREPROC_STRING(f) ")");      \
  __asm volatile ("j " EE_PREPROC_STRING(EE_tc_isr2_global_wrapper)); \
}

/* Define an ISR (category 1)
   In TriCore Architecture ISR ID and ISR Priority are the same.
   I Use ISR ID as interrupt vector entry because is tied to handler name
   and I can easily reconstruct that here */
#define EE_ISR1_DEFINITION(f,p)                         \
/* Declare The ISR User handler */                      \
extern void f ( void );                                 \
void __interrupt_fast(p) __bisr_(p) EE_VECTOR_TABLE     \
EE_COMPILER_EXPORT EE_PREPROC_JOIN(ISR1_,f) ( void )    \
{                                                       \
  f();                                                  \
}

/* I don't need to emit asm directives for alignment for TASKING */
#define EE_ISR_ENTRY_ALIGN(p)

#elif defined(__GNUC__)

#define EE_ISR1_ENTRY_DEFINITION(f,p)                       \
  __asm (".globl EE_tc_isr1_entry_" EE_PREPROC_STRING(p));  \
  __asm ("EE_tc_isr1_entry_" EE_PREPROC_STRING(p) ":");     \
  __asm ("bisr " EE_PREPROC_STRING(p));                     \
  __asm ("movh.a %a15,hi:" EE_PREPROC_STRING(f));           \
  __asm ("lea %a15,[%a15]lo:" EE_PREPROC_STRING(f));        \
  __asm ("calli %a15");                                     \
  __asm ("rslcx");                                          \
  __asm ("rfe");                                            \
  __asm (".align 5");

/* This macro do not use "bisr" instruction (Begin ISR = Save LC + enable)
   because we need to increment nesting level in critical section.
   The code to do that is implemented in CPU common layer.
   So we prefer to reuse that code respect to save one instruction due
   the split of bisr in svlc + enable */
#define EE_ISR2_ENTRY_DEFINITION(f,p)                       \
  __asm (".globl EE_tc_isr2_entry_" EE_PREPROC_STRING(p));  \
  __asm ("EE_tc_isr2_entry_" EE_PREPROC_STRING(p) ":");     \
  __asm ("svlcx");                                          \
  __asm ("movh.a %a4,hi:" EE_PREPROC_STRING(f));            \
  __asm ("lea %a4,[%a4]lo:" EE_PREPROC_STRING(f));          \
  __asm ("j EE_tc_isr2_global_wrapper");                    \
  __asm (".align 5");

#define EE_ISR2_DEFINITION(f,p) EE_ISR2_ENTRY_DEFINITION(f,p)

#define EE_ISR1_DEFINITION(f,p) EE_ISR1_ENTRY_DEFINITION(f,p)

/*  I would have loved to use .org assembly directive (or .= instruction)
    instead generate dummy entries, but assembler ignore two .org consecutive
    directives (or two .= or any combination of the two) */
#define EE_ISR_ENTRY_ALIGN2(p)                                  \
  __asm (".globl EE_tc_isr_dummy_entry_" EE_PREPROC_STRING(p)); \
  __asm ("EE_tc_isr_dummy_entry_" EE_PREPROC_STRING(p) ":");    \
  __asm ("nop");                                                \
  __asm (".align  5");                                          \

/* Two pre-processor steps to let macros explode */
#define EE_ISR_ENTRY_ALIGN(p) EE_ISR_ENTRY_ALIGN2(p)

__asm ("                                \n\
  .section .inttab, \"ax\", @progbits   \n\
  .align 8                              \n\
  .globl EE_tc_interrupt_table          \n\
EE_tc_interrupt_table:                  \n\
  .skip 0x20                            \n\
");
#elif defined(__DCC__)

/* With tasking I can generate the Interrupt vector and the
   ISR2 wrappers in one step */
#define EE_ISR2_ENTRY_DEFINITION(f,p)                       \
/* Declare The ISR User handler */                          \
extern void f ( void );                                     \
/* ISR2 Vector Entry */                                     \
__asm ("  .globl EE_tc_isr2_entry_" EE_PREPROC_STRING(p));  \
__asm ("EE_tc_isr2_entry_" EE_PREPROC_STRING(p) ":");       \
__asm ("  svlcx");                                          \
__asm ("  movh.a %a4," EE_PREPROC_STRING(f) "@ha");         \
__asm ("  lea %a4,[%a4]" EE_PREPROC_STRING(f) "@l");        \
__asm ("  j EE_tc_isr2_global_wrapper");                    \
__asm ("  .align 5");
/* Two pre-processor steps to let macros explode */
#define EE_ISR2_DEFINITION(f,p) EE_ISR2_ENTRY_DEFINITION(f,p)

/* Define an ISR (category 1)
   In TriCore Architecture ISR ID and ISR Priority are the same.
   I Use ISR ID as interrupt vector entry because is tied to handler name
   and I can easily reconstruct that here */
#define EE_ISR1_ENTRY_DEFINITION(f,p)                       \
/* Declare The ISR User handler */                          \
extern void f ( void );                                     \
/* ISR1 Vector Entry */                                     \
__asm ("  .globl EE_tc_isr1_entry_" EE_PREPROC_STRING(p));  \
__asm ("EE_tc_isr1_entry_" EE_PREPROC_STRING(p) ":");       \
__asm ("  bisr " EE_PREPROC_STRING(p));                     \
__asm ("  movh.a %a15," EE_PREPROC_STRING(f) "@ha");        \
__asm ("  lea %a15,[%a15]" EE_PREPROC_STRING(f) "@l");      \
__asm ("  calli %a15");                                     \
__asm ("  rslcx");                                          \
__asm ("  rfe");                                            \
__asm ("  .align 5");
/* Two pre-processor steps to let macros explode */
#define EE_ISR1_DEFINITION(f,p) EE_ISR1_ENTRY_DEFINITION(f,p)

/* To Align the ISR vector */
#define EE_ISR_ENTRY_ALIGN2(p)                                  \
/* To Align the ISR vector */                                   \
__asm ("  .globl EE_tc_isr_dummy_entry_" EE_PREPROC_STRING(p)); \
__asm ("EE_tc_isr_dummy_entry_" EE_PREPROC_STRING(p) ":");      \
__asm ("  .org . + 32");
/* Two pre-processor steps to let macros explode */
#define EE_ISR_ENTRY_ALIGN(p) EE_ISR_ENTRY_ALIGN2(p)

/* Declare symbol for ISR Vector Table */
__asm ("                                \n\
  .section .inttab,4,rx                 \n\
  .set noreorder                        \n\
  .align 8                              \n\
  .globl EE_tc_interrupt_table          \n\
EE_tc_interrupt_table:                  \n\
  .skip 0x20                            \n\
");
#else
#error Unsupported compiler!
#endif /* __TASKING__ || __GNUC__ || __DCC__ */

/* Erika Interrupt Vector Definition */
#if defined(EE_TC_1_ISR) && (EE_TC_1_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_1_ISR, EE_TC_1_ISR_PRI)
#elif defined(EE_TC_1_ISR) && (EE_TC_1_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_1_ISR, EE_TC_1_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 1)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_1)
#endif /* EE_TC_1_ISR */
#if defined(EE_TC_2_ISR) && (EE_TC_2_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_2_ISR, EE_TC_2_ISR_PRI)
#elif defined(EE_TC_2_ISR) && (EE_TC_2_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_2_ISR, EE_TC_2_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 2)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_2)
#endif /* EE_TC_2_ISR */
#if defined(EE_TC_3_ISR) && (EE_TC_3_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_3_ISR, EE_TC_3_ISR_PRI)
#elif defined(EE_TC_3_ISR) && (EE_TC_3_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_3_ISR, EE_TC_3_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 3)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_3)
#endif /* EE_TC_3_ISR */
#if defined(EE_TC_4_ISR) && (EE_TC_4_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_4_ISR, EE_TC_4_ISR_PRI)
#elif defined(EE_TC_4_ISR) && (EE_TC_4_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_4_ISR, EE_TC_4_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 4)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_4)
#endif /* EE_TC_4_ISR */
#if defined(EE_TC_5_ISR) && (EE_TC_5_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_5_ISR, EE_TC_5_ISR_PRI)
#elif defined(EE_TC_5_ISR) && (EE_TC_5_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_5_ISR, EE_TC_5_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 5)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_5)
#endif /* EE_TC_5_ISR */
#if defined(EE_TC_6_ISR) && (EE_TC_6_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_6_ISR, EE_TC_6_ISR_PRI)
#elif defined(EE_TC_6_ISR) && (EE_TC_6_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_6_ISR, EE_TC_6_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 6)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_6)
#endif /* EE_TC_6_ISR */
#if defined(EE_TC_7_ISR) && (EE_TC_7_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_7_ISR, EE_TC_7_ISR_PRI)
#elif defined(EE_TC_7_ISR) && (EE_TC_7_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_7_ISR, EE_TC_7_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 7)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_7)
#endif /* EE_TC_7_ISR */
#if defined(EE_TC_8_ISR) && (EE_TC_8_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_8_ISR, EE_TC_8_ISR_PRI)
#elif defined(EE_TC_8_ISR) && (EE_TC_8_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_8_ISR, EE_TC_8_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 8)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_8)
#endif /* EE_TC_8_ISR */
#if defined(EE_TC_9_ISR) && (EE_TC_9_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_9_ISR, EE_TC_9_ISR_PRI)
#elif defined(EE_TC_9_ISR) && (EE_TC_9_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_9_ISR, EE_TC_9_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 9)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_9)
#endif /* EE_TC_9_ISR */
#if defined(EE_TC_10_ISR) && (EE_TC_10_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_10_ISR, EE_TC_10_ISR_PRI)
#elif defined(EE_TC_10_ISR) && (EE_TC_10_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_10_ISR, EE_TC_10_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 10)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_10)
#endif /* EE_TC_10_ISR */
#if defined(EE_TC_11_ISR) && (EE_TC_11_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_11_ISR, EE_TC_11_ISR_PRI)
#elif defined(EE_TC_11_ISR) && (EE_TC_11_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_11_ISR, EE_TC_11_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 11)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_11)
#endif /* EE_TC_11_ISR */
#if defined(EE_TC_12_ISR) && (EE_TC_12_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_12_ISR, EE_TC_12_ISR_PRI)
#elif defined(EE_TC_12_ISR) && (EE_TC_12_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_12_ISR, EE_TC_12_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 12)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_12)
#endif /* EE_TC_12_ISR */
#if defined(EE_TC_13_ISR) && (EE_TC_13_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_13_ISR, EE_TC_13_ISR_PRI)
#elif defined(EE_TC_13_ISR) && (EE_TC_13_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_13_ISR, EE_TC_13_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 13)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_13)
#endif /* EE_TC_13_ISR */
#if defined(EE_TC_14_ISR) && (EE_TC_14_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_14_ISR, EE_TC_14_ISR_PRI)
#elif defined(EE_TC_14_ISR) && (EE_TC_14_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_14_ISR, EE_TC_14_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 14)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_14)
#endif /* EE_TC_14_ISR */
#if defined(EE_TC_15_ISR) && (EE_TC_15_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_15_ISR, EE_TC_15_ISR_PRI)
#elif defined(EE_TC_15_ISR) && (EE_TC_15_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_15_ISR, EE_TC_15_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 15)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_15)
#endif /* EE_TC_15_ISR */
#if defined(EE_TC_16_ISR) && (EE_TC_16_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_16_ISR, EE_TC_16_ISR_PRI)
#elif defined(EE_TC_16_ISR) && (EE_TC_16_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_16_ISR, EE_TC_16_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 16)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_16)
#endif /* EE_TC_16_ISR */
#if defined(EE_TC_17_ISR) && (EE_TC_17_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_17_ISR, EE_TC_17_ISR_PRI)
#elif defined(EE_TC_17_ISR) && (EE_TC_17_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_17_ISR, EE_TC_17_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 17)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_17)
#endif /* EE_TC_17_ISR */
#if defined(EE_TC_18_ISR) && (EE_TC_18_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_18_ISR, EE_TC_18_ISR_PRI)
#elif defined(EE_TC_18_ISR) && (EE_TC_18_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_18_ISR, EE_TC_18_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 18)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_18)
#endif /* EE_TC_18_ISR */
#if defined(EE_TC_19_ISR) && (EE_TC_19_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_19_ISR, EE_TC_19_ISR_PRI)
#elif defined(EE_TC_19_ISR) && (EE_TC_19_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_19_ISR, EE_TC_19_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 19)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_19)
#endif /* EE_TC_19_ISR */
#if defined(EE_TC_20_ISR) && (EE_TC_20_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_20_ISR, EE_TC_20_ISR_PRI)
#elif defined(EE_TC_20_ISR) && (EE_TC_20_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_20_ISR, EE_TC_20_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 20)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_20)
#endif /* EE_TC_20_ISR */
#if defined(EE_TC_21_ISR) && (EE_TC_21_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_21_ISR, EE_TC_21_ISR_PRI)
#elif defined(EE_TC_21_ISR) && (EE_TC_21_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_21_ISR, EE_TC_21_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 21)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_21)
#endif /* EE_TC_21_ISR */
#if defined(EE_TC_22_ISR) && (EE_TC_22_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_22_ISR, EE_TC_22_ISR_PRI)
#elif defined(EE_TC_22_ISR) && (EE_TC_22_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_22_ISR, EE_TC_22_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 22)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_22)
#endif /* EE_TC_22_ISR */
#if defined(EE_TC_23_ISR) && (EE_TC_23_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_23_ISR, EE_TC_23_ISR_PRI)
#elif defined(EE_TC_23_ISR) && (EE_TC_23_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_23_ISR, EE_TC_23_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 23)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_23)
#endif /* EE_TC_23_ISR */
#if defined(EE_TC_24_ISR) && (EE_TC_24_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_24_ISR, EE_TC_24_ISR_PRI)
#elif defined(EE_TC_24_ISR) && (EE_TC_24_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_24_ISR, EE_TC_24_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 24)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_24)
#endif /* EE_TC_24_ISR */
#if defined(EE_TC_25_ISR) && (EE_TC_25_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_25_ISR, EE_TC_25_ISR_PRI)
#elif defined(EE_TC_25_ISR) && (EE_TC_25_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_25_ISR, EE_TC_25_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 25)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_25)
#endif /* EE_TC_25_ISR */
#if defined(EE_TC_26_ISR) && (EE_TC_26_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_26_ISR, EE_TC_26_ISR_PRI)
#elif defined(EE_TC_26_ISR) && (EE_TC_26_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_26_ISR, EE_TC_26_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 26)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_26)
#endif /* EE_TC_26_ISR */
#if defined(EE_TC_27_ISR) && (EE_TC_27_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_27_ISR, EE_TC_27_ISR_PRI)
#elif defined(EE_TC_27_ISR) && (EE_TC_27_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_27_ISR, EE_TC_27_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 27)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_27)
#endif /* EE_TC_27_ISR */
#if defined(EE_TC_28_ISR) && (EE_TC_28_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_28_ISR, EE_TC_28_ISR_PRI)
#elif defined(EE_TC_28_ISR) && (EE_TC_28_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_28_ISR, EE_TC_28_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 28)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_28)
#endif /* EE_TC_28_ISR */
#if defined(EE_TC_29_ISR) && (EE_TC_29_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_29_ISR, EE_TC_29_ISR_PRI)
#elif defined(EE_TC_29_ISR) && (EE_TC_29_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_29_ISR, EE_TC_29_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 29)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_29)
#endif /* EE_TC_29_ISR */
#if defined(EE_TC_30_ISR) && (EE_TC_30_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_30_ISR, EE_TC_30_ISR_PRI)
#elif defined(EE_TC_30_ISR) && (EE_TC_30_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_30_ISR, EE_TC_30_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 30)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_30)
#endif /* EE_TC_30_ISR */
#if defined(EE_TC_31_ISR) && (EE_TC_31_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_31_ISR, EE_TC_31_ISR_PRI)
#elif defined(EE_TC_31_ISR) && (EE_TC_31_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_31_ISR, EE_TC_31_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 31)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_31)
#endif /* EE_TC_31_ISR */
#if defined(EE_TC_32_ISR) && (EE_TC_32_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_32_ISR, EE_TC_32_ISR_PRI)
#elif defined(EE_TC_32_ISR) && (EE_TC_32_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_32_ISR, EE_TC_32_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 32)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_32)
#endif /* EE_TC_32_ISR */
#if defined(EE_TC_33_ISR) && (EE_TC_33_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_33_ISR, EE_TC_33_ISR_PRI)
#elif defined(EE_TC_33_ISR) && (EE_TC_33_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_33_ISR, EE_TC_33_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 33)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_33)
#endif /* EE_TC_33_ISR */
#if defined(EE_TC_34_ISR) && (EE_TC_34_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_34_ISR, EE_TC_34_ISR_PRI)
#elif defined(EE_TC_34_ISR) && (EE_TC_34_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_34_ISR, EE_TC_34_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 34)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_34)
#endif /* EE_TC_34_ISR */
#if defined(EE_TC_35_ISR) && (EE_TC_35_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_35_ISR, EE_TC_35_ISR_PRI)
#elif defined(EE_TC_35_ISR) && (EE_TC_35_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_35_ISR, EE_TC_35_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 35)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_35)
#endif /* EE_TC_35_ISR */
#if defined(EE_TC_36_ISR) && (EE_TC_36_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_36_ISR, EE_TC_36_ISR_PRI)
#elif defined(EE_TC_36_ISR) && (EE_TC_36_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_36_ISR, EE_TC_36_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 36)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_36)
#endif /* EE_TC_36_ISR */
#if defined(EE_TC_37_ISR) && (EE_TC_37_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_37_ISR, EE_TC_37_ISR_PRI)
#elif defined(EE_TC_37_ISR) && (EE_TC_37_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_37_ISR, EE_TC_37_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 37)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_37)
#endif /* EE_TC_37_ISR */
#if defined(EE_TC_38_ISR) && (EE_TC_38_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_38_ISR, EE_TC_38_ISR_PRI)
#elif defined(EE_TC_38_ISR) && (EE_TC_38_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_38_ISR, EE_TC_38_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 38)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_38)
#endif /* EE_TC_38_ISR */
#if defined(EE_TC_39_ISR) && (EE_TC_39_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_39_ISR, EE_TC_39_ISR_PRI)
#elif defined(EE_TC_39_ISR) && (EE_TC_39_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_39_ISR, EE_TC_39_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 39)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_39)
#endif /* EE_TC_39_ISR */
#if defined(EE_TC_40_ISR) && (EE_TC_40_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_40_ISR, EE_TC_40_ISR_PRI)
#elif defined(EE_TC_40_ISR) && (EE_TC_40_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_40_ISR, EE_TC_40_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 40)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_40)
#endif /* EE_TC_40_ISR */
#if defined(EE_TC_41_ISR) && (EE_TC_41_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_41_ISR, EE_TC_41_ISR_PRI)
#elif defined(EE_TC_41_ISR) && (EE_TC_41_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_41_ISR, EE_TC_41_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 41)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_41)
#endif /* EE_TC_41_ISR */
#if defined(EE_TC_42_ISR) && (EE_TC_42_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_42_ISR, EE_TC_42_ISR_PRI)
#elif defined(EE_TC_42_ISR) && (EE_TC_42_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_42_ISR, EE_TC_42_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 42)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_42)
#endif /* EE_TC_42_ISR */
#if defined(EE_TC_43_ISR) && (EE_TC_43_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_43_ISR, EE_TC_43_ISR_PRI)
#elif defined(EE_TC_43_ISR) && (EE_TC_43_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_43_ISR, EE_TC_43_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 43)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_43)
#endif /* EE_TC_43_ISR */
#if defined(EE_TC_44_ISR) && (EE_TC_44_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_44_ISR, EE_TC_44_ISR_PRI)
#elif defined(EE_TC_44_ISR) && (EE_TC_44_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_44_ISR, EE_TC_44_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 44)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_44)
#endif /* EE_TC_44_ISR */
#if defined(EE_TC_45_ISR) && (EE_TC_45_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_45_ISR, EE_TC_45_ISR_PRI)
#elif defined(EE_TC_45_ISR) && (EE_TC_45_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_45_ISR, EE_TC_45_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 45)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_45)
#endif /* EE_TC_45_ISR */
#if defined(EE_TC_46_ISR) && (EE_TC_46_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_46_ISR, EE_TC_46_ISR_PRI)
#elif defined(EE_TC_46_ISR) && (EE_TC_46_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_46_ISR, EE_TC_46_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 46)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_46)
#endif /* EE_TC_46_ISR */
#if defined(EE_TC_47_ISR) && (EE_TC_47_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_47_ISR, EE_TC_47_ISR_PRI)
#elif defined(EE_TC_47_ISR) && (EE_TC_47_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_47_ISR, EE_TC_47_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 47)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_47)
#endif /* EE_TC_47_ISR */
#if defined(EE_TC_48_ISR) && (EE_TC_48_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_48_ISR, EE_TC_48_ISR_PRI)
#elif defined(EE_TC_48_ISR) && (EE_TC_48_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_48_ISR, EE_TC_48_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 48)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_48)
#endif /* EE_TC_48_ISR */
#if defined(EE_TC_49_ISR) && (EE_TC_49_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_49_ISR, EE_TC_49_ISR_PRI)
#elif defined(EE_TC_49_ISR) && (EE_TC_49_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_49_ISR, EE_TC_49_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 49)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_49)
#endif /* EE_TC_49_ISR */
#if defined(EE_TC_50_ISR) && (EE_TC_50_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_50_ISR, EE_TC_50_ISR_PRI)
#elif defined(EE_TC_50_ISR) && (EE_TC_50_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_50_ISR, EE_TC_50_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 50)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_50)
#endif /* EE_TC_50_ISR */
#if defined(EE_TC_51_ISR) && (EE_TC_51_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_51_ISR, EE_TC_51_ISR_PRI)
#elif defined(EE_TC_51_ISR) && (EE_TC_51_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_51_ISR, EE_TC_51_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 51)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_51)
#endif /* EE_TC_51_ISR */
#if defined(EE_TC_52_ISR) && (EE_TC_52_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_52_ISR, EE_TC_52_ISR_PRI)
#elif defined(EE_TC_52_ISR) && (EE_TC_52_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_52_ISR, EE_TC_52_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 52)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_52)
#endif /* EE_TC_52_ISR */
#if defined(EE_TC_53_ISR) && (EE_TC_53_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_53_ISR, EE_TC_53_ISR_PRI)
#elif defined(EE_TC_53_ISR) && (EE_TC_53_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_53_ISR, EE_TC_53_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 53)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_53)
#endif /* EE_TC_53_ISR */
#if defined(EE_TC_54_ISR) && (EE_TC_54_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_54_ISR, EE_TC_54_ISR_PRI)
#elif defined(EE_TC_54_ISR) && (EE_TC_54_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_54_ISR, EE_TC_54_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 54)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_54)
#endif /* EE_TC_54_ISR */
#if defined(EE_TC_55_ISR) && (EE_TC_55_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_55_ISR, EE_TC_55_ISR_PRI)
#elif defined(EE_TC_55_ISR) && (EE_TC_55_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_55_ISR, EE_TC_55_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 55)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_55)
#endif /* EE_TC_55_ISR */
#if defined(EE_TC_56_ISR) && (EE_TC_56_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_56_ISR, EE_TC_56_ISR_PRI)
#elif defined(EE_TC_56_ISR) && (EE_TC_56_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_56_ISR, EE_TC_56_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 56)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_56)
#endif /* EE_TC_56_ISR */
#if defined(EE_TC_57_ISR) && (EE_TC_57_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_57_ISR, EE_TC_57_ISR_PRI)
#elif defined(EE_TC_57_ISR) && (EE_TC_57_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_57_ISR, EE_TC_57_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 57)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_57)
#endif /* EE_TC_57_ISR */
#if defined(EE_TC_58_ISR) && (EE_TC_58_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_58_ISR, EE_TC_58_ISR_PRI)
#elif defined(EE_TC_58_ISR) && (EE_TC_58_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_58_ISR, EE_TC_58_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 58)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_58)
#endif /* EE_TC_58_ISR */
#if defined(EE_TC_59_ISR) && (EE_TC_59_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_59_ISR, EE_TC_59_ISR_PRI)
#elif defined(EE_TC_59_ISR) && (EE_TC_59_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_59_ISR, EE_TC_59_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 59)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_59)
#endif /* EE_TC_59_ISR */
#if defined(EE_TC_60_ISR) && (EE_TC_60_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_60_ISR, EE_TC_60_ISR_PRI)
#elif defined(EE_TC_60_ISR) && (EE_TC_60_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_60_ISR, EE_TC_60_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 60)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_60)
#endif /* EE_TC_60_ISR */
#if defined(EE_TC_61_ISR) && (EE_TC_61_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_61_ISR, EE_TC_61_ISR_PRI)
#elif defined(EE_TC_61_ISR) && (EE_TC_61_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_61_ISR, EE_TC_61_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 61)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_61)
#endif /* EE_TC_61_ISR */
#if defined(EE_TC_62_ISR) && (EE_TC_62_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_62_ISR, EE_TC_62_ISR_PRI)
#elif defined(EE_TC_62_ISR) && (EE_TC_62_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_62_ISR, EE_TC_62_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 62)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_62)
#endif /* EE_TC_62_ISR */
#if defined(EE_TC_63_ISR) && (EE_TC_63_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_63_ISR, EE_TC_63_ISR_PRI)
#elif defined(EE_TC_63_ISR) && (EE_TC_63_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_63_ISR, EE_TC_63_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 63)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_63)
#endif /* EE_TC_63_ISR */
#if defined(EE_TC_64_ISR) && (EE_TC_64_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_64_ISR, EE_TC_64_ISR_PRI)
#elif defined(EE_TC_64_ISR) && (EE_TC_64_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_64_ISR, EE_TC_64_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 64)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_64)
#endif /* EE_TC_64_ISR */
#if defined(EE_TC_65_ISR) && (EE_TC_65_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_65_ISR, EE_TC_65_ISR_PRI)
#elif defined(EE_TC_65_ISR) && (EE_TC_65_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_65_ISR, EE_TC_65_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 65)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_65)
#endif /* EE_TC_65_ISR */
#if defined(EE_TC_66_ISR) && (EE_TC_66_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_66_ISR, EE_TC_66_ISR_PRI)
#elif defined(EE_TC_66_ISR) && (EE_TC_66_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_66_ISR, EE_TC_66_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 66)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_66)
#endif /* EE_TC_66_ISR */
#if defined(EE_TC_67_ISR) && (EE_TC_67_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_67_ISR, EE_TC_67_ISR_PRI)
#elif defined(EE_TC_67_ISR) && (EE_TC_67_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_67_ISR, EE_TC_67_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 67)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_67)
#endif /* EE_TC_67_ISR */
#if defined(EE_TC_68_ISR) && (EE_TC_68_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_68_ISR, EE_TC_68_ISR_PRI)
#elif defined(EE_TC_68_ISR) && (EE_TC_68_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_68_ISR, EE_TC_68_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 68)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_68)
#endif /* EE_TC_68_ISR */
#if defined(EE_TC_69_ISR) && (EE_TC_69_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_69_ISR, EE_TC_69_ISR_PRI)
#elif defined(EE_TC_69_ISR) && (EE_TC_69_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_69_ISR, EE_TC_69_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 69)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_69)
#endif /* EE_TC_69_ISR */
#if defined(EE_TC_70_ISR) && (EE_TC_70_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_70_ISR, EE_TC_70_ISR_PRI)
#elif defined(EE_TC_70_ISR) && (EE_TC_70_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_70_ISR, EE_TC_70_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 70)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_70)
#endif /* EE_TC_70_ISR */
#if defined(EE_TC_71_ISR) && (EE_TC_71_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_71_ISR, EE_TC_71_ISR_PRI)
#elif defined(EE_TC_71_ISR) && (EE_TC_71_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_71_ISR, EE_TC_71_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 71)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_71)
#endif /* EE_TC_71_ISR */
#if defined(EE_TC_72_ISR) && (EE_TC_72_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_72_ISR, EE_TC_72_ISR_PRI)
#elif defined(EE_TC_72_ISR) && (EE_TC_72_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_72_ISR, EE_TC_72_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 72)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_72)
#endif /* EE_TC_72_ISR */
#if defined(EE_TC_73_ISR) && (EE_TC_73_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_73_ISR, EE_TC_73_ISR_PRI)
#elif defined(EE_TC_73_ISR) && (EE_TC_73_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_73_ISR, EE_TC_73_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 73)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_73)
#endif /* EE_TC_73_ISR */
#if defined(EE_TC_74_ISR) && (EE_TC_74_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_74_ISR, EE_TC_74_ISR_PRI)
#elif defined(EE_TC_74_ISR) && (EE_TC_74_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_74_ISR, EE_TC_74_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 74)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_74)
#endif /* EE_TC_74_ISR */
#if defined(EE_TC_75_ISR) && (EE_TC_75_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_75_ISR, EE_TC_75_ISR_PRI)
#elif defined(EE_TC_75_ISR) && (EE_TC_75_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_75_ISR, EE_TC_75_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 75)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_75)
#endif /* EE_TC_75_ISR */
#if defined(EE_TC_76_ISR) && (EE_TC_76_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_76_ISR, EE_TC_76_ISR_PRI)
#elif defined(EE_TC_76_ISR) && (EE_TC_76_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_76_ISR, EE_TC_76_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 76)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_76)
#endif /* EE_TC_76_ISR */
#if defined(EE_TC_77_ISR) && (EE_TC_77_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_77_ISR, EE_TC_77_ISR_PRI)
#elif defined(EE_TC_77_ISR) && (EE_TC_77_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_77_ISR, EE_TC_77_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 77)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_77)
#endif /* EE_TC_77_ISR */
#if defined(EE_TC_78_ISR) && (EE_TC_78_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_78_ISR, EE_TC_78_ISR_PRI)
#elif defined(EE_TC_78_ISR) && (EE_TC_78_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_78_ISR, EE_TC_78_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 78)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_78)
#endif /* EE_TC_78_ISR */
#if defined(EE_TC_79_ISR) && (EE_TC_79_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_79_ISR, EE_TC_79_ISR_PRI)
#elif defined(EE_TC_79_ISR) && (EE_TC_79_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_79_ISR, EE_TC_79_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 79)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_79)
#endif /* EE_TC_79_ISR */
#if defined(EE_TC_80_ISR) && (EE_TC_80_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_80_ISR, EE_TC_80_ISR_PRI)
#elif defined(EE_TC_80_ISR) && (EE_TC_80_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_80_ISR, EE_TC_80_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 80)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_80)
#endif /* EE_TC_80_ISR */
#if defined(EE_TC_81_ISR) && (EE_TC_81_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_81_ISR, EE_TC_81_ISR_PRI)
#elif defined(EE_TC_81_ISR) && (EE_TC_81_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_81_ISR, EE_TC_81_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 81)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_81)
#endif /* EE_TC_81_ISR */
#if defined(EE_TC_82_ISR) && (EE_TC_82_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_82_ISR, EE_TC_82_ISR_PRI)
#elif defined(EE_TC_82_ISR) && (EE_TC_82_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_82_ISR, EE_TC_82_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 82)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_82)
#endif /* EE_TC_82_ISR */
#if defined(EE_TC_83_ISR) && (EE_TC_83_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_83_ISR, EE_TC_83_ISR_PRI)
#elif defined(EE_TC_83_ISR) && (EE_TC_83_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_83_ISR, EE_TC_83_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 83)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_83)
#endif /* EE_TC_83_ISR */
#if defined(EE_TC_84_ISR) && (EE_TC_84_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_84_ISR, EE_TC_84_ISR_PRI)
#elif defined(EE_TC_84_ISR) && (EE_TC_84_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_84_ISR, EE_TC_84_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 84)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_84)
#endif /* EE_TC_84_ISR */
#if defined(EE_TC_85_ISR) && (EE_TC_85_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_85_ISR, EE_TC_85_ISR_PRI)
#elif defined(EE_TC_85_ISR) && (EE_TC_85_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_85_ISR, EE_TC_85_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 85)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_85)
#endif /* EE_TC_85_ISR */
#if defined(EE_TC_86_ISR) && (EE_TC_86_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_86_ISR, EE_TC_86_ISR_PRI)
#elif defined(EE_TC_86_ISR) && (EE_TC_86_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_86_ISR, EE_TC_86_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 86)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_86)
#endif /* EE_TC_86_ISR */
#if defined(EE_TC_87_ISR) && (EE_TC_87_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_87_ISR, EE_TC_87_ISR_PRI)
#elif defined(EE_TC_87_ISR) && (EE_TC_87_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_87_ISR, EE_TC_87_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 87)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_87)
#endif /* EE_TC_87_ISR */
#if defined(EE_TC_88_ISR) && (EE_TC_88_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_88_ISR, EE_TC_88_ISR_PRI)
#elif defined(EE_TC_88_ISR) && (EE_TC_88_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_88_ISR, EE_TC_88_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 88)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_88)
#endif /* EE_TC_88_ISR */
#if defined(EE_TC_89_ISR) && (EE_TC_89_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_89_ISR, EE_TC_89_ISR_PRI)
#elif defined(EE_TC_89_ISR) && (EE_TC_89_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_89_ISR, EE_TC_89_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 89)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_89)
#endif /* EE_TC_89_ISR */
#if defined(EE_TC_90_ISR) && (EE_TC_90_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_90_ISR, EE_TC_90_ISR_PRI)
#elif defined(EE_TC_90_ISR) && (EE_TC_90_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_90_ISR, EE_TC_90_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 90)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_90)
#endif /* EE_TC_90_ISR */
#if defined(EE_TC_91_ISR) && (EE_TC_91_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_91_ISR, EE_TC_91_ISR_PRI)
#elif defined(EE_TC_91_ISR) && (EE_TC_91_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_91_ISR, EE_TC_91_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 91)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_91)
#endif /* EE_TC_91_ISR */
#if defined(EE_TC_92_ISR) && (EE_TC_92_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_92_ISR, EE_TC_92_ISR_PRI)
#elif defined(EE_TC_92_ISR) && (EE_TC_92_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_92_ISR, EE_TC_92_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 92)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_92)
#endif /* EE_TC_92_ISR */
#if defined(EE_TC_93_ISR) && (EE_TC_93_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_93_ISR, EE_TC_93_ISR_PRI)
#elif defined(EE_TC_93_ISR) && (EE_TC_93_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_93_ISR, EE_TC_93_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 93)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_93)
#endif /* EE_TC_93_ISR */
#if defined(EE_TC_94_ISR) && (EE_TC_94_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_94_ISR, EE_TC_94_ISR_PRI)
#elif defined(EE_TC_94_ISR) && (EE_TC_94_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_94_ISR, EE_TC_94_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 94)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_94)
#endif /* EE_TC_94_ISR */
#if defined(EE_TC_95_ISR) && (EE_TC_95_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_95_ISR, EE_TC_95_ISR_PRI)
#elif defined(EE_TC_95_ISR) && (EE_TC_95_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_95_ISR, EE_TC_95_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 95)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_95)
#endif /* EE_TC_95_ISR */
#if defined(EE_TC_96_ISR) && (EE_TC_96_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_96_ISR, EE_TC_96_ISR_PRI)
#elif defined(EE_TC_96_ISR) && (EE_TC_96_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_96_ISR, EE_TC_96_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 96)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_96)
#endif /* EE_TC_96_ISR */
#if defined(EE_TC_97_ISR) && (EE_TC_97_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_97_ISR, EE_TC_97_ISR_PRI)
#elif defined(EE_TC_97_ISR) && (EE_TC_97_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_97_ISR, EE_TC_97_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 97)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_97)
#endif /* EE_TC_97_ISR */
#if defined(EE_TC_98_ISR) && (EE_TC_98_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_98_ISR, EE_TC_98_ISR_PRI)
#elif defined(EE_TC_98_ISR) && (EE_TC_98_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_98_ISR, EE_TC_98_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 98)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_98)
#endif /* EE_TC_98_ISR */
#if defined(EE_TC_99_ISR) && (EE_TC_99_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_99_ISR, EE_TC_99_ISR_PRI)
#elif defined(EE_TC_99_ISR) && (EE_TC_99_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_99_ISR, EE_TC_99_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 99)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_99)
#endif /* EE_TC_99_ISR */
#if defined(EE_TC_100_ISR) && (EE_TC_100_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_100_ISR, EE_TC_100_ISR_PRI)
#elif defined(EE_TC_100_ISR) && (EE_TC_100_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_100_ISR, EE_TC_100_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 100)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_100)
#endif /* EE_TC_100_ISR */
#if defined(EE_TC_101_ISR) && (EE_TC_101_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_101_ISR, EE_TC_101_ISR_PRI)
#elif defined(EE_TC_101_ISR) && (EE_TC_101_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_101_ISR, EE_TC_101_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 101)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_101)
#endif /* EE_TC_101_ISR */
#if defined(EE_TC_102_ISR) && (EE_TC_102_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_102_ISR, EE_TC_102_ISR_PRI)
#elif defined(EE_TC_102_ISR) && (EE_TC_102_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_102_ISR, EE_TC_102_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 102)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_102)
#endif /* EE_TC_102_ISR */
#if defined(EE_TC_103_ISR) && (EE_TC_103_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_103_ISR, EE_TC_103_ISR_PRI)
#elif defined(EE_TC_103_ISR) && (EE_TC_103_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_103_ISR, EE_TC_103_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 103)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_103)
#endif /* EE_TC_103_ISR */
#if defined(EE_TC_104_ISR) && (EE_TC_104_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_104_ISR, EE_TC_104_ISR_PRI)
#elif defined(EE_TC_104_ISR) && (EE_TC_104_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_104_ISR, EE_TC_104_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 104)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_104)
#endif /* EE_TC_104_ISR */
#if defined(EE_TC_105_ISR) && (EE_TC_105_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_105_ISR, EE_TC_105_ISR_PRI)
#elif defined(EE_TC_105_ISR) && (EE_TC_105_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_105_ISR, EE_TC_105_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 105)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_105)
#endif /* EE_TC_105_ISR */
#if defined(EE_TC_106_ISR) && (EE_TC_106_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_106_ISR, EE_TC_106_ISR_PRI)
#elif defined(EE_TC_106_ISR) && (EE_TC_106_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_106_ISR, EE_TC_106_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 106)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_106)
#endif /* EE_TC_106_ISR */
#if defined(EE_TC_107_ISR) && (EE_TC_107_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_107_ISR, EE_TC_107_ISR_PRI)
#elif defined(EE_TC_107_ISR) && (EE_TC_107_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_107_ISR, EE_TC_107_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 107)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_107)
#endif /* EE_TC_107_ISR */
#if defined(EE_TC_108_ISR) && (EE_TC_108_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_108_ISR, EE_TC_108_ISR_PRI)
#elif defined(EE_TC_108_ISR) && (EE_TC_108_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_108_ISR, EE_TC_108_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 108)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_108)
#endif /* EE_TC_108_ISR */
#if defined(EE_TC_109_ISR) && (EE_TC_109_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_109_ISR, EE_TC_109_ISR_PRI)
#elif defined(EE_TC_109_ISR) && (EE_TC_109_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_109_ISR, EE_TC_109_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 109)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_109)
#endif /* EE_TC_109_ISR */
#if defined(EE_TC_110_ISR) && (EE_TC_110_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_110_ISR, EE_TC_110_ISR_PRI)
#elif defined(EE_TC_110_ISR) && (EE_TC_110_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_110_ISR, EE_TC_110_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 110)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_110)
#endif /* EE_TC_110_ISR */
#if defined(EE_TC_111_ISR) && (EE_TC_111_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_111_ISR, EE_TC_111_ISR_PRI)
#elif defined(EE_TC_111_ISR) && (EE_TC_111_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_111_ISR, EE_TC_111_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 111)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_111)
#endif /* EE_TC_111_ISR */
#if defined(EE_TC_112_ISR) && (EE_TC_112_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_112_ISR, EE_TC_112_ISR_PRI)
#elif defined(EE_TC_112_ISR) && (EE_TC_112_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_112_ISR, EE_TC_112_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 112)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_112)
#endif /* EE_TC_112_ISR */
#if defined(EE_TC_113_ISR) && (EE_TC_113_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_113_ISR, EE_TC_113_ISR_PRI)
#elif defined(EE_TC_113_ISR) && (EE_TC_113_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_113_ISR, EE_TC_113_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 113)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_113)
#endif /* EE_TC_113_ISR */
#if defined(EE_TC_114_ISR) && (EE_TC_114_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_114_ISR, EE_TC_114_ISR_PRI)
#elif defined(EE_TC_114_ISR) && (EE_TC_114_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_114_ISR, EE_TC_114_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 114)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_114)
#endif /* EE_TC_114_ISR */
#if defined(EE_TC_115_ISR) && (EE_TC_115_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_115_ISR, EE_TC_115_ISR_PRI)
#elif defined(EE_TC_115_ISR) && (EE_TC_115_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_115_ISR, EE_TC_115_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 115)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_115)
#endif /* EE_TC_115_ISR */
#if defined(EE_TC_116_ISR) && (EE_TC_116_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_116_ISR, EE_TC_116_ISR_PRI)
#elif defined(EE_TC_116_ISR) && (EE_TC_116_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_116_ISR, EE_TC_116_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 116)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_116)
#endif /* EE_TC_116_ISR */
#if defined(EE_TC_117_ISR) && (EE_TC_117_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_117_ISR, EE_TC_117_ISR_PRI)
#elif defined(EE_TC_117_ISR) && (EE_TC_117_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_117_ISR, EE_TC_117_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 117)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_117)
#endif /* EE_TC_117_ISR */
#if defined(EE_TC_118_ISR) && (EE_TC_118_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_118_ISR, EE_TC_118_ISR_PRI)
#elif defined(EE_TC_118_ISR) && (EE_TC_118_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_118_ISR, EE_TC_118_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 118)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_118)
#endif /* EE_TC_118_ISR */
#if defined(EE_TC_119_ISR) && (EE_TC_119_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_119_ISR, EE_TC_119_ISR_PRI)
#elif defined(EE_TC_119_ISR) && (EE_TC_119_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_119_ISR, EE_TC_119_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 119)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_119)
#endif /* EE_TC_119_ISR */
#if defined(EE_TC_120_ISR) && (EE_TC_120_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_120_ISR, EE_TC_120_ISR_PRI)
#elif defined(EE_TC_120_ISR) && (EE_TC_120_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_120_ISR, EE_TC_120_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 120)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_120)
#endif /* EE_TC_120_ISR */
#if defined(EE_TC_121_ISR) && (EE_TC_121_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_121_ISR, EE_TC_121_ISR_PRI)
#elif defined(EE_TC_121_ISR) && (EE_TC_121_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_121_ISR, EE_TC_121_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 121)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_121)
#endif /* EE_TC_121_ISR */
#if defined(EE_TC_122_ISR) && (EE_TC_122_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_122_ISR, EE_TC_122_ISR_PRI)
#elif defined(EE_TC_122_ISR) && (EE_TC_122_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_122_ISR, EE_TC_122_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 122)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_122)
#endif /* EE_TC_122_ISR */
#if defined(EE_TC_123_ISR) && (EE_TC_123_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_123_ISR, EE_TC_123_ISR_PRI)
#elif defined(EE_TC_123_ISR) && (EE_TC_123_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_123_ISR, EE_TC_123_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 123)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_123)
#endif /* EE_TC_123_ISR */
#if defined(EE_TC_124_ISR) && (EE_TC_124_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_124_ISR, EE_TC_124_ISR_PRI)
#elif defined(EE_TC_124_ISR) && (EE_TC_124_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_124_ISR, EE_TC_124_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 124)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_124)
#endif /* EE_TC_124_ISR */
#if defined(EE_TC_125_ISR) && (EE_TC_125_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_125_ISR, EE_TC_125_ISR_PRI)
#elif defined(EE_TC_125_ISR) && (EE_TC_125_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_125_ISR, EE_TC_125_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 125)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_125)
#endif /* EE_TC_125_ISR */
#if defined(EE_TC_126_ISR) && (EE_TC_126_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_126_ISR, EE_TC_126_ISR_PRI)
#elif defined(EE_TC_126_ISR) && (EE_TC_126_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_126_ISR, EE_TC_126_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 126)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_126)
#endif /* EE_TC_126_ISR */
#if defined(EE_TC_127_ISR) && (EE_TC_127_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_127_ISR, EE_TC_127_ISR_PRI)
#elif defined(EE_TC_127_ISR) && (EE_TC_127_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_127_ISR, EE_TC_127_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 127)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_127)
#endif /* EE_TC_127_ISR */
#if defined(EE_TC_128_ISR) && (EE_TC_128_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_128_ISR, EE_TC_128_ISR_PRI)
#elif defined(EE_TC_128_ISR) && (EE_TC_128_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_128_ISR, EE_TC_128_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 128)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_128)
#endif /* EE_TC_128_ISR */
#if defined(EE_TC_129_ISR) && (EE_TC_129_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_129_ISR, EE_TC_129_ISR_PRI)
#elif defined(EE_TC_129_ISR) && (EE_TC_129_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_129_ISR, EE_TC_129_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 129)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_129)
#endif /* EE_TC_129_ISR */
#if defined(EE_TC_130_ISR) && (EE_TC_130_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_130_ISR, EE_TC_130_ISR_PRI)
#elif defined(EE_TC_130_ISR) && (EE_TC_130_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_130_ISR, EE_TC_130_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 130)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_130)
#endif /* EE_TC_130_ISR */
#if defined(EE_TC_131_ISR) && (EE_TC_131_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_131_ISR, EE_TC_131_ISR_PRI)
#elif defined(EE_TC_131_ISR) && (EE_TC_131_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_131_ISR, EE_TC_131_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 131)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_131)
#endif /* EE_TC_131_ISR */
#if defined(EE_TC_132_ISR) && (EE_TC_132_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_132_ISR, EE_TC_132_ISR_PRI)
#elif defined(EE_TC_132_ISR) && (EE_TC_132_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_132_ISR, EE_TC_132_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 132)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_132)
#endif /* EE_TC_132_ISR */
#if defined(EE_TC_133_ISR) && (EE_TC_133_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_133_ISR, EE_TC_133_ISR_PRI)
#elif defined(EE_TC_133_ISR) && (EE_TC_133_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_133_ISR, EE_TC_133_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 133)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_133)
#endif /* EE_TC_133_ISR */
#if defined(EE_TC_134_ISR) && (EE_TC_134_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_134_ISR, EE_TC_134_ISR_PRI)
#elif defined(EE_TC_134_ISR) && (EE_TC_134_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_134_ISR, EE_TC_134_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 134)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_134)
#endif /* EE_TC_134_ISR */
#if defined(EE_TC_135_ISR) && (EE_TC_135_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_135_ISR, EE_TC_135_ISR_PRI)
#elif defined(EE_TC_135_ISR) && (EE_TC_135_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_135_ISR, EE_TC_135_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 135)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_135)
#endif /* EE_TC_135_ISR */
#if defined(EE_TC_136_ISR) && (EE_TC_136_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_136_ISR, EE_TC_136_ISR_PRI)
#elif defined(EE_TC_136_ISR) && (EE_TC_136_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_136_ISR, EE_TC_136_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 136)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_136)
#endif /* EE_TC_136_ISR */
#if defined(EE_TC_137_ISR) && (EE_TC_137_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_137_ISR, EE_TC_137_ISR_PRI)
#elif defined(EE_TC_137_ISR) && (EE_TC_137_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_137_ISR, EE_TC_137_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 137)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_137)
#endif /* EE_TC_137_ISR */
#if defined(EE_TC_138_ISR) && (EE_TC_138_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_138_ISR, EE_TC_138_ISR_PRI)
#elif defined(EE_TC_138_ISR) && (EE_TC_138_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_138_ISR, EE_TC_138_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 138)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_138)
#endif /* EE_TC_138_ISR */
#if defined(EE_TC_139_ISR) && (EE_TC_139_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_139_ISR, EE_TC_139_ISR_PRI)
#elif defined(EE_TC_139_ISR) && (EE_TC_139_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_139_ISR, EE_TC_139_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 139)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_139)
#endif /* EE_TC_139_ISR */
#if defined(EE_TC_140_ISR) && (EE_TC_140_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_140_ISR, EE_TC_140_ISR_PRI)
#elif defined(EE_TC_140_ISR) && (EE_TC_140_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_140_ISR, EE_TC_140_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 140)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_140)
#endif /* EE_TC_140_ISR */
#if defined(EE_TC_141_ISR) && (EE_TC_141_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_141_ISR, EE_TC_141_ISR_PRI)
#elif defined(EE_TC_141_ISR) && (EE_TC_141_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_141_ISR, EE_TC_141_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 141)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_141)
#endif /* EE_TC_141_ISR */
#if defined(EE_TC_142_ISR) && (EE_TC_142_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_142_ISR, EE_TC_142_ISR_PRI)
#elif defined(EE_TC_142_ISR) && (EE_TC_142_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_142_ISR, EE_TC_142_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 142)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_142)
#endif /* EE_TC_142_ISR */
#if defined(EE_TC_143_ISR) && (EE_TC_143_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_143_ISR, EE_TC_143_ISR_PRI)
#elif defined(EE_TC_143_ISR) && (EE_TC_143_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_143_ISR, EE_TC_143_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 143)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_143)
#endif /* EE_TC_143_ISR */
#if defined(EE_TC_144_ISR) && (EE_TC_144_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_144_ISR, EE_TC_144_ISR_PRI)
#elif defined(EE_TC_144_ISR) && (EE_TC_144_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_144_ISR, EE_TC_144_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 144)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_144)
#endif /* EE_TC_144_ISR */
#if defined(EE_TC_145_ISR) && (EE_TC_145_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_145_ISR, EE_TC_145_ISR_PRI)
#elif defined(EE_TC_145_ISR) && (EE_TC_145_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_145_ISR, EE_TC_145_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 145)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_145)
#endif /* EE_TC_145_ISR */
#if defined(EE_TC_146_ISR) && (EE_TC_146_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_146_ISR, EE_TC_146_ISR_PRI)
#elif defined(EE_TC_146_ISR) && (EE_TC_146_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_146_ISR, EE_TC_146_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 146)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_146)
#endif /* EE_TC_146_ISR */
#if defined(EE_TC_147_ISR) && (EE_TC_147_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_147_ISR, EE_TC_147_ISR_PRI)
#elif defined(EE_TC_147_ISR) && (EE_TC_147_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_147_ISR, EE_TC_147_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 147)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_147)
#endif /* EE_TC_147_ISR */
#if defined(EE_TC_148_ISR) && (EE_TC_148_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_148_ISR, EE_TC_148_ISR_PRI)
#elif defined(EE_TC_148_ISR) && (EE_TC_148_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_148_ISR, EE_TC_148_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 148)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_148)
#endif /* EE_TC_148_ISR */
#if defined(EE_TC_149_ISR) && (EE_TC_149_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_149_ISR, EE_TC_149_ISR_PRI)
#elif defined(EE_TC_149_ISR) && (EE_TC_149_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_149_ISR, EE_TC_149_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 149)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_149)
#endif /* EE_TC_149_ISR */
#if defined(EE_TC_150_ISR) && (EE_TC_150_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_150_ISR, EE_TC_150_ISR_PRI)
#elif defined(EE_TC_150_ISR) && (EE_TC_150_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_150_ISR, EE_TC_150_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 150)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_150)
#endif /* EE_TC_150_ISR */
#if defined(EE_TC_151_ISR) && (EE_TC_151_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_151_ISR, EE_TC_151_ISR_PRI)
#elif defined(EE_TC_151_ISR) && (EE_TC_151_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_151_ISR, EE_TC_151_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 151)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_151)
#endif /* EE_TC_151_ISR */
#if defined(EE_TC_152_ISR) && (EE_TC_152_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_152_ISR, EE_TC_152_ISR_PRI)
#elif defined(EE_TC_152_ISR) && (EE_TC_152_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_152_ISR, EE_TC_152_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 152)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_152)
#endif /* EE_TC_152_ISR */
#if defined(EE_TC_153_ISR) && (EE_TC_153_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_153_ISR, EE_TC_153_ISR_PRI)
#elif defined(EE_TC_153_ISR) && (EE_TC_153_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_153_ISR, EE_TC_153_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 153)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_153)
#endif /* EE_TC_153_ISR */
#if defined(EE_TC_154_ISR) && (EE_TC_154_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_154_ISR, EE_TC_154_ISR_PRI)
#elif defined(EE_TC_154_ISR) && (EE_TC_154_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_154_ISR, EE_TC_154_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 154)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_154)
#endif /* EE_TC_154_ISR */
#if defined(EE_TC_155_ISR) && (EE_TC_155_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_155_ISR, EE_TC_155_ISR_PRI)
#elif defined(EE_TC_155_ISR) && (EE_TC_155_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_155_ISR, EE_TC_155_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 155)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_155)
#endif /* EE_TC_155_ISR */
#if defined(EE_TC_156_ISR) && (EE_TC_156_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_156_ISR, EE_TC_156_ISR_PRI)
#elif defined(EE_TC_156_ISR) && (EE_TC_156_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_156_ISR, EE_TC_156_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 156)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_156)
#endif /* EE_TC_156_ISR */
#if defined(EE_TC_157_ISR) && (EE_TC_157_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_157_ISR, EE_TC_157_ISR_PRI)
#elif defined(EE_TC_157_ISR) && (EE_TC_157_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_157_ISR, EE_TC_157_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 157)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_157)
#endif /* EE_TC_157_ISR */
#if defined(EE_TC_158_ISR) && (EE_TC_158_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_158_ISR, EE_TC_158_ISR_PRI)
#elif defined(EE_TC_158_ISR) && (EE_TC_158_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_158_ISR, EE_TC_158_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 158)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_158)
#endif /* EE_TC_158_ISR */
#if defined(EE_TC_159_ISR) && (EE_TC_159_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_159_ISR, EE_TC_159_ISR_PRI)
#elif defined(EE_TC_159_ISR) && (EE_TC_159_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_159_ISR, EE_TC_159_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 159)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_159)
#endif /* EE_TC_159_ISR */
#if defined(EE_TC_160_ISR) && (EE_TC_160_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_160_ISR, EE_TC_160_ISR_PRI)
#elif defined(EE_TC_160_ISR) && (EE_TC_160_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_160_ISR, EE_TC_160_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 160)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_160)
#endif /* EE_TC_160_ISR */
#if defined(EE_TC_161_ISR) && (EE_TC_161_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_161_ISR, EE_TC_161_ISR_PRI)
#elif defined(EE_TC_161_ISR) && (EE_TC_161_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_161_ISR, EE_TC_161_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 161)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_161)
#endif /* EE_TC_161_ISR */
#if defined(EE_TC_162_ISR) && (EE_TC_162_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_162_ISR, EE_TC_162_ISR_PRI)
#elif defined(EE_TC_162_ISR) && (EE_TC_162_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_162_ISR, EE_TC_162_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 162)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_162)
#endif /* EE_TC_162_ISR */
#if defined(EE_TC_163_ISR) && (EE_TC_163_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_163_ISR, EE_TC_163_ISR_PRI)
#elif defined(EE_TC_163_ISR) && (EE_TC_163_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_163_ISR, EE_TC_163_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 163)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_163)
#endif /* EE_TC_163_ISR */
#if defined(EE_TC_164_ISR) && (EE_TC_164_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_164_ISR, EE_TC_164_ISR_PRI)
#elif defined(EE_TC_164_ISR) && (EE_TC_164_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_164_ISR, EE_TC_164_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 164)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_164)
#endif /* EE_TC_164_ISR */
#if defined(EE_TC_165_ISR) && (EE_TC_165_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_165_ISR, EE_TC_165_ISR_PRI)
#elif defined(EE_TC_165_ISR) && (EE_TC_165_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_165_ISR, EE_TC_165_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 165)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_165)
#endif /* EE_TC_165_ISR */
#if defined(EE_TC_166_ISR) && (EE_TC_166_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_166_ISR, EE_TC_166_ISR_PRI)
#elif defined(EE_TC_166_ISR) && (EE_TC_166_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_166_ISR, EE_TC_166_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 166)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_166)
#endif /* EE_TC_166_ISR */
#if defined(EE_TC_167_ISR) && (EE_TC_167_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_167_ISR, EE_TC_167_ISR_PRI)
#elif defined(EE_TC_167_ISR) && (EE_TC_167_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_167_ISR, EE_TC_167_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 167)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_167)
#endif /* EE_TC_167_ISR */
#if defined(EE_TC_168_ISR) && (EE_TC_168_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_168_ISR, EE_TC_168_ISR_PRI)
#elif defined(EE_TC_168_ISR) && (EE_TC_168_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_168_ISR, EE_TC_168_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 168)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_168)
#endif /* EE_TC_168_ISR */
#if defined(EE_TC_169_ISR) && (EE_TC_169_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_169_ISR, EE_TC_169_ISR_PRI)
#elif defined(EE_TC_169_ISR) && (EE_TC_169_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_169_ISR, EE_TC_169_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 169)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_169)
#endif /* EE_TC_169_ISR */
#if defined(EE_TC_170_ISR) && (EE_TC_170_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_170_ISR, EE_TC_170_ISR_PRI)
#elif defined(EE_TC_170_ISR) && (EE_TC_170_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_170_ISR, EE_TC_170_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 170)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_170)
#endif /* EE_TC_170_ISR */
#if defined(EE_TC_171_ISR) && (EE_TC_171_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_171_ISR, EE_TC_171_ISR_PRI)
#elif defined(EE_TC_171_ISR) && (EE_TC_171_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_171_ISR, EE_TC_171_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 171)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_171)
#endif /* EE_TC_171_ISR */
#if defined(EE_TC_172_ISR) && (EE_TC_172_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_172_ISR, EE_TC_172_ISR_PRI)
#elif defined(EE_TC_172_ISR) && (EE_TC_172_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_172_ISR, EE_TC_172_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 172)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_172)
#endif /* EE_TC_172_ISR */
#if defined(EE_TC_173_ISR) && (EE_TC_173_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_173_ISR, EE_TC_173_ISR_PRI)
#elif defined(EE_TC_173_ISR) && (EE_TC_173_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_173_ISR, EE_TC_173_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 173)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_173)
#endif /* EE_TC_173_ISR */
#if defined(EE_TC_174_ISR) && (EE_TC_174_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_174_ISR, EE_TC_174_ISR_PRI)
#elif defined(EE_TC_174_ISR) && (EE_TC_174_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_174_ISR, EE_TC_174_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 174)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_174)
#endif /* EE_TC_174_ISR */
#if defined(EE_TC_175_ISR) && (EE_TC_175_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_175_ISR, EE_TC_175_ISR_PRI)
#elif defined(EE_TC_175_ISR) && (EE_TC_175_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_175_ISR, EE_TC_175_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 175)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_175)
#endif /* EE_TC_175_ISR */
#if defined(EE_TC_176_ISR) && (EE_TC_176_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_176_ISR, EE_TC_176_ISR_PRI)
#elif defined(EE_TC_176_ISR) && (EE_TC_176_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_176_ISR, EE_TC_176_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 176)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_176)
#endif /* EE_TC_176_ISR */
#if defined(EE_TC_177_ISR) && (EE_TC_177_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_177_ISR, EE_TC_177_ISR_PRI)
#elif defined(EE_TC_177_ISR) && (EE_TC_177_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_177_ISR, EE_TC_177_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 177)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_177)
#endif /* EE_TC_177_ISR */
#if defined(EE_TC_178_ISR) && (EE_TC_178_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_178_ISR, EE_TC_178_ISR_PRI)
#elif defined(EE_TC_178_ISR) && (EE_TC_178_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_178_ISR, EE_TC_178_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 178)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_178)
#endif /* EE_TC_178_ISR */
#if defined(EE_TC_179_ISR) && (EE_TC_179_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_179_ISR, EE_TC_179_ISR_PRI)
#elif defined(EE_TC_179_ISR) && (EE_TC_179_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_179_ISR, EE_TC_179_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 179)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_179)
#endif /* EE_TC_179_ISR */
#if defined(EE_TC_180_ISR) && (EE_TC_180_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_180_ISR, EE_TC_180_ISR_PRI)
#elif defined(EE_TC_180_ISR) && (EE_TC_180_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_180_ISR, EE_TC_180_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 180)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_180)
#endif /* EE_TC_180_ISR */
#if defined(EE_TC_181_ISR) && (EE_TC_181_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_181_ISR, EE_TC_181_ISR_PRI)
#elif defined(EE_TC_181_ISR) && (EE_TC_181_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_181_ISR, EE_TC_181_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 181)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_181)
#endif /* EE_TC_181_ISR */
#if defined(EE_TC_182_ISR) && (EE_TC_182_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_182_ISR, EE_TC_182_ISR_PRI)
#elif defined(EE_TC_182_ISR) && (EE_TC_182_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_182_ISR, EE_TC_182_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 182)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_182)
#endif /* EE_TC_182_ISR */
#if defined(EE_TC_183_ISR) && (EE_TC_183_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_183_ISR, EE_TC_183_ISR_PRI)
#elif defined(EE_TC_183_ISR) && (EE_TC_183_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_183_ISR, EE_TC_183_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 183)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_183)
#endif /* EE_TC_183_ISR */
#if defined(EE_TC_184_ISR) && (EE_TC_184_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_184_ISR, EE_TC_184_ISR_PRI)
#elif defined(EE_TC_184_ISR) && (EE_TC_184_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_184_ISR, EE_TC_184_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 184)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_184)
#endif /* EE_TC_184_ISR */
#if defined(EE_TC_185_ISR) && (EE_TC_185_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_185_ISR, EE_TC_185_ISR_PRI)
#elif defined(EE_TC_185_ISR) && (EE_TC_185_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_185_ISR, EE_TC_185_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 185)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_185)
#endif /* EE_TC_185_ISR */
#if defined(EE_TC_186_ISR) && (EE_TC_186_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_186_ISR, EE_TC_186_ISR_PRI)
#elif defined(EE_TC_186_ISR) && (EE_TC_186_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_186_ISR, EE_TC_186_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 186)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_186)
#endif /* EE_TC_186_ISR */
#if defined(EE_TC_187_ISR) && (EE_TC_187_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_187_ISR, EE_TC_187_ISR_PRI)
#elif defined(EE_TC_187_ISR) && (EE_TC_187_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_187_ISR, EE_TC_187_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 187)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_187)
#endif /* EE_TC_187_ISR */
#if defined(EE_TC_188_ISR) && (EE_TC_188_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_188_ISR, EE_TC_188_ISR_PRI)
#elif defined(EE_TC_188_ISR) && (EE_TC_188_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_188_ISR, EE_TC_188_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 188)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_188)
#endif /* EE_TC_188_ISR */
#if defined(EE_TC_189_ISR) && (EE_TC_189_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_189_ISR, EE_TC_189_ISR_PRI)
#elif defined(EE_TC_189_ISR) && (EE_TC_189_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_189_ISR, EE_TC_189_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 189)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_189)
#endif /* EE_TC_189_ISR */
#if defined(EE_TC_190_ISR) && (EE_TC_190_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_190_ISR, EE_TC_190_ISR_PRI)
#elif defined(EE_TC_190_ISR) && (EE_TC_190_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_190_ISR, EE_TC_190_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 190)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_190)
#endif /* EE_TC_190_ISR */
#if defined(EE_TC_191_ISR) && (EE_TC_191_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_191_ISR, EE_TC_191_ISR_PRI)
#elif defined(EE_TC_191_ISR) && (EE_TC_191_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_191_ISR, EE_TC_191_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 191)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_191)
#endif /* EE_TC_191_ISR */
#if defined(EE_TC_192_ISR) && (EE_TC_192_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_192_ISR, EE_TC_192_ISR_PRI)
#elif defined(EE_TC_192_ISR) && (EE_TC_192_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_192_ISR, EE_TC_192_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 192)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_192)
#endif /* EE_TC_192_ISR */
#if defined(EE_TC_193_ISR) && (EE_TC_193_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_193_ISR, EE_TC_193_ISR_PRI)
#elif defined(EE_TC_193_ISR) && (EE_TC_193_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_193_ISR, EE_TC_193_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 193)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_193)
#endif /* EE_TC_193_ISR */
#if defined(EE_TC_194_ISR) && (EE_TC_194_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_194_ISR, EE_TC_194_ISR_PRI)
#elif defined(EE_TC_194_ISR) && (EE_TC_194_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_194_ISR, EE_TC_194_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 194)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_194)
#endif /* EE_TC_194_ISR */
#if defined(EE_TC_195_ISR) && (EE_TC_195_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_195_ISR, EE_TC_195_ISR_PRI)
#elif defined(EE_TC_195_ISR) && (EE_TC_195_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_195_ISR, EE_TC_195_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 195)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_195)
#endif /* EE_TC_195_ISR */
#if defined(EE_TC_196_ISR) && (EE_TC_196_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_196_ISR, EE_TC_196_ISR_PRI)
#elif defined(EE_TC_196_ISR) && (EE_TC_196_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_196_ISR, EE_TC_196_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 196)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_196)
#endif /* EE_TC_196_ISR */
#if defined(EE_TC_197_ISR) && (EE_TC_197_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_197_ISR, EE_TC_197_ISR_PRI)
#elif defined(EE_TC_197_ISR) && (EE_TC_197_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_197_ISR, EE_TC_197_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 197)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_197)
#endif /* EE_TC_197_ISR */
#if defined(EE_TC_198_ISR) && (EE_TC_198_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_198_ISR, EE_TC_198_ISR_PRI)
#elif defined(EE_TC_198_ISR) && (EE_TC_198_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_198_ISR, EE_TC_198_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 198)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_198)
#endif /* EE_TC_198_ISR */
#if defined(EE_TC_199_ISR) && (EE_TC_199_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_199_ISR, EE_TC_199_ISR_PRI)
#elif defined(EE_TC_199_ISR) && (EE_TC_199_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_199_ISR, EE_TC_199_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 199)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_199)
#endif /* EE_TC_199_ISR */
#if defined(EE_TC_200_ISR) && (EE_TC_200_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_200_ISR, EE_TC_200_ISR_PRI)
#elif defined(EE_TC_200_ISR) && (EE_TC_200_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_200_ISR, EE_TC_200_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 200)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_200)
#endif /* EE_TC_200_ISR */
#if defined(EE_TC_201_ISR) && (EE_TC_201_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_201_ISR, EE_TC_201_ISR_PRI)
#elif defined(EE_TC_201_ISR) && (EE_TC_201_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_201_ISR, EE_TC_201_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 201)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_201)
#endif /* EE_TC_201_ISR */
#if defined(EE_TC_202_ISR) && (EE_TC_202_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_202_ISR, EE_TC_202_ISR_PRI)
#elif defined(EE_TC_202_ISR) && (EE_TC_202_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_202_ISR, EE_TC_202_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 202)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_202)
#endif /* EE_TC_202_ISR */
#if defined(EE_TC_203_ISR) && (EE_TC_203_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_203_ISR, EE_TC_203_ISR_PRI)
#elif defined(EE_TC_203_ISR) && (EE_TC_203_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_203_ISR, EE_TC_203_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 203)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_203)
#endif /* EE_TC_203_ISR */
#if defined(EE_TC_204_ISR) && (EE_TC_204_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_204_ISR, EE_TC_204_ISR_PRI)
#elif defined(EE_TC_204_ISR) && (EE_TC_204_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_204_ISR, EE_TC_204_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 204)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_204)
#endif /* EE_TC_204_ISR */
#if defined(EE_TC_205_ISR) && (EE_TC_205_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_205_ISR, EE_TC_205_ISR_PRI)
#elif defined(EE_TC_205_ISR) && (EE_TC_205_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_205_ISR, EE_TC_205_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 205)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_205)
#endif /* EE_TC_205_ISR */
#if defined(EE_TC_206_ISR) && (EE_TC_206_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_206_ISR, EE_TC_206_ISR_PRI)
#elif defined(EE_TC_206_ISR) && (EE_TC_206_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_206_ISR, EE_TC_206_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 206)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_206)
#endif /* EE_TC_206_ISR */
#if defined(EE_TC_207_ISR) && (EE_TC_207_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_207_ISR, EE_TC_207_ISR_PRI)
#elif defined(EE_TC_207_ISR) && (EE_TC_207_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_207_ISR, EE_TC_207_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 207)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_207)
#endif /* EE_TC_207_ISR */
#if defined(EE_TC_208_ISR) && (EE_TC_208_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_208_ISR, EE_TC_208_ISR_PRI)
#elif defined(EE_TC_208_ISR) && (EE_TC_208_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_208_ISR, EE_TC_208_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 208)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_208)
#endif /* EE_TC_208_ISR */
#if defined(EE_TC_209_ISR) && (EE_TC_209_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_209_ISR, EE_TC_209_ISR_PRI)
#elif defined(EE_TC_209_ISR) && (EE_TC_209_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_209_ISR, EE_TC_209_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 209)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_209)
#endif /* EE_TC_209_ISR */
#if defined(EE_TC_210_ISR) && (EE_TC_210_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_210_ISR, EE_TC_210_ISR_PRI)
#elif defined(EE_TC_210_ISR) && (EE_TC_210_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_210_ISR, EE_TC_210_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 210)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_210)
#endif /* EE_TC_210_ISR */
#if defined(EE_TC_211_ISR) && (EE_TC_211_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_211_ISR, EE_TC_211_ISR_PRI)
#elif defined(EE_TC_211_ISR) && (EE_TC_211_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_211_ISR, EE_TC_211_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 211)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_211)
#endif /* EE_TC_211_ISR */
#if defined(EE_TC_212_ISR) && (EE_TC_212_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_212_ISR, EE_TC_212_ISR_PRI)
#elif defined(EE_TC_212_ISR) && (EE_TC_212_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_212_ISR, EE_TC_212_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 212)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_212)
#endif /* EE_TC_212_ISR */
#if defined(EE_TC_213_ISR) && (EE_TC_213_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_213_ISR, EE_TC_213_ISR_PRI)
#elif defined(EE_TC_213_ISR) && (EE_TC_213_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_213_ISR, EE_TC_213_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 213)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_213)
#endif /* EE_TC_213_ISR */
#if defined(EE_TC_214_ISR) && (EE_TC_214_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_214_ISR, EE_TC_214_ISR_PRI)
#elif defined(EE_TC_214_ISR) && (EE_TC_214_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_214_ISR, EE_TC_214_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 214)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_214)
#endif /* EE_TC_214_ISR */
#if defined(EE_TC_215_ISR) && (EE_TC_215_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_215_ISR, EE_TC_215_ISR_PRI)
#elif defined(EE_TC_215_ISR) && (EE_TC_215_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_215_ISR, EE_TC_215_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 215)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_215)
#endif /* EE_TC_215_ISR */
#if defined(EE_TC_216_ISR) && (EE_TC_216_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_216_ISR, EE_TC_216_ISR_PRI)
#elif defined(EE_TC_216_ISR) && (EE_TC_216_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_216_ISR, EE_TC_216_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 216)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_216)
#endif /* EE_TC_216_ISR */
#if defined(EE_TC_217_ISR) && (EE_TC_217_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_217_ISR, EE_TC_217_ISR_PRI)
#elif defined(EE_TC_217_ISR) && (EE_TC_217_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_217_ISR, EE_TC_217_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 217)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_217)
#endif /* EE_TC_217_ISR */
#if defined(EE_TC_218_ISR) && (EE_TC_218_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_218_ISR, EE_TC_218_ISR_PRI)
#elif defined(EE_TC_218_ISR) && (EE_TC_218_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_218_ISR, EE_TC_218_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 218)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_218)
#endif /* EE_TC_218_ISR */
#if defined(EE_TC_219_ISR) && (EE_TC_219_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_219_ISR, EE_TC_219_ISR_PRI)
#elif defined(EE_TC_219_ISR) && (EE_TC_219_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_219_ISR, EE_TC_219_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 219)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_219)
#endif /* EE_TC_219_ISR */
#if defined(EE_TC_220_ISR) && (EE_TC_220_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_220_ISR, EE_TC_220_ISR_PRI)
#elif defined(EE_TC_220_ISR) && (EE_TC_220_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_220_ISR, EE_TC_220_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 220)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_220)
#endif /* EE_TC_220_ISR */
#if defined(EE_TC_221_ISR) && (EE_TC_221_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_221_ISR, EE_TC_221_ISR_PRI)
#elif defined(EE_TC_221_ISR) && (EE_TC_221_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_221_ISR, EE_TC_221_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 221)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_221)
#endif /* EE_TC_221_ISR */
#if defined(EE_TC_222_ISR) && (EE_TC_222_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_222_ISR, EE_TC_222_ISR_PRI)
#elif defined(EE_TC_222_ISR) && (EE_TC_222_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_222_ISR, EE_TC_222_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 222)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_222)
#endif /* EE_TC_222_ISR */
#if defined(EE_TC_223_ISR) && (EE_TC_223_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_223_ISR, EE_TC_223_ISR_PRI)
#elif defined(EE_TC_223_ISR) && (EE_TC_223_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_223_ISR, EE_TC_223_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 223)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_223)
#endif /* EE_TC_223_ISR */
#if defined(EE_TC_224_ISR) && (EE_TC_224_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_224_ISR, EE_TC_224_ISR_PRI)
#elif defined(EE_TC_224_ISR) && (EE_TC_224_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_224_ISR, EE_TC_224_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 224)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_224)
#endif /* EE_TC_224_ISR */
#if defined(EE_TC_225_ISR) && (EE_TC_225_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_225_ISR, EE_TC_225_ISR_PRI)
#elif defined(EE_TC_225_ISR) && (EE_TC_225_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_225_ISR, EE_TC_225_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 225)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_225)
#endif /* EE_TC_225_ISR */
#if defined(EE_TC_226_ISR) && (EE_TC_226_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_226_ISR, EE_TC_226_ISR_PRI)
#elif defined(EE_TC_226_ISR) && (EE_TC_226_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_226_ISR, EE_TC_226_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 226)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_226)
#endif /* EE_TC_226_ISR */
#if defined(EE_TC_227_ISR) && (EE_TC_227_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_227_ISR, EE_TC_227_ISR_PRI)
#elif defined(EE_TC_227_ISR) && (EE_TC_227_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_227_ISR, EE_TC_227_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 227)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_227)
#endif /* EE_TC_227_ISR */
#if defined(EE_TC_228_ISR) && (EE_TC_228_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_228_ISR, EE_TC_228_ISR_PRI)
#elif defined(EE_TC_228_ISR) && (EE_TC_228_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_228_ISR, EE_TC_228_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 228)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_228)
#endif /* EE_TC_228_ISR */
#if defined(EE_TC_229_ISR) && (EE_TC_229_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_229_ISR, EE_TC_229_ISR_PRI)
#elif defined(EE_TC_229_ISR) && (EE_TC_229_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_229_ISR, EE_TC_229_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 229)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_229)
#endif /* EE_TC_229_ISR */
#if defined(EE_TC_230_ISR) && (EE_TC_230_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_230_ISR, EE_TC_230_ISR_PRI)
#elif defined(EE_TC_230_ISR) && (EE_TC_230_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_230_ISR, EE_TC_230_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 230)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_230)
#endif /* EE_TC_230_ISR */
#if defined(EE_TC_231_ISR) && (EE_TC_231_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_231_ISR, EE_TC_231_ISR_PRI)
#elif defined(EE_TC_231_ISR) && (EE_TC_231_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_231_ISR, EE_TC_231_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 231)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_231)
#endif /* EE_TC_231_ISR */
#if defined(EE_TC_232_ISR) && (EE_TC_232_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_232_ISR, EE_TC_232_ISR_PRI)
#elif defined(EE_TC_232_ISR) && (EE_TC_232_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_232_ISR, EE_TC_232_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 232)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_232)
#endif /* EE_TC_232_ISR */
#if defined(EE_TC_233_ISR) && (EE_TC_233_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_233_ISR, EE_TC_233_ISR_PRI)
#elif defined(EE_TC_233_ISR) && (EE_TC_233_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_233_ISR, EE_TC_233_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 233)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_233)
#endif /* EE_TC_233_ISR */
#if defined(EE_TC_234_ISR) && (EE_TC_234_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_234_ISR, EE_TC_234_ISR_PRI)
#elif defined(EE_TC_234_ISR) && (EE_TC_234_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_234_ISR, EE_TC_234_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 234)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_234)
#endif /* EE_TC_234_ISR */
#if defined(EE_TC_235_ISR) && (EE_TC_235_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_235_ISR, EE_TC_235_ISR_PRI)
#elif defined(EE_TC_235_ISR) && (EE_TC_235_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_235_ISR, EE_TC_235_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 235)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_235)
#endif /* EE_TC_235_ISR */
#if defined(EE_TC_236_ISR) && (EE_TC_236_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_236_ISR, EE_TC_236_ISR_PRI)
#elif defined(EE_TC_236_ISR) && (EE_TC_236_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_236_ISR, EE_TC_236_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 236)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_236)
#endif /* EE_TC_236_ISR */
#if defined(EE_TC_237_ISR) && (EE_TC_237_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_237_ISR, EE_TC_237_ISR_PRI)
#elif defined(EE_TC_237_ISR) && (EE_TC_237_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_237_ISR, EE_TC_237_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 237)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_237)
#endif /* EE_TC_237_ISR */
#if defined(EE_TC_238_ISR) && (EE_TC_238_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_238_ISR, EE_TC_238_ISR_PRI)
#elif defined(EE_TC_238_ISR) && (EE_TC_238_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_238_ISR, EE_TC_238_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 238)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_238)
#endif /* EE_TC_238_ISR */
#if defined(EE_TC_239_ISR) && (EE_TC_239_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_239_ISR, EE_TC_239_ISR_PRI)
#elif defined(EE_TC_239_ISR) && (EE_TC_239_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_239_ISR, EE_TC_239_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 239)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_239)
#endif /* EE_TC_239_ISR */
#if defined(EE_TC_240_ISR) && (EE_TC_240_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_240_ISR, EE_TC_240_ISR_PRI)
#elif defined(EE_TC_240_ISR) && (EE_TC_240_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_240_ISR, EE_TC_240_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 240)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_240)
#endif /* EE_TC_240_ISR */
#if defined(EE_TC_241_ISR) && (EE_TC_241_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_241_ISR, EE_TC_241_ISR_PRI)
#elif defined(EE_TC_241_ISR) && (EE_TC_241_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_241_ISR, EE_TC_241_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 241)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_241)
#endif /* EE_TC_241_ISR */
#if defined(EE_TC_242_ISR) && (EE_TC_242_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_242_ISR, EE_TC_242_ISR_PRI)
#elif defined(EE_TC_242_ISR) && (EE_TC_242_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_242_ISR, EE_TC_242_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 242)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_242)
#endif /* EE_TC_242_ISR */
#if defined(EE_TC_243_ISR) && (EE_TC_243_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_243_ISR, EE_TC_243_ISR_PRI)
#elif defined(EE_TC_243_ISR) && (EE_TC_243_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_243_ISR, EE_TC_243_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 243)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_243)
#endif /* EE_TC_243_ISR */
#if defined(EE_TC_244_ISR) && (EE_TC_244_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_244_ISR, EE_TC_244_ISR_PRI)
#elif defined(EE_TC_244_ISR) && (EE_TC_244_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_244_ISR, EE_TC_244_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 244)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_244)
#endif /* EE_TC_244_ISR */
#if defined(EE_TC_245_ISR) && (EE_TC_245_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_245_ISR, EE_TC_245_ISR_PRI)
#elif defined(EE_TC_245_ISR) && (EE_TC_245_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_245_ISR, EE_TC_245_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 245)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_245)
#endif /* EE_TC_245_ISR */
#if defined(EE_TC_246_ISR) && (EE_TC_246_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_246_ISR, EE_TC_246_ISR_PRI)
#elif defined(EE_TC_246_ISR) && (EE_TC_246_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_246_ISR, EE_TC_246_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 246)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_246)
#endif /* EE_TC_246_ISR */
#if defined(EE_TC_247_ISR) && (EE_TC_247_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_247_ISR, EE_TC_247_ISR_PRI)
#elif defined(EE_TC_247_ISR) && (EE_TC_247_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_247_ISR, EE_TC_247_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 247)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_247)
#endif /* EE_TC_247_ISR */
#if defined(EE_TC_248_ISR) && (EE_TC_248_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_248_ISR, EE_TC_248_ISR_PRI)
#elif defined(EE_TC_248_ISR) && (EE_TC_248_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_248_ISR, EE_TC_248_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 248)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_248)
#endif /* EE_TC_248_ISR */
#if defined(EE_TC_249_ISR) && (EE_TC_249_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_249_ISR, EE_TC_249_ISR_PRI)
#elif defined(EE_TC_249_ISR) && (EE_TC_249_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_249_ISR, EE_TC_249_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 249)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_249)
#endif /* EE_TC_249_ISR */
#if defined(EE_TC_250_ISR) && (EE_TC_250_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_250_ISR, EE_TC_250_ISR_PRI)
#elif defined(EE_TC_250_ISR) && (EE_TC_250_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_250_ISR, EE_TC_250_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 250)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_250)
#endif /* EE_TC_250_ISR */
#if defined(EE_TC_251_ISR) && (EE_TC_251_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_251_ISR, EE_TC_251_ISR_PRI)
#elif defined(EE_TC_251_ISR) && (EE_TC_251_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_251_ISR, EE_TC_251_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 251)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_251)
#endif /* EE_TC_251_ISR */
#if defined(EE_TC_252_ISR) && (EE_TC_252_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_252_ISR, EE_TC_252_ISR_PRI)
#elif defined(EE_TC_252_ISR) && (EE_TC_252_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_252_ISR, EE_TC_252_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 252)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_252)
#endif /* EE_TC_252_ISR */
#if defined(EE_TC_253_ISR) && (EE_TC_253_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_253_ISR, EE_TC_253_ISR_PRI)
#elif defined(EE_TC_253_ISR) && (EE_TC_253_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_253_ISR, EE_TC_253_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 253)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_253)
#endif /* EE_TC_253_ISR */
#if defined(EE_TC_254_ISR) && (EE_TC_254_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_254_ISR, EE_TC_254_ISR_PRI)
#elif defined(EE_TC_254_ISR) && (EE_TC_254_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_254_ISR, EE_TC_254_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 254)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_254)
#endif /* EE_TC_254_ISR */
#if defined(EE_TC_255_ISR) && (EE_TC_255_ISR_CAT == 2)
EE_ISR2_DEFINITION(EE_TC_255_ISR, EE_TC_255_ISR_PRI)
#elif defined(EE_TC_255_ISR) && (EE_TC_255_ISR_CAT == 1)
EE_ISR1_DEFINITION(EE_TC_255_ISR, EE_TC_255_ISR_PRI)
#elif (EE_TC_MAX_ISR_ID >= 255)
EE_ISR_ENTRY_ALIGN(EE_ISR_PRI_255)
#endif /* EE_TC_255_ISR */

/* Re-enable optimization for DCC */
#if defined(__DCC__)
__asm(".set reorder");
#endif /* Renable optimization */

/* If there are ISR2: Create The Wrapper */
#if defined(EE_MAX_ISR2) && (EE_MAX_ISR2 > 0)

/* If MemMap.h support is enabled (i.e. because memory protection): use it */
#ifdef EE_SUPPORT_MEMMAP_H
#define OS_START_SEC_CODE
#include "MemMap.h"

#else /* EE_SUPPORT_MEMMAP_H */
/* Otherwise put the following in .inttab section (for no real reason:
   I just like it near to interrupt vector) */
#ifdef __GNUC__
#pragma section ".inttab" ax
#elif defined(__TASKING__)
/* Labelled Pragmas are Bugged in TASKING 4.0r1 Compiler */
/* EE_DO_PRAGMA(section code .inttab) */
#elif defined (__DCC__)
#pragma section CODE ".inttab" X
#endif /* __GNUC__ || __TASKING__ || __DCC__ */
#endif /* !EE_SUPPORT_MEMMAP_H */

void EE_TC_INTERRUPT_HANDER EE_tc_isr2_global_wrapper ( EE_tc_ISR_handler f )
{
  EE_ISR2_WRAPPER_BODY(f);

  EE_TC_RFE();
}

/* If MemMap.h support is enabled (i.e. because memory protection): use it */
#ifdef EE_SUPPORT_MEMMAP_H
#define OS_STOP_SEC_CODE
#include "MemMap.h"
#endif /* EE_SUPPORT_MEMMAP_H */

#endif /* EE_MAX_ISR2 */
