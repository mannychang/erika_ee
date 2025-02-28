/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2008  Evidence Srl
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
 * Author: 2004 Paolo Gai
 * CVS: $Id: ee_inline.h,v 1.3 2006/04/08 21:15:23 pj Exp $
 */

/* This file is ONLY included when we are NOT compiling a library that
   will be used in BINARY DISTRIBUTIONS */

#ifndef PKG_KERNEL_OO_INC_EE_OO_INLINE_H
#define PKG_KERNEL_OO_INC_EE_OO_INLINE_H

/***************************************************************************
 * 13.2 Task management 
 ***************************************************************************/

/***************************************************************************
 * 13.3 Interrupt handling 
 ***************************************************************************/

/***************************************************************************
 * The simbol EE_OLD_HAL marks architecture that doesn't not implement new
 * HAL APIs (MUST be defined in the header ee_cpu.h of these architectures)
 ***************************************************************************/
#if (!defined(EE_OLD_HAL))
/* 13.3.2.1: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_ENABLEALLINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_EnableAllInterrupts(void)
{
  /* I begin with the suspend for atomicity. */
  register EE_FREG const temp_suspend = EE_hal_suspendIRQ();
  EE_ORTI_set_service_in(EE_SERVICETRACE_ENABLEALLINTERRUPTS);

  /* [OS299]: If EnableAllInterrupts()/ResumeAllInterrupts()/
      ResumeOSInterrupts() are called and no corresponding
      DisableAllInterupts()/SuspendAllInterrupts()/SuspendOSInterrupts()
      was done before, the Operating System shall not perform this OS
      service. */
  if ( EE_oo_IRQ_disable_count > 0U )
  {
    --EE_oo_IRQ_disable_count;
    if ( EE_oo_IRQ_disable_count == 0U )
    {
      /* Stop DisableAllInterrupts TP budget, if needed */
      EE_as_tp_active_pause_and_update_budgets();
      EE_as_tp_active_stop_budget(EE_ALL_INTERRUPT_LOCK_BUDGET,
        INVALID_OBJECTID, EE_TRUE);
      EE_as_tp_active_update_budgets_and_restart();

      EE_ORTI_set_service_out(EE_SERVICETRACE_ENABLEALLINTERRUPTS);
      EE_hal_enableIRQ();
    } else {
      EE_ORTI_set_service_out(EE_SERVICETRACE_ENABLEALLINTERRUPTS);
    }
  } else {
    /* Revert What I did */
    EE_ORTI_set_service_out(EE_SERVICETRACE_ENABLEALLINTERRUPTS);
    EE_hal_resumeIRQ(temp_suspend);
  }
}
#endif

/* 13.3.2.2: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_DISABLEALLINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_DisableAllInterrupts(void)
{
  /* I begin with the disable for atomicity. */
  EE_hal_disableIRQ();
  EE_ORTI_set_service_in(EE_SERVICETRACE_DISABLEALLINTERRUPTS);

  ++EE_oo_IRQ_disable_count;

  /* Enable DisableAllInterrupts TP budget, if needed */
  if ( EE_oo_IRQ_disable_count == 1U ) {
    EE_as_tp_active_pause_and_update_budgets();
    EE_as_tp_active_activate_budget(EE_ALL_INTERRUPT_LOCK_BUDGET,
      INVALID_OBJECTID, EE_TRUE);
    EE_as_tp_active_update_budgets_and_restart();
  }

  EE_ORTI_set_service_out(EE_SERVICETRACE_DISABLEALLINTERRUPTS);
}
#endif /* ! __PRIVATE_DISABLEALLINTERRUPTS__ && ! __EE_MEMORY_PROTECTION__ */

/* 13.3.2.3: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_RESUMEALLINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_ResumeAllInterrupts(void)
{
  /* I begin with the suspend for atomicity. */
  register EE_FREG const temp_suspend = EE_hal_suspendIRQ();
  EE_ORTI_set_service_in(EE_SERVICETRACE_RESUMEALLINTERRUPTS);

  /* [OS299]: If EnableAllInterrupts()/ResumeAllInterrupts()/
      ResumeOSInterrupts() are called and no corresponding
      DisableAllInterupts()/SuspendAllInterrupts() / SuspendOSInterrupts()
      was done before, the Operating System shall not perform this OS
      service. */
  if ( EE_oo_IRQ_disable_count > 0U )
  {
    --EE_oo_IRQ_disable_count;
    if ( EE_oo_IRQ_disable_count == 0U )
    {
      /* Stop DisableAllInterrupts TP budget, if needed */
      EE_as_tp_active_pause_and_update_budgets();
      EE_as_tp_active_stop_budget(EE_ALL_INTERRUPT_LOCK_BUDGET,
        INVALID_OBJECTID, EE_TRUE);
#if (!defined(EE_REALLY_HANDLE_OS_IRQ))
      /* XXX: Work Around to the actual ISR locking implementation, both
              ALL and OS budgets are enabled */
      EE_as_tp_active_stop_budget(EE_OS_INTERRUPT_LOCK_BUDGET,
        INVALID_OBJECTID, EE_TRUE);
#endif /* !EE_REALLY_HANDLE_OS_IRQ */
      EE_as_tp_active_update_budgets_and_restart();

      EE_ORTI_set_service_out(EE_SERVICETRACE_RESUMEALLINTERRUPTS);
      EE_hal_resumeIRQ(EE_oo_IRQ_suspend_status);
    } else {
      EE_ORTI_set_service_out(EE_SERVICETRACE_RESUMEALLINTERRUPTS);
    }
  } else {
    /* Revert What I did */
    EE_ORTI_set_service_out(EE_SERVICETRACE_RESUMEALLINTERRUPTS);
    EE_hal_resumeIRQ(temp_suspend);
  }
}
#endif /* ! __PRIVATE_RESUMEALLINTERRUPTS__ && ! __EE_MEMORY_PROTECTION__ */

/* 13.3.2.4: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_SUSPENDALLINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_SuspendAllInterrupts(void)
{
  /* I begin with the suspend for atomicity. */
  register EE_FREG const temp_suspend  = EE_hal_suspendIRQ();
  EE_ORTI_set_service_in(EE_SERVICETRACE_SUSPENDALLINTERRUPTS);

  /* Increment disabling counter */
  ++EE_oo_IRQ_disable_count;

  /* Check if this is the first time that a Disable/Suspend function is called
     by this TASK */
  if ( EE_oo_IRQ_disable_count == 1U )
  {
    EE_oo_IRQ_suspend_status = temp_suspend;
    /* Enable DisableAllInterrupts TP budget, if needed */
    EE_as_tp_active_pause_and_update_budgets();
#if (!defined(EE_REALLY_HANDLE_OS_IRQ))
      /* XXX: Work Around to the actual ISR locking implementation, enable both
              ALL and OS budgets */
      EE_as_tp_active_activate_budget(EE_OS_INTERRUPT_LOCK_BUDGET,
        INVALID_OBJECTID, EE_TRUE);
#endif /* !EE_REALLY_HANDLE_OS_IRQ */
    EE_as_tp_active_activate_budget(EE_ALL_INTERRUPT_LOCK_BUDGET,
      INVALID_OBJECTID, EE_TRUE);
    EE_as_tp_active_update_budgets_and_restart();
  }

  EE_ORTI_set_service_out(EE_SERVICETRACE_SUSPENDALLINTERRUPTS);
}
#endif /* ! __PRIVATE_SUSPENDALLINTERRUPTS__ && ! __EE_MEMORY_PROTECTION__ */

#if (defined(EE_REALLY_HANDLE_OS_IRQ))
/* 13.3.2.5: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_RESUMEOSINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_ResumeOSInterrupts(void)
{
  /* I begin with the suspend for atomicity. */
  register EE_FREG const temp_suspend = EE_hal_suspend_OsIRQ();
  EE_ORTI_set_service_in(EE_SERVICETRACE_RESUMEOSINTERRUPTS);

  /* [OS299]: If EnableAllInterrupts()/ResumeAllInterrupts()/
      ResumeOSInterrupts() are called and no corresponding
      DisableAllInterupts()/SuspendAllInterrupts() / SuspendOSInterrupts()
      was done before, the Operating System shall not perform this OS
      service. */
  if ( EE_oo_OS_IRQ_suspend_count > 0U )
  {
    --EE_oo_OS_IRQ_suspend_count;
    if ( EE_oo_OS_IRQ_suspend_count == 0U )
    {
      /* Stop SuspendOSInterrupts TP budget, if needed */
      EE_as_tp_active_pause_and_update_budgets();
      EE_as_tp_active_stop_budget(EE_OS_INTERRUPT_LOCK_BUDGET,
        INVALID_OBJECTID, EE_TRUE);
      EE_as_tp_active_update_budgets_and_restart();

      EE_ORTI_set_service_out(EE_SERVICETRACE_RESUMEOSINTERRUPTS);
      EE_hal_resume_OsIRQ(EE_oo_OS_IRQ_suspend_status);
    } else {
      EE_ORTI_set_service_out(EE_SERVICETRACE_RESUMEOSINTERRUPTS);
    }
  } else {
    /* Revert What I did */
    EE_ORTI_set_service_out(EE_SERVICETRACE_RESUMEOSINTERRUPTS);
    EE_hal_resume_OsIRQ(temp_suspend);
  }
}
#endif /* ! __PRIVATE_RESUMEOSINTERRUPTS__ && ! __EE_MEMORY_PROTECTION__ */

/* 13.3.2.6: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_SUSPENDOSINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_SuspendOSInterrupts(void)
{
  /* I begin with the suspend for atomicity. */
  register EE_FREG const temp_suspend = EE_hal_suspend_OsIRQ();
  EE_ORTI_set_service_in(EE_SERVICETRACE_SUSPENDOSINTERRUPTS);

  /* Increment disabling counter */
  ++EE_oo_OS_IRQ_suspend_count;

  /* Check if this is the first time that a SuspendOSInterrupts is called
     by this TASK */
  if ( EE_oo_OS_IRQ_suspend_count == 1U ) {
    EE_oo_OS_IRQ_suspend_status = temp_suspend;
    /* Enable SuspendOSInterrupts TP budget, if needed */
    EE_as_tp_active_pause_and_update_budgets();
    EE_as_tp_active_activate_budget(EE_OS_INTERRUPT_LOCK_BUDGET,
      INVALID_OBJECTID, EE_TRUE);

    EE_as_tp_active_update_budgets_and_restart();
  }

  EE_ORTI_set_service_out(EE_SERVICETRACE_SUSPENDOSINTERRUPTS);
}
#endif /* ! __PRIVATE_SUSPENDOSINTERRUPTS__ && ! __EE_MEMORY_PROTECTION__ */

#else /* EE_REALLY_HANDLE_OS_IRQ */

/* 13.3.2.5: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_RESUMEOSINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_ResumeOSInterrupts(void)
{
  /* I begin with the suspend for atomicity. */
  register EE_FREG const temp_suspend = EE_hal_suspendIRQ();
  EE_ORTI_set_service_in(EE_SERVICETRACE_RESUMEOSINTERRUPTS);

  /* [OS299]: If EnableAllInterrupts()/ResumeAllInterrupts()/
      ResumeOSInterrupts() are called and no corresponding
      DisableAllInterupts()/SuspendAllInterrupts() / SuspendOSInterrupts()
      was done before, the Operating System shall not perform this OS
      service. */
  if ( EE_oo_IRQ_disable_count > 0U )
  {
    --EE_oo_IRQ_disable_count;
    if ( EE_oo_IRQ_disable_count == 0U )
    {
      /* Stop DisableAllInterrupts TP budget, if needed */
      EE_as_tp_active_pause_and_update_budgets();
      EE_as_tp_active_stop_budget(EE_ALL_INTERRUPT_LOCK_BUDGET,
        INVALID_OBJECTID, EE_TRUE);
      /* XXX: Work Around to the actual ISR locking implementation, both
              ALL and OS budgets are enabled */
      EE_as_tp_active_stop_budget(EE_OS_INTERRUPT_LOCK_BUDGET,
        INVALID_OBJECTID, EE_TRUE);
      EE_as_tp_active_update_budgets_and_restart();

      EE_hal_resumeIRQ(EE_oo_IRQ_suspend_status);
    }
  } else {
    /* Revert What I did */
    EE_hal_resumeIRQ(temp_suspend);
  }

  EE_ORTI_set_service_out(EE_SERVICETRACE_RESUMEOSINTERRUPTS);
}
#endif /* ! __PRIVATE_RESUMEOSINTERRUPTS__ && ! __EE_MEMORY_PROTECTION__ */

/* 13.3.2.6: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_SUSPENDOSINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_SuspendOSInterrupts(void)
{
  /* I begin with the suspend for atomicity. */
  register EE_FREG const temp_suspend = EE_hal_suspendIRQ();
  EE_ORTI_set_service_in(EE_SERVICETRACE_SUSPENDOSINTERRUPTS);

  /* Increment disabling counter */
  ++EE_oo_IRQ_disable_count;
  /* Check if this is the first time that a Disable/Suspend function is called
     by this TASK */
  if ( EE_oo_IRQ_disable_count == 1U ) {
    EE_oo_IRQ_suspend_status = temp_suspend;
    /* Enable DisableAllInterrupts TP budget, if needed */
    EE_as_tp_active_pause_and_update_budgets();
    EE_as_tp_active_activate_budget(EE_ALL_INTERRUPT_LOCK_BUDGET,
      INVALID_OBJECTID, EE_TRUE);
    /* XXX: Work Around to the actual ISR locking implementation, enable both
            ALL and OS budgets */
    EE_as_tp_active_activate_budget(EE_OS_INTERRUPT_LOCK_BUDGET,
      INVALID_OBJECTID, EE_TRUE);
    EE_as_tp_active_update_budgets_and_restart();
  }

  EE_ORTI_set_service_out(EE_SERVICETRACE_SUSPENDOSINTERRUPTS);
}
#endif /* ! __PRIVATE_SUSPENDOSINTERRUPTS__ && ! __EE_MEMORY_PROTECTION__ */
#endif /* EE_REALLY_HANDLE_OS_IRQ */

#else /* !!! OLD INTERRUPT HANDLING PRIMITIVES !!! */

/* 13.3.2.1: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_ENABLEALLINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_EnableAllInterrupts(void)
{
  EE_ORTI_set_service_in(EE_SERVICETRACE_ENABLEALLINTERRUPTS);

  EE_hal_enableIRQ();

  EE_ORTI_set_service_out(EE_SERVICETRACE_ENABLEALLINTERRUPTS);
}
#endif

/* 13.3.2.2: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_DISABLEALLINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_DisableAllInterrupts(void)
{
  EE_ORTI_set_service_in(EE_SERVICETRACE_DISABLEALLINTERRUPTS);

  EE_hal_disableIRQ();

  EE_ORTI_set_service_out(EE_SERVICETRACE_DISABLEALLINTERRUPTS);
}
#endif /* ! __PRIVATE_DISABLEALLINTERRUPTS__ */

/* 13.3.2.3: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_RESUMEALLINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_ResumeAllInterrupts(void)
{
  EE_ORTI_set_service_in(EE_SERVICETRACE_RESUMEALLINTERRUPTS);

  EE_oo_IRQ_disable_count--;
  if (EE_oo_IRQ_disable_count == 0U) {
    EE_hal_enableIRQ();
  }

  EE_ORTI_set_service_out(EE_SERVICETRACE_RESUMEALLINTERRUPTS);
}
#endif /* ! __PRIVATE_RESUMEALLINTERRUPTS__ */

/* 13.3.2.4: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_SUSPENDALLINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_SuspendAllInterrupts(void)
{
  EE_ORTI_set_service_in(EE_SERVICETRACE_SUSPENDALLINTERRUPTS);

  if (EE_oo_IRQ_disable_count == 0U) {
    EE_hal_disableIRQ();
  }
  EE_oo_IRQ_disable_count++;

  EE_ORTI_set_service_out(EE_SERVICETRACE_SUSPENDALLINTERRUPTS);
}
#endif /* ! __PRIVATE_SUSPENDALLINTERRUPTS__ */

/* 13.3.2.5: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_RESUMEOSINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_ResumeOSInterrupts(void)
{
  EE_ORTI_set_service_in(EE_SERVICETRACE_RESUMEOSINTERRUPTS);

  EE_oo_IRQ_disable_count--;
  if (EE_oo_IRQ_disable_count == 0U) {
    EE_hal_enableIRQ();
  }

  EE_ORTI_set_service_out(EE_SERVICETRACE_RESUMEOSINTERRUPTS);
}
#endif /* ! __PRIVATE_RESUMEOSINTERRUPTS__ */

/* 13.3.2.6: BCC1, BCC2, ECC1, ECC2 */
#ifndef __PRIVATE_SUSPENDOSINTERRUPTS__
__INLINE__ void __ALWAYS_INLINE__ EE_oo_SuspendOSInterrupts(void)
{
  EE_ORTI_set_service_in(EE_SERVICETRACE_SUSPENDOSINTERRUPTS);

  if (EE_oo_IRQ_disable_count == 0U) {
    EE_hal_disableIRQ();
  }
  EE_oo_IRQ_disable_count++;

  EE_ORTI_set_service_out(EE_SERVICETRACE_SUSPENDOSINTERRUPTS);
}
#endif /* ! __PRIVATE_SUSPENDOSINTERRUPTS__ */

#endif /* EE_OLD_HAL */

#endif /* __INCLUDE_OO_INLINE_H__ */
