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

#ifndef EE_MMU_H
#define EE_MMU_H

#ifdef __VLE__
#define VLE_FLAG 0x20
#else
#define VLE_FLAG 0x0
#endif

/*
 * Place here your MMU configuration if
 * you don't use the BAM MMU configuration
 */
#define TLB0_MAS0 0x00010000
#define TLB0_MAS1 0xC0000700
#define TLB0_MAS2 (0x00000000 | VLE_FLAG)
#define TLB0_MAS3 0x0000003F

#define TLB1_MAS0 0x00040000
#define TLB1_MAS1 0xC0000580
#define TLB1_MAS2 0xC3E0000A
#define TLB1_MAS3 0xC3E0003F

#define TLB2_MAS0 0x00020000
#define TLB2_MAS1 0xC0000700
#define TLB2_MAS2 (0x20000000 | VLE_FLAG)
#define TLB2_MAS3 0x0000003F

#define TLB3_MAS0 0x00030000
#define TLB3_MAS1 0xC0000480
/* cache inhibited, memory coherence enabled, guarded */
#define TLB3_MAS2 (0x4000000E | VLE_FLAG)
#define TLB3_MAS3 0x4000003F

#define TLB4_MAS0 0x00000000
#define TLB4_MAS1 0xC0000580
#define TLB4_MAS2 0xFFE0000A
#define TLB4_MAS3 0xFFE0003F

#endif  /* EE_MMU_H */

