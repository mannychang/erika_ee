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

#ifndef __flex_simple802154_h__
#define __flex_simple802154_h__

#include <ee.h>
#include "ozb_ieee802154.h"

#define FLEX_SIMPLE802154_ADDRESSES 	10
#define FLEX_SIMPLE802154_CHANNELS 	255

typedef struct flex_simple802154_packet_t {
	EE_UINT16	src_addr;
	EE_UINT16 	dst_addr;
	EE_UINT16 	data_id;
	float 		data;
} flex_simple802154_packet_t; 

extern EE_UINT16 flex_simple802154_address_table[FLEX_SIMPLE802154_ADDRESSES];
extern EE_UINT16 flex_simple802154_local_address;

__INLINE__ 
EE_UINT16 flex_simple802154_address_lookup(EE_UINT16 address)
{
	EE_UINT16 i;

	for (i = 0; i < FLEX_SIMPLE802154_ADDRESSES; i++)
		if (flex_simple802154_address_table[i] == address)
			break;
	return i;
}

__INLINE__ 
void flex_simple802154_add_lookup(EE_UINT16 address)
{
	EE_UINT16 i;

	if (address == 0xFFFF) /* 0xFFFF is free entry) */
		return;
	i = flex_simple802154_address_lookup(address);
	if (i < SCICOS_OPENZB_ADDRESSES) /* Address already in lookup table */
		return;
	for (i = 0; i < SCICOS_OPENZB_ADDRESSES; i++) {
		/* Find free space (0xFFFF is free entry) in the lookup table */
		if (scicos_simple802154_address_table[i] == 0xFFFF) {
			scicos_simple802154_address_table[i] = address;
			return;
		}
	}
}

#endif