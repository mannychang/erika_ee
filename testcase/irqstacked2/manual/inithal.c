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
 * Author: 2001-2002 Paolo Gai
 * CVS: $Id: inithal.c,v 1.5 2006/05/14 18:01:00 pj Exp $
 */

#include "ee.h"

#ifdef __EVALUATOR7T__
#include "cfg_e7t.h"
#endif
#ifdef __TRISCENDA7S__
#include "cfg_triscend.h"
#endif
#ifdef __JANUS__
#include "cfg_janus0.h"
#endif

void thread0(void);
void thread1(void);
void thread2(void);
void thread3(void);

const EE_ADDR EE_hal_thread_body[THREAD_MAX] = {
    (EE_ADDR)thread0,
    (EE_ADDR)thread1,
    (EE_ADDR)thread2,
    (EE_ADDR)thread3
};


#ifdef __MULTI__

struct EE_TOS EE_arm7_system_tos[THREAD_MAX+1] =
{
    {(EE_ADDR)(__SYS_STACK_ADDRESS        ), (EE_ADDR)(__IRQ_STACK_ADDRESS        )}, /* main. MANDATORY! */
    {(EE_ADDR)(__SYS_STACK_ADDRESS-512 -32), (EE_ADDR)(__IRQ_STACK_ADDRESS-512 -32)}, /* thread0 */
    {(EE_ADDR)(__SYS_STACK_ADDRESS-1024-32), (EE_ADDR)(__IRQ_STACK_ADDRESS-1024-32)}, /* thread1 */
    {(EE_ADDR)(__SYS_STACK_ADDRESS-1536-32), (EE_ADDR)(__IRQ_STACK_ADDRESS-1536-32)}, /* thread2 */
    {(EE_ADDR)(__SYS_STACK_ADDRESS-2048-32), (EE_ADDR)(__IRQ_STACK_ADDRESS-2048-32)} /* thread3 */
};


/* stack used only by IRQ handlers */
#ifdef __IRQ_STACK_NEEDED__
struct EE_TOS EE_arm7_IRQ_tos = {
  (EE_ADDR)(__SYS_STACK_ADDRESS-2560), (EE_ADDR)(__IRQ_STACK_ADDRESS-2560)
};
#endif


EE_UREG EE_arm7_thread_tos[THREAD_MAX+1] =
{
    0,				/* main */
    1,				/* thread0 */
    2,				/* thread1 */
    3,				/* thread2 */
    4				/* thread3 */
};


EE_UREG EE_arm7_active_tos = 0; /* main */

#endif /* __MULTI__ */



#ifdef __ARM7GNU__

#ifdef __EVALUATOR7T__

#ifdef __TIMER_1_USED__
#define MYHANDLER myhandler
void myhandler(void);
#else
#define MYHANDLER forever_loop
#endif

void forever_loop(void)
{ 
  for (;;);
}

const EE_ADDR EE_IC_EXTIRQ_vector[22] = {
  (EE_ADDR)forever_loop, /* Ext 0 */
  (EE_ADDR)forever_loop, /* Ext 1 */
  (EE_ADDR)forever_loop, /* Ext 2 */
  (EE_ADDR)forever_loop, /* Ext 3 */
  (EE_ADDR)forever_loop, /* UART0 Tx */
  (EE_ADDR)forever_loop, /* UART0 Rx & error*/
  (EE_ADDR)forever_loop, /* UART1 Tx */
  (EE_ADDR)forever_loop, /* UART1 Rx & error*/
  (EE_ADDR)forever_loop, /* GDMA ch. 0 */
  (EE_ADDR)forever_loop, /* GDMA ch. 1 */
  (EE_ADDR)forever_loop, /* Timer 0 */
  (EE_ADDR)MYHANDLER, /* Timer 1 */
  (EE_ADDR)forever_loop, /* HDLC A Tx */
  (EE_ADDR)forever_loop, /* HDLC A Rx */
  (EE_ADDR)forever_loop, /* HDLC B Tx */
  (EE_ADDR)forever_loop, /* HDLC B Rx */
  (EE_ADDR)forever_loop, /* Ethernet BDMA Tx */
  (EE_ADDR)forever_loop, /* Ethernet BDMA Rx */
  (EE_ADDR)forever_loop, /* Ethernet MAC Tx */
  (EE_ADDR)forever_loop, /* Ethernet MAC Rx */
  (EE_ADDR)forever_loop, /* I2C-bus */
  (EE_ADDR)forever_loop /* No Pending Interrupt */
};




#endif

#if defined(__TRISCENDA7S__)

/* IRQ handlers installation */
#ifdef __TIMER_0_USED__
EE_ADDR EE_timer0_handler_addr = 0;
#endif

#ifdef __TIMER_1_USED__
void myhandler(void);
EE_ADDR EE_timer1_handler_addr = (EE_ADDR)myhandler;
#endif


#ifdef __WATCHDOG_USED__
EE_ADDR EE_watchdog_handler_addr = (EE_ADDR)timer_and_watchdog_handler;
#endif

void fiq_handler(void){
	/*put here the fiq handler */
							}
EE_ADDR EE_fiq_handler_addr = (EE_ADDR)fiq_handler;
#endif

#endif //__ARM7GNU__
