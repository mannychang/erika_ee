/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2009-2012  Evidence Srl
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
  Author:Errico Guidieri, 2012
  */ 

#include "ee.h"
#include "kernel/sem/inc/ee_sem.h"
#include "kernel/sem/inc/ee_api.h"
#include "test/assert/inc/ee_assert.h"

#ifndef TRUE
#define TRUE 1
#endif  /* TRUE */

/* assertion data */
EE_TYPEASSERTVALUE EE_assertions[10];
/* This example has 2 semaphores, used by the Task1 and by the Task2 tasks. */

/* Final result */
EE_TYPEASSERTVALUE result;

/* This semaphore is initialized automatically */
SemType P = STATICSEM(1);

/* This semaphore is initialized inside the Background Task */
SemType V;

volatile int taskp_counter = 0;
volatile int taskc_counter = 0;

TASK(Task1)
{
  taskp_counter++;
  EE_assert(2, taskp_counter==1, 1);
  ActivateTask(Task2);
  EE_assert(4, taskp_counter==1, 3);
  PostSem(&V); 
  EE_assert(6, taskp_counter==1, 5);
}

TASK(Task2)
{
  taskc_counter++;
  EE_assert(3, taskc_counter==1, 2);
  WaitSem(&V);
  EE_assert(5, taskc_counter==1, 4);
}

int main(void)
{
  EE_assert(1, TRUE, EE_ASSERT_NIL);
  /* Initialization of the second semaphore of the example; the first
  semaphore is initialized inside the definition */
  InitSem(V,0);

  ActivateTask(Task1);

  EE_assert_range(0,1,6);
  result = EE_assert_last();

  /* Forever loop: background activities (if any) should go here.
  Please note that in this example the code never reach this point... */
  for (;;);

  return 0;
}
