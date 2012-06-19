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
 * GPT372:	Gpt_Irq.c File for implementation of interrupt service routine
 * 		(BSW00314)
 *
 * AUTOSAR-"like" GPT Driver Channels ISRs Source File.
 *
 * Author:  2012,  Francesco Esposito
 */

/*
 * GPT261:	Gpt_Irq.c shall include Gpt.h for the prototype declaration of
 * 		the notification functions. (BSW164)
 */
#include "Gpt.h"

/* 
 * GPT256:	The GPT module shall perform Inter Module Checks to avoid
 * 		integration of incompatible files. The imported included files
 * 		shall be checked by preprocessing directives. (BSW004)
 *
 * The following version numbers shall be verified:
 * - <MODULENAME>_AR_RELEASE_MAJOR_VERSION
 * - <MODULENAME>_AR_RELEASE_MINOR_VERSION
 * Where <MODULENAME> is the module abbreviation of the other (external) modules
 * which provide header files included by the GPT module.
 *
 * If the values are not identical to the expected values, an error shall be
 * reported.
 */
#define	GPT_AR_RELEASE_MAJOR_VERSION	4
#define	GPT_AR_RELEASE_MINOR_VERSION	0

#if !defined( GPT_AR_MAJOR_VERSION ) || \
    ( GPT_AR_MAJOR_VERSION != GPT_AR_RELEASE_MAJOR_VERSION )
#error	Gpt_Irq: version mismatch.
#endif

#include "ee.h"
#include "ee_irq.h"
#include "Gpt_Internal.h"

/*
 * Type that holds all global data for Gpt Driver
 */
typedef struct
{
  boolean			Init;		/* GPT Driver Initialized?    */

  const Gpt_ConfigType *	ConfigPtr;	/* Actual Configuration	      */

  uint32			Status;		/* Channel Status	      */

  uint32			Notifications;	/* Channel Notifications      */

} Gpt_GlobalType;

/*
 * GPT Driver Global Configuration Reference
 */
extern Gpt_GlobalType Gpt_Global;

#if ( GPT_ENABLE_DISABLE_NOTIFICATION_API == STD_ON )

extern int Gpt_NotificationList[];

#if ( \
  defined(EE_PPCE200ZX_30_ISR) || \
  defined(EE_PPCE200ZX_31_ISR) || \
  defined(EE_PPCE200ZX_32_ISR) || \
  defined(EE_PPCE200ZX_33_ISR) || \
  defined(EE_PPCE200ZX_59_ISR) || \
  defined(EE_PPCE200ZX_60_ISR) || \
  defined(EE_PPCE200ZX_61_ISR) || \
  defined(EE_PPCE200ZX_127_ISR) \
)

Gpt_Notification prova, prova2;

void Gpt_Isr_STM(
  Gpt_ChannelType		Channel_STM
)
{
/*	register boolean		init;
  	register const Gpt_ConfigType	*cfg;
	register EE_FREG		flags;

	init = Gpt_Global.Init;
	cfg = Gpt_Global.ConfigPtr;

	// Notification Callback Call.
	if ( ( Channel_STM < 3U) &&
	( cfg->GptChannels[Channel_STM].GptNotificationPtr != NULL_PTR ) &&
	( Gpt_NotificationList[Channel_STM] == NOTIFICATION_ENABLED)
	) {
		//(*(cfg->GptChannels[Channel_STM].GptNotificationPtr))();
		Gpt_Notification_Channel_0_STM();

//prova = cfg->GptChannels[Channel_STM].GptNotificationPtr;
//prova2 = Gpt_Notification_Channel_0_STM;
//cfg->GptChannels[Channel_STM].GptNotificationPtr();
	}

	// clear request
        mpc5643l_stm_clear_int(Channel_STM);

	if (cfg->GptChannels[Channel_STM].GptChannelMode == \
		GPT_CH_MODE_CONTINUOUS)
	{
		// stop timer
		mpc5643l_stm_disable();

		// enable channel 0 to raise a new interrupt
		mpc5643l_stm_select_channel(Channel_STM);

		// initial counter value (equal to 0)
		mpc5643l_stm_set_counter(0);

		// start timer
		mpc5643l_stm_enable();
	}*/

// Periodic activity: send CAN message
        Gpt_Notification_Channel_0_STM();

        // clear request
        mpc5643l_stm_clear_int(0);

        // stop timer
        mpc5643l_stm_disable();

        // enable channel 0 to raise a new interrupt
        mpc5643l_stm_select_channel(0);

        // initial counter value (equal to 0)
        mpc5643l_stm_set_counter(0);

	// start timer
        mpc5643l_stm_enable();
}

static void Gpt_Isr_PIT(
  Gpt_ChannelType		Channel_PIT
)
{
	;
}
#endif

/*
 * STM timers definitions
 */

#ifdef	EE_PPCE200ZX_30_ISR
DeclareIsr(EE_PPCE200ZX_30_ISR);
/* GPT CHANNEL 0 STM ISR */
ISR2(EE_PPCE200ZX_30_ISR)
{
  Gpt_Isr_STM(GPT_CHANNEL_0_STM);
}
#endif

#ifdef	EE_PPCE200ZX_31_ISR
DeclareIsr(EE_PPCE200ZX_31_ISR);
/* GPT CHANNEL 1 STM ISR */
ISR2(EE_PPCE200ZX_31_ISR)
{
  Gpt_Isr_STM(GPT_CHANNEL_1_STM);
}
#endif

#ifdef	EE_PPCE200ZX_32_ISR
DeclareIsr(EE_PPCE200ZX_32_ISR);
/* GPT CHANNEL 2 STM ISR */
ISR2(EE_PPCE200ZX_32_ISR)
{
  Gpt_Isr_STM(GPT_CHANNEL_2_STM);
}
#endif

#ifdef	EE_PPCE200ZX_33_ISR
DeclareIsr(EE_PPCE200ZX_33_ISR);
/* GPT CHANNEL 3 STM ISR */
ISR2(EE_PPCE200ZX_33_ISR)
{
  Gpt_Isr_STM(GPT_CHANNEL_3_STM);
}
#endif

/*
 * PIT timers definitions
 */

#ifdef	EE_PPCE200ZX_59_ISR
DeclareIsr(EE_PPCE200ZX_59_ISR);
/* GPT CHANNEL 0 PIT ISR */
ISR2(EE_PPCE200ZX_59_ISR)
{
  Gpt_Isr_PIT(GPT_CHANNEL_0_PIT);
}
#endif

#ifdef	EE_PPCE200ZX_60_ISR
DeclareIsr(EE_PPCE200ZX_60_ISR);
/* GPT CHANNEL 1 PIT ISR */
ISR2(EE_PPCE200ZX_60_ISR)
{
  Gpt_Isr_PIT(GPT_CHANNEL_1_PIT);
}
#endif

#ifdef	EE_PPCE200ZX_61_ISR
DeclareIsr(EE_PPCE200ZX_61_ISR);
/* GPT CHANNEL 2 PIT ISR */
ISR2(EE_PPCE200ZX_61_ISR)
{
  Gpt_Isr_PIT(GPT_CHANNEL_2_PIT);
}
#endif

#ifdef	EE_PPCE200ZX_127_ISR
DeclareIsr(EE_PPCE200ZX_127_ISR);
/* GPT CHANNEL 3 PIT ISR */
ISR2(EE_PPCE200ZX_127_ISR)
{
  Gpt_Isr_PIT(GPT_CHANNEL_3_PIT);
}
#endif


#endif	/* GPT_NOTIFICATIONS_API */