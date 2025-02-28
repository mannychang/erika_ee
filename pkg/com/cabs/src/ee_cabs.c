/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2016  Evidence Srl
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
 * Author: 2002, Paolo Gai
 *         2016, Giuseppe Serano
 */

#include "ee.h"

#ifndef	EE_PRIVATE_CAB_INIT__
void EE_cab_init(const struct EE_cab_ROM_desc *c)
{
  EE_CAB_INDEX i;

  for ( i = 0; i < ( c->n_msg - 1 ); i++ )
  {
    c->freestk[i] = i;
    c->used[i] = 0;
  } 
}
#endif	/* EE_PRIVATE_CAB_INIT__ */

#ifndef	EE_PRIVATE_CAB_RESERVE__
void
EE_cab_reserve(
  const struct EE_cab_ROM_desc *c, void **msg, EE_CAB_INDEX *msg_num
) {
  EE_CAB_INDEX	i;
  EE_FREG	flags;

  flags = EE_hal_suspendIRQ();

#ifdef	__MSRP__	/* Multicore */
  EE_hal_spin_in(c->sl_id);
#endif	/* __MSRP__ - Multicore */

  i = --( c->ram_desc->free );

  *msg_num = c->freestk[i];

  *msg = (void *)(c->msgs + ((*msg_num) * c->dim_msg));

#ifdef	__MSRP__	/* Multicore */
  EE_hal_spin_out(c->sl_id);
#endif	/* __MSRP__ - Multicore */

  EE_hal_resumeIRQ(flags);
}
#endif	/* EE_PRIVATE_CAB_RESERVE__ */

#ifndef	EE_PRIVATE_CAB_PUTMES__
void EE_cab_putmes(const struct EE_cab_ROM_desc *c, EE_CAB_INDEX msg_num)
{
  EE_CAB_INDEX	old;
  EE_FREG	flags;

  flags = EE_hal_suspendIRQ();

#ifdef	__MSRP__	/* Multicore */
  EE_hal_spin_in(c->sl_id);
#endif	/* __MSRP__ - Multicore */

  old = c->ram_desc->mrd;

  if ( !c->used[old] )
  {
    c->freestk[c->ram_desc->free++] = old;
  }
  
  c->ram_desc->mrd = msg_num;

#ifdef	__MSRP__	/* Multicore */
  EE_hal_spin_out(c->sl_id);
#endif	/* __MSRP__ - Multicore */

  EE_hal_resumeIRQ(flags);
}
#endif	/* EE_PRIVATE_CAB_PUTMES__ */

#ifndef	EE_PRIVATE_CAB_GETMES__
void
EE_cab_getmes(
  const struct EE_cab_ROM_desc *c, void **msg, EE_CAB_INDEX *msg_num
) {
  EE_CAB_INDEX	mrd;
  EE_FREG	flags;

  flags = EE_hal_suspendIRQ();

#ifdef	__MSRP__	/* Multicore */
  EE_hal_spin_in(c->sl_id);
#endif	/* __MSRP__ - Multicore */

  mrd = c->ram_desc->mrd;
  c->used[mrd]++;

  *msg = (void *)(c->msgs + (mrd * c->dim_msg));
  *msg_num = mrd;

#ifdef	__MSRP__	/* Multicore */
  EE_hal_spin_out(c->sl_id);
#endif	/* __MSRP__ - Multicore */

  EE_hal_resumeIRQ(flags);
}
#endif	/* EE_PRIVATE_CAB_GETMES__ */

#ifndef	EE_PRIVATE_CAB_UNGET__
void EE_cab_unget(const struct EE_cab_ROM_desc *c, EE_CAB_INDEX msg_num)
{
  EE_FREG	flags;

  flags = EE_hal_suspendIRQ();

#ifdef	__MSRP__	/* Multicore */
  EE_hal_spin_in(c->sl_id);
#endif	/* __MSRP__ - Multicore */

  c->used[msg_num]--;

  if (!c->used[msg_num] && ( msg_num != c->ram_desc->mrd ))
  {
    c->freestk[c->ram_desc->free++] = msg_num;
  }

#ifdef	__MSRP__	/* Multicore */
  EE_hal_spin_out(c->sl_id);
#endif	/* __MSRP__ - Multicore */

  EE_hal_resumeIRQ(flags);
}
#endif	/* EE_PRIVATE_CAB_UNGET__ */
