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
 * CVS: $Id: threads.c,v 1.1 2005/07/17 13:10:52 pj Exp $
 */

#include "ee.h"
#include "test/assert/inc/ee_assert.h"

void pause_and_display(int disp);
void mytimer_start(void);
extern volatile int cycle;

TASK(thread0)
{
    EE_assert_range(0, 1, 21);

    EE_assert_last();

#ifdef __LEDS_USED__
    EE_led_1_on();
#endif

#ifdef __LCD_USED__
    pause_and_display(0x0);
#endif

#ifdef __LEDS_USED__
    EE_led_1_off();
#endif

    TerminateTask();
}

TASK(thread1)
{
    EE_assert(4, 1, 3);
#ifdef __LEDS_USED__
    EE_led_2_on();
#endif

#ifdef __LCD_USED__
    pause_and_display(0x1);
#endif

    GetResource(myres);

    EE_assert(5, 1, 4);

    mytimer_start();

    EE_assert(7, 1, 6);

    cycle=1;
    while (cycle);

    EE_assert(10, 1, 9);

    //irq without preemption
    
    ReleaseResource(myres);

    EE_assert(12, 1, 11);

    //(preemption)
#ifdef __LCD_USED__
    pause_and_display(0x1);
#endif

    //timer reprogramming
    //lock
    //second cycle
    // irq with preemption
    //unlock

#ifdef __LCD_USED__
    pause_and_display(0x1);
#endif

    GetResource(myres);

    EE_assert(13, 1, 12);

    mytimer_start();

    EE_assert(15, 1, 14);

    cycle=1;
    while (cycle);

    //irq with preemption

    EE_assert(20, 1, 19);

#ifdef __LCD_USED__
    pause_and_display(0x1);
#endif    

    ReleaseResource(myres);

    EE_assert(21, 1, 20);

#ifdef __LEDS_USED__
    EE_led_2_off();
#endif

    TerminateTask();
}

TASK(thread2)
{
    EE_assert(11, 1, 10);

#ifdef __LEDS_USED__
    EE_led_3_on();
#endif

#ifdef __LCD_USED__
    pause_and_display(0x2);
#endif

#ifdef __LEDS_USED__
    EE_led_3_off();
#endif

    TerminateTask();
}

TASK(thread3)
{
    EE_assert(19, 1, 18);

#ifdef __LEDS_USED__
    EE_led_4_on();
#endif

#ifdef __LCD_USED__
    pause_and_display(0x3);
#endif

#ifdef __LEDS_USED__
    EE_led_4_off();
#endif

    TerminateTask();
}
