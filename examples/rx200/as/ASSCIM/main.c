/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
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
 *  Simple project to to estimate the SCI driver MINIMAL services.
 *
 * Author: 2013,  Gianluca Franchino
 */

 /* RT-Kernel */
#include <ee.h>

#include "Dio.h"
#include "Mcu.h"
#include "Port.h"
#include "Sci.h"
#include "test/assert/inc/ee_assert.h"


#ifndef	TRUE
#define	TRUE	1
#endif

/* Assertions */
enum EE_ASSERTIONS {
	EE_ASSERT_FIN = 0,
	EE_ASSERT_INIT,
	EE_ASSERT_SCI_INIT,
	EE_ASSERT_DIM
};

EE_TYPEASSERTVALUE EE_assertions[EE_ASSERT_DIM];

/* Final result */
volatile EE_TYPEASSERTVALUE result;

/*
 * TASK 1
 */
TASK(Task1)
{

  /* Nothing to do. */

}

#define	ASCII_NULL	0x00
#define	ASCII_BELL	0x07
/*
 * MAIN TASK
 */
int main(void)
{
	Dio_PortLevelType	lvl;
	Sci_StatusType	rxstat;
	EE_UINT8	TxData = ASCII_NULL;
	EE_UINT8	RxData = ASCII_NULL;

	Mcu_Init(MCU_CONFIG_DEFAULT_PTR);
	
	EE_assert(EE_ASSERT_INIT, TRUE, EE_ASSERT_NIL);
  
	Port_Init(PORT_CONFIG_DEFAULT_PTR);
	Dio_Init(DIO_CONFIG_DEFAULT_PTR);

	Port_Init(PORT_CONFIG_SCI_PTR);
	Sci_Init(SCI_CONFIG_DEFAULT_PTR);

	EE_assert(EE_ASSERT_SCI_INIT, TRUE, EE_ASSERT_INIT);

	EE_assert_range(EE_ASSERT_FIN, TRUE, EE_ASSERT_SCI_INIT);
	result = EE_assert_last();
	
	  /* Forever loop: background activities (if any) should go here */
	for (;result == 1;) {

		rxstat = Sci_GetStatus(SCI_CHANNEL_0);

		switch (rxstat) {
		case SCI_NOT_OK:
		/* Sci_GetStatus() Error */
			TxData = ASCII_BELL;
		break;
		case SCI_RX_ERROR:
		/* Rx Error Error */
			Sci_DeInit();
			Sci_Init(SCI_CONFIG_DEFAULT_PTR);
			TxData = ASCII_BELL;
		break;
		case SCI_OPERATIONAL:
			if ( Sci_ReadRxData(SCI_CHANNEL_0, &RxData) == E_OK )
				TxData = RxData;
			else
			/* Sci_ReadRxData() Error */
				TxData = ASCII_BELL;
		break;			
		case SCI_TX_BUSY:
		case SCI_TX_OK:
		default:
			TxData = ASCII_NULL;
		break;

		}	
		
		if ( TxData != ASCII_NULL ) {

			if ( Sci_WriteTxData(SCI_CHANNEL_0, TxData) == E_OK ) {

				lvl = Dio_ReadChannel(DIO_CHANNEL_USER_LED_0);

				if (lvl == STD_HIGH)
					Dio_WriteChannel(DIO_CHANNEL_USER_LED_0, STD_LOW);
				else
					Dio_WriteChannel(DIO_CHANNEL_USER_LED_0, STD_HIGH);

			}

		}
	}
	
}
