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

/*
 * CPU-dependent part of HAL that have to be seen by user code
 * Author: 2012 Errico Guidieri
 */

#ifndef PKG_CPU_E200ZX_INC_EE_CPU_OS_H
#define PKG_CPU_E200ZX_INC_EE_CPU_OS_H

/* All defines needed by kernel too */
#include "eecfg.h"

/*************************************************************************
 HAL Types and structures
 *************************************************************************/

#ifdef __GNUC__
#include "cpu/common/inc/ee_compiler_gcc.h"
#define EE_SUPPORT_MEMMAP_H
#elif defined (__DCC__)
#include "cpu/common/inc/ee_compiler_diab.h"
#define EE_SUPPORT_MEMMAP_H
#elif defined (__CWCC__)
#include "cpu/common/inc/ee_compiler_codewarrior.h"
#else
#error Unsupported compiler
#endif

/* Primitive data types */
#include "cpu/common/inc/ee_types.h"

/* Define HAL types */
#include "cpu/e200zx/inc/ee_e200zx_types.h"

/* Use the "standard" implementation 
  (I need to put this here because it needs to see HAL types) */
#include "cpu/common/inc/ee_hal_structs.h"

/* defines used both by C and assembly */
#include "ee_cpu_asm.h"

/* INTC symbols */
#include "ee_mcu_regs.h"

/*
                        !!! WARNING WORK AROUND !!!

    To not include frescale file to access INTC register.

    I include only address (used usually in asm files) and I access registers
    by them.

    I don't want put in OS user interface freescale MCU file because
    integration problem, but I still need to access those register in ISR wrap
    code generated by ISRx macros.

    The real solution WOULD be break ISRx wrapper declaration and ISRx wrapper
    definition in two different macros and move the later in internal.

    And let RT-Druid declare ISR wrappers in eecfg.c according to the
    configuration.

    Furthermore this solution would let move a lot of other code in internal
    too.
 */
#include "ee_mcu_regs_asm.h"

/******************************************************************************/
/*            INTERRUPT CONTROL REGISTERS ACCESS MACROS                      */
/******************************************************************************/
#define EE_INTC_MCR       (*((volatile EE_UREG*)(INTC_MCR_ADDR)))
#define EE_INTC_IACKR     (*((volatile EE_UREG*)(INTC_IACKR_ADDR)))
#define EE_INTC_EOIR      (*((volatile EE_UREG*)(INTC_EOIR_ADDR)))

/* Thread IDs */
typedef EE_INT32 EE_TID;

/* Thread IDs - unsigned version*/
typedef EE_UINT32 EE_UTID;

/* ISR Priority representation type */
typedef EE_UREG EE_TYPEISR2PRIO;

/* Flag (OR'ed to an EE_TID) to mark a task as stacked. */
#define TID_IS_STACKED_MARK	0x80000000U

/* Remote TID, to be used with Remote notifications */
#define EE_REMOTE_TID 0x80000000U

/* Type pointing to an ISR (Here because ISR macro code generation) */
typedef void (*EE_e200z7_ISR_handler)(void);

/* ORTI types */
typedef EE_e200z7_ISR_handler EE_ORTI_runningisr2_type;
#ifdef __OO_ORTI_RUNNINGISR2__
__INLINE__ EE_ORTI_runningisr2_type __ALWAYS_INLINE__
EE_ORTI_build_isr2id(EE_ORTI_runningisr2_type orti_isr2id) {
  return orti_isr2id;
}
#else /* __OO_ORTI_RUNNINGISR2__ */
__INLINE__ EE_ORTI_runningisr2_type __ALWAYS_INLINE__
EE_ORTI_build_isr2id(EE_ORTI_runningisr2_type orti_isr2id) {
  return (EE_ORTI_runningisr2_type)0;
}
#endif /* else __OO_ORTI_RUNNINGISR2__ */

#ifdef __MULTI__
/***************************************************************************
   These internal structures must be seen by the user because are used by
   the context switch functions that are inline and are used by common irq
   stub
 ***************************************************************************/

/* Top of each private stack. */
extern struct EE_TOS EE_e200z7_system_tos[EE_E200Z7_SYSTEM_TOS_SIZE];

/* Index of the current stack. */
extern EE_UREG EE_e200z7_active_tos;

/* Used in common context */
#define EE_hal_active_tos EE_e200z7_active_tos
#endif /* __MULTI__ */

/*************************************************************************
                          Utility Functions
       (Have to be here because ISR macro code generation use it)
 *************************************************************************/

__INLINE__ void __ALWAYS_INLINE__ EE_e200zx_mbar(void)
{
	__asm volatile("mbar 0\n");
}

__INLINE__ void __ALWAYS_INLINE__ EE_e200zx_mbar_1(void)
{
	__asm volatile("mbar 1\n");
}

__INLINE__ void __ALWAYS_INLINE__ EE_e200zx_msync(void)
{
	__asm volatile("msync\n");
}

__INLINE__ void __ALWAYS_INLINE__ EE_e200zx_isync(void)
{
	__asm volatile ("isync");
}

/*********************************************************************
                  E200Z7 interrupt disabling/enabling
 (These should stay in ee_cpu.h but because inline Kernel service I 
  must put them here)
 *********************************************************************/

#define MSR_EE  ((EE_FREG)1U << 15U)
#define MSR_PR  ((EE_FREG)1U << 14U)

__INLINE__ void __ALWAYS_INLINE__ EE_e200z7_enableIRQ(void)
{
	__asm volatile ("wrteei 1\n");
}

__INLINE__ void __ALWAYS_INLINE__ EE_e200z7_disableIRQ(void)
{
	__asm volatile ("wrteei 0\n");
}

#ifdef	__DCC__
__asm volatile static void EE_e200z7_resumeIRQ(EE_FREG msr)
{
% reg msr
	mtmsr msr
}
#else
__INLINE__ void __ALWAYS_INLINE__ EE_e200z7_resumeIRQ(EE_FREG msr)
{
	__asm volatile ("mtmsr %0\n" :: "r"(msr));
}
#endif

#ifdef	__DCC__
__asm volatile static EE_FREG EE_e200z7_suspendIRQ(void)
{
! "r3"
	mfmsr	r3
	wrteei	0
}
#else
__INLINE__ EE_FREG __ALWAYS_INLINE__ EE_e200z7_suspendIRQ(void)
{
  EE_FREG msr;

  __asm volatile ("mfmsr %0   \n"
      "wrteei 0\n"
      : "=r"(msr));
  return msr;
}
#endif

/* FIXME: In Erika HAL doesn't exit a method to check IRQ status! */
#ifdef __DCC__
__asm volatile static EE_FREG EE_e200z7_isIRQEnabled(void)
{
! "r3"
  mfmsr	r3
  /* This istruction has been retro-engineered looking at the code generated
     by CodeWarrior compiler! */
  rlwinm   r3,r3,0x11,0x1F,0x1F
}
#else
__INLINE__ EE_TYPEBOOL __ALWAYS_INLINE__ EE_e200z7_isIRQEnabled(void)
{
  EE_FREG msr;

  __asm volatile ("mfmsr %0   \n"
      : "=r"(msr));
  return ((msr & MSR_EE) != 0U);
}
#endif

__INLINE__ EE_TYPEISR2PRIO __ALWAYS_INLINE__ EE_e200zx_get_int_prio(void)
{
	return INTC_CPR;
}

__INLINE__ void __ALWAYS_INLINE__ EE_e200zx_set_int_prio(EE_TYPEISR2PRIO prio)
{
  /* To understand why all this synchronization is needed look at one of
     frescale ppc's reference manual at the following:
     9.3.1.2 INTC Current Priority Register NOTE
     9.5.5.2 Ensuring Coherency
  */
  /* Execution syncronization -> all stores executed (for coherency: 9.5.5.2) */
  EE_e200zx_mbar();
  INTC_CPR = prio;
  /* Context syncronization + INTC_CPR store executed */
  EE_e200zx_mbar();
  EE_e200zx_isync();
}

/*********************************************************************
             E200Z7 mode user/supervisor switch functions
 *********************************************************************/

#ifdef __DCC__
__asm volatile static void EE_e200z7_switch_to_user_mode(void)
{
! "r3"
  mfmsr r3
  ori   r3, r3, (1U << 14U)
  mtmsr r3
}

__asm volatile static void EE_e200z7_switch_to_supervisor_mode(void)
{
! "r3"
  mfmsr r3
  andi  r3, r3, 0xBFFFU
  mtmsr r3
}

#else

#define READ_MSR(msr) \
  do { \
    __asm volatile ("mfmsr %0 \n" \
                    : "=r"(msr)); \
  } while (0)

#define WRITE_MSR(msr) \
  do { \
  __asm volatile ("mtmsr %0   \n" \
		  :: "r"(msr));	  \
  } while (0)

__INLINE__ void EE_e200z7_switch_to_user_mode(void)
{
  EE_FREG msr;
  READ_MSR(msr);
  msr |= MSR_PR;
  WRITE_MSR(msr);
}


__INLINE__  void EE_e200z7_switch_to_supervisor_mode(void)
{
  EE_FREG msr;
  READ_MSR(msr);
  msr &= ~MSR_PR;
  WRITE_MSR(msr);
}
#endif

/**************************************************************************
 *                          Interrupt Handling
 * Even though the following are hal so they should stay in internal, but
 * they must be visible by user because they are used by common irq stub
 * and in inline kernel services.
 **************************************************************************/

/* ISR priority level defines */
#define EE_ISR_UNMASKED 0U
#define EE_ISR_PRI_1  1U
#define EE_ISR_PRI_2  2U
#define EE_ISR_PRI_3  3U
#define EE_ISR_PRI_4  4U
#define EE_ISR_PRI_5  5U
#define EE_ISR_PRI_6  6U
#define EE_ISR_PRI_7  7U
#define EE_ISR_PRI_8  8U
#define EE_ISR_PRI_9  9U
#define EE_ISR_PRI_10 10U
#define EE_ISR_PRI_11 11U
#define EE_ISR_PRI_12 12U
#define EE_ISR_PRI_13 13U
#define EE_ISR_PRI_14 14U
#define EE_ISR_PRI_15 15U

/* Disable/Enable Interrupts */
__INLINE__ void __ALWAYS_INLINE__ EE_hal_enableIRQ(void)
{
	EE_e200z7_enableIRQ();
}

__INLINE__ void __ALWAYS_INLINE__ EE_hal_disableIRQ(void)
{
	EE_e200z7_disableIRQ();
}

__INLINE__ void __ALWAYS_INLINE__ EE_hal_resumeIRQ(EE_FREG flags)
{
	EE_e200z7_resumeIRQ(flags);
}

__INLINE__ EE_FREG __ALWAYS_INLINE__ EE_hal_suspendIRQ(void)
{
	return EE_e200z7_suspendIRQ();
}

/* With the following Macro we declare that this cpu support really handle OS
   interrupts so the Suspend/ResumeOSInterrupts can avoid to disable all
   interrupts */
#define EE_REALLY_HANDLE_OS_IRQ

__INLINE__ EE_FREG __ALWAYS_INLINE__ EE_hal_suspend_OsIRQ ( void )
{
  register EE_TYPEISR2PRIO actual_prio = EE_e200zx_get_int_prio();

  if ( actual_prio < EE_MAX_ISR2_PRI )
  {
    /* Set the new priority */
    EE_e200zx_set_int_prio(EE_MAX_ISR2_PRI);
  }
  return (EE_FREG)actual_prio;
}

__INLINE__ void __ALWAYS_INLINE__ EE_hal_resume_OsIRQ ( EE_FREG flags )
{
  EE_e200zx_set_int_prio((EE_TYPEISR2PRIO)flags);
}

#if (defined(__EE_MEMORY_PROTECTION__))

#ifdef EE_SUPPORT_MEMMAP_H
#define OS_START_SEC_VAR_NOINIT
#include "MemMap.h"
#endif /* EE_SUPPORT_MEMMAP_H */
extern EE_FREG EE_e200zx_mp_suspend_os_pri;
#ifdef EE_SUPPORT_MEMMAP_H
#define OS_STOP_SEC_VAR_NOINIT
#include "MemMap.h"
#endif /* EE_SUPPORT_MEMMAP_H */

#define EE_hal_set_irq_valid_flag(f) ((f) | (MSR_EE << 1))
#define EE_hal_clear_irq_flag(f) ((f) & ~MSR_EE)
#define EE_hal_copy_irq_flag(from, to) (((to) & ~MSR_EE) | ((from) & MSR_EE))

__INLINE__ EE_FREG __ALWAYS_INLINE__
  EE_hal_set_max_isr2_pri_flag ( EE_FREG next )
{
  EE_e200zx_mp_suspend_os_pri = EE_hal_suspend_OsIRQ();
  return next;
}

__INLINE__ EE_FREG __ALWAYS_INLINE__
  EE_hal_copy_pri_flag ( EE_FREG from, EE_FREG to )
{
  EE_hal_resume_OsIRQ(EE_e200zx_mp_suspend_os_pri);
  return EE_hal_copy_irq_flag(from, to);
}

#endif /* __EE_MEMORY_PROTECTION__ */

/*************************************************************************
                CPU-dependent ORT support (mainly OTM)
               (here because ISR macro code generation)
 *************************************************************************/

/* Probably, some parts of the OTM code below does not depend on the
 * architecture.  They should be moved somewhere into pkg/cpu/common if this
 * turns out to be the case. */

#define EE_ORTI_OTM_ID_RUNNINGISR2 1
#define EE_ORTI_OTM_ID_SERVICETRACE 2

#ifdef __OO_ORTI_USE_OTM__
void EE_e200zx_send_otm8(EE_UINT8 id, EE_UINT8 data);
void EE_e200zx_send_otm32(EE_UINT8 id, EE_UINT32 data);

#else /* if __OO_ORTI_USE_OTM__ */
/* OTM disabled */
#define EE_e200zx_send_otm8(id, data)   ((void)0)
#define EE_e200zx_send_otm32(id, data)  ((void)0)
#endif /* else __OO_ORTI_USE_OTM__ */

#if (defined(__OO_ORTI_RUNNINGISR2__)) && (defined(__OO_ORTI_USE_OTM__))
__INLINE__ void EE_ORTI_send_otm_runningisr2(EE_ORTI_runningisr2_type isr2)
{
	EE_e200zx_send_otm32(EE_ORTI_OTM_ID_RUNNINGISR2, (EE_UINT32)isr2);
}
#else /* __OO_ORTI_RUNNINGISR2__ && __OO_ORTI_USE_OTM__ */
#define EE_ORTI_send_otm_runningisr2(isr2)    ((void)0)
#endif /* __OO_ORTI_RUNNINGISR2__ && __OO_ORTI_USE_OTM__ */

#ifdef __OO_ORTI_SERVICETRACE__
__INLINE__ void EE_ORTI_send_otm_servicetrace(EE_UINT8 srv)
{
	EE_e200zx_send_otm8(EE_ORTI_OTM_ID_SERVICETRACE, srv);
}

#if defined(__EE_MEMORY_PROTECTION__)
#if (defined(RTDRUID_CONFIGURATOR_NUMBER)) \
 && (RTDRUID_CONFIGURATOR_NUMBER >= RTDRUID_CONFNUM_ORTI_SERVICE_API)
#if defined __DCC__
__asm volatile static void EE_ORTI_ext_set_service(EE_UINT8 srv)
{
% reg srv
! "r0","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","ctr"
	mr	r3, srv
	li	r0, EE_ID_ORTI_ext_set_service
	sc
}
#endif /* __DCC__ */
#else /* RTDRUID_CONFNUM_ORTI_SERVICE_API */
__INLINE__ void EE_ORTI_ext_set_service(EE_UINT8 srv)
{
}
#endif /* else RTDRUID_CONFNUM_ORTI_SERVICE_API */
#endif /* __EE_MEMORY_PROTECTION__ */

#endif /* __OO_ORTI_SERVICETRACE__ */

#endif /* INCLUDE_E200ZX_EE_CPU_OS_H__ */

