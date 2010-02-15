/** 
* @file ee_timer.h
* @brief TIMER interface for PIC32
* @author Christian Nastasi
* @version 0.1
* @date 2010-02-02
*/
#ifndef __INCLUDE_MICROCHIP_PIC32_TIMER_H__
#define __INCLUDE_MICROCHIP_PIC32_TIMER_H__

#include "ee.h"

enum {
	EE_TIMER_1  = 0,
	EE_TIMER_2,
	EE_TIMER_3,
	EE_TIMER_4,
	EE_TIMER_5,
	EE_TIMER_N,
};

#define EE_TIMER_NO_ERRORS		1	//Used positive
#define EE_TIMER_ERR_BAD_TIMER_ID	1
#define EE_TIMER_ERR_BAD_ARGS		2
#define EE_TIMER_ERR_UNIMPLEMENTED	10

/* See pic32mx family reference manual. */
#define EE_TIMER_PRESCALE_1		0
#define EE_TIMER_PRESCALE_8		1
#define EE_TIMER_PRESCALE_64		2
#define EE_TIMER_PRESCALE_256		3

EE_INT8 EE_timer_hard_init(EE_UINT8 id, EE_UINT16 period, EE_UINT8 prescale); 
EE_INT8 EE_timer_soft_init(EE_UINT8 id, EE_UINT32 period_us, EE_UINT32 f_tick); 
EE_INT8 EE_timer_set_callback(EE_UINT8 id, void (*f)(void)); 
EE_INT8 EE_timer_start(EE_UINT8 id);
EE_INT8 EE_timer_stop(EE_UINT8 id);
EE_INT8 EE_timer_get_val(EE_UINT8 id, EE_UINT16 *v);

#endif