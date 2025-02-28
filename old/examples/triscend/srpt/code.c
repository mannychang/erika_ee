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
 * Author: 2001-2002 Marko Bertogna
 * CVS: $Id: code.c,v 1.4 2006/05/14 18:00:59 pj Exp $
 */

#include "code.h"




/*
 * Display a number during a short pause 
 */
#define LOOP_LENGHT (0040)	//1.000.000 is ~1 sec	@ 30MHz 

void pause_and_display(int disp)
{
    int i=0;
    for (i=0; i<LOOP_LENGHT; i++)    EE_LED_display(disp);
}



void fiq_handler(void)
{
	/* put here the fiq handler */
}

void main(void)		
{
    /* peripherals initialization */
    init_drivers();

    /* periodic timer start */
    EE_timer0_start(0xffff, 0x000000c8);	//enable timer 0 periodic mode, bck/256

    tick_start();

    /* if there are no thread to execute, the main thread will go
     * into an infinite loop */
    for (;;) {
	/* LCD idle indication */
	pause_and_display(0xd);
	
	/* background activities, none in this test */
    }
}
