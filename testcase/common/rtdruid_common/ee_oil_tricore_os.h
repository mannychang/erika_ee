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

#ifdef	tricore

EE_OPT = "EE_EXECUTE_FROM_RAM";
#ifdef	tricore_tc29x
EE_OPT = "EE_BYPASS_CLOCK_CONFIGURATION";
#endif	/* tricore_tc29x */

MCU_DATA = TRICORE {
#ifdef	tricore_tc27x
    MODEL = TC27xC;
#endif	/* tricore_tc27x */
#ifdef	tricore_tc29x
    MODEL = TC29x;
#endif	/* tricore_tc29x */
};

CPU_DATA = TRICORE {
    APP_SRC = "code.c";
#ifdef	USEIRQ
    APP_SRC = "../../common/tricore/test_irq.c";
#endif	/* USEIRQ */
    SYS_STACK_SIZE=4096;
#ifdef	tricore_tc29x
    CPU_CLOCK = 100.0;
#endif	/* tricore_tc29x */
#ifdef	tasking
    COMPILER_TYPE = TASKING;
#endif	/* tasking */
#ifdef	gnu
    COMPILER_TYPE = GNU;
#endif	/* gnu */

#endif	/* tricore */
