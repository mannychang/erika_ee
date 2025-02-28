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
 * Author: 2002 Paolo Gai
 * CVS: $Id: code.c,v 1.2 2006/01/05 14:38:25 pj Exp $
 */

/* Test Description:
   This test has 4 tasks with priority th1<th2<th3<th4

   This test uses:
   - (ECC1 or ECC2) + Multistack
   - StartOS
   - ActivateTask into StartupHook (no preemption)
   - TerminateTask on the base level
   - SetEvent/ClearEvent/GetEvent/WaitEvent
   
   here is a trace of the example:
   3: start + WaitEvent
   1: start + Waitevent
   0: start
   0: SetEvent (thread3) - preemption
   3: GetEvent + Clearevent
   3: GetEvent (no events set)
   3: Terminate
   0: Activate 2
   2: SetEvent(thread1) - no preemption
   2: GetEvent(on thread1)
   2: Terminate
   1: GetEvent + Clearevent
   1: GetEvent (no events set)
   1: Terminate
   0: Terminate

*/

#include "ee.h"

#if defined(__JANUS__)
EE_UINT16 fiq_arm0_handler(EE_UINT16 etu0_fir)
{
  return etu0_fir;
}

/* this  was used to program the EIC controller... */
void user_handler1(void)
{
}
#endif

/*
 * Display a number during a short pause 
 */
//#define LOOP_LENGHT 100000
#ifndef __H8__
#define LOOP_LENGHT 10000
#else
#define LOOP_LENGHT 1000
#endif

//#define LOOP_LENGHT 100
//#define LOOP_LENGHT 10
//#define LOOP_LENGHT 1
void pause_and_display(int disp)
{
  int i=0;
  int j=0;

#ifdef __PLAY_NOTE_SUPPORT__
  EE_play_note(_DO0, 15);  
#endif

  for (i=0; i<LOOP_LENGHT; i++) {
#ifdef __LCD_USED__        
    EE_LCD_display(disp);
#endif
    for(j=0; j<10; j++) {
      /* do nothing */
    }
  }
}

#ifdef __OSEKOS_HAS_PRETASKHOOK__
volatile int pretaskcount = 0;
void PreTaskHook(void)
{
  pretaskcount++;
}
#endif

#ifdef __OSEKOS_HAS_POSTTASKHOOK__
volatile int posttaskcount = 0;
void PostTaskHook(void)
{
  posttaskcount++;
}
#endif

#ifdef __OSEKOS_HAS_ERRORHOOK__
volatile int errorhookcount = 0;
void ErrorHook(StatusType Error)
{
  errorhookcount++;
}
#endif

void StartupHook(void)
{
  ActivateTask(thread0);
  ActivateTask(thread1);
  ActivateTask(thread3);
}

void main(void)
{
#ifdef __H8__
    EE_init();
#else
#ifdef __LEDS_USED__
    EE_leds_init();
#endif
#ifdef __LCD_USED__
    EE_LCD_init();
#endif
#endif

  StartOS(OSDEFAULTAPPMODE);
}
