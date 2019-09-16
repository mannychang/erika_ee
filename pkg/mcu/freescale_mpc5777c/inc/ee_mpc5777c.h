/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2011  Evidence Srl
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
 * MPC5777C register map
 *         2014 Francesco Esposito
 */

#ifndef EE_MCU_MPC5777C_H
#define EE_MCU_MPC5777C_H

#if !defined(EE_MM_OPT)
/* ISO int types used by MPC5777C.h */
#define STDINT_SKIP_64BIT
#include "cpu/common/inc/ee_stdint.h"
#endif /* !EE_MM_OPT */

/*#include "MPC5777C.h"*/

#define EE_E200ZX_MAX_EXT_IRQ 512

#if (! defined EE_CURRENTCPU) || (EE_CURRENTCPU == 0)
#define INTC_CORE_OFFSET    0x0U
#define CORE_MASK_SELECTOR  0x00U   /* 00000000 = core 0 */
#else
#define INTC_CORE_OFFSET    0x4U
#define CORE_MASK_SELECTOR  0xC0U   /* 11000000 = core 1 */
#endif
/* INTC Base */
#define INTC_BASE 0xFFF48000U

/* INTC_PSR Base */
#define INTC_PSR_BASE (INTC_BASE + 0x40U)

/* INTC SSCIR Base */
#define INTC_SSCIR_BASE (INTC_BASE + 0x20U)

/* SEMA4 Base */
#define SEMA4_BASE  (0xFFF24000U)

/* SEMA4_GATE Base */
#define SEMA4_GATE_BASE (SEMA4_BASE)

/* INTC base register setup */
#define INTC_CPR (*(EE_UREG *)(INTC_BASE + 0x8U + INTC_CORE_OFFSET))
#define INTC_IACKR (*(EE_UREG *)(INTC_BASE + 0x10U + INTC_CORE_OFFSET))
#define INTC_EOIR (*(EE_UREG *)(INTC_BASE + 0x18U + INTC_CORE_OFFSET))
/* This value is ignored in MPC5777C,
 * it is provided just to guarantee backward compatibility */
#define EE_E200ZX_INTC_CURRPROC CORE_MASK_SELECTOR

#define SET_INT_PRIO(level, pri) \
		((*(volatile EE_UINT8 *) (INTC_PSR_BASE+((EE_UINT32)(level)-16U))) = \
            (volatile EE_UINT8)(CORE_MASK_SELECTOR | (pri)))

#define INTC_SSCIR(n)   (*(EE_UINT8 volatile *)(INTC_SSCIR_BASE + (n)))
#define INTC_PSR(n)     (*(EE_UINT8 volatile *)(INTC_PSR_BASE + (n)))
#define SEMA4_GATE(n)   (*(EE_UINT8 volatile *)(SEMA4_GATE_BASE + (n)))

#ifndef EE_ISR_DYNAMIC_TABLE
void EE_mpc5777c_initialize_external_IRQ_priorities(void);
#endif /* __STATIC_ISR_TABLE__ */

/* SIU_LOCKSTEP register. Used to check if slave core is working in lockstep */
#define SIU_BASE        (0xC3F90000U)
#define SIU_LOCKSTEP    (*(EE_UREG *)(SIU_BASE + 0x9D4U))

__asm void EE_mpc5777c_initMMU(void);

#endif /* EE_MCU_MPC5777C_H */