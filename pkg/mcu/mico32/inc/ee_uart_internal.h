/*
* ee_uart_internal.h
*/


#ifndef __INCLUDE_EEMCUMICO32_UART_INTERNAL_H__
#define __INCLUDE_EEMCUMICO32_UART_INTERNAL_H__

#ifdef __USE_UART__

#include "cpu/mico32/inc/ee_internal.h"
#include "mcu/mico32/inc/ee_internal.h"
#include "mcu/mico32/inc/ee_buffer.h"
#include <MicoUart.h>				// to use LATTICE data structures.


/*************************************************************************
 Uart
 *************************************************************************/

/*
	UART structure (used in ISR mode):
*/
typedef struct {
	int err;							// last error condition 					
	int mode;							// uart operating mode (polling, isr, ...)
    MicoUart_t* base;					// controller base address
    int irqf;							// irq flag to register the handler
    EE_ISR_callback rxcbk;				// rx callback
    EE_ISR_callback txcbk;				// tx callback
    EE_buffer rxbuf;					// rx buffer used in isr mode
    EE_buffer txbuf;					// tx buffer used in isr mode
} EE_uart_st;

/* Uart settings */
#define EE_UART_MSGSIZE 	(1)		
#define EE_UART_BUFSIZE 	(4)

#define EE_UART_BIT8_ODD	(0x0B)
#define EE_UART_BIT8_EVEN	(0x1B)
#define EE_UART_BIT8_NO		(0x03)

#define EE_UART_BIT7_ODD	(0x0A)
#define EE_UART_BIT7_EVEN	(0x1A)
#define EE_UART_BIT7_NO		(0x02)

#define EE_UART_BIT6_ODD	(0x09)
#define EE_UART_BIT6_EVEN	(0x19)
#define EE_UART_BIT6_NO		(0x01)

#define EE_UART_BIT5_ODD	(0x08)
#define EE_UART_BIT5_EVEN	(0x18)
#define EE_UART_BIT5_NO		(0x00)

#define EE_UART_BIT_STOP_1	(0x00)
#define EE_UART_BIT_STOP_2	(0x04)

/* Uart operating modes */
#define EE_UART_POLLING		(0x00)
#define EE_UART_RX_ISR		(0x01)
#define EE_UART_TX_ISR		(0x02)	
#define EE_UART_RXTX_ISR	(0x03)
#define EE_UART_RX_BLOCK	(0x10)
#define EE_UART_TX_BLOCK	(0x20)
#define EE_UART_RXTX_BLOCK  (0x30)	

#define EE_uart_need_init_rx_buf(old,new)  ( !((old) & EE_UART_RX_ISR) && ((new) & EE_UART_RX_ISR) )
#define EE_uart_need_init_tx_buf(old,new)  ( !((old) & EE_UART_TX_ISR) && ((new) & EE_UART_TX_ISR) )
#define EE_uart_need_enable_rx_int(old, new)  ( (new) & EE_UART_RX_ISR )
#define EE_uart_need_disable_rx_int(old, new)  ( !((new) & EE_UART_RX_ISR) )
#define EE_uart_enabled_rx_int(mode)  ( (mode) & EE_UART_RX_ISR )
#define EE_uart_need_enable_int(new)  ( ((new) & EE_UART_RX_ISR) || ((new) & EE_UART_TX_ISR) )

#define EE_uart_tx_polling(mode) ( !((mode) & EE_UART_TX_ISR) )
#define EE_uart_rx_polling(mode) ( !((mode) & EE_UART_RX_ISR) )
#define EE_uart_rx_block(mode) ( ((mode) & EE_UART_RX_BLOCK) )
#define EE_uart_tx_block(mode) ( ((mode) & EE_UART_TX_BLOCK) )

/* Uart utils */
#define EE_UART_NULL_VET			((EE_UINT8 *)0)

/* Uart return values */
#define EE_UART_OK					(0x00)
#define EE_UART_ERR_RX_BUF_FULL   	(-20)
#define EE_UART_ERR_RX_BUF_EMPTY   	(-21)
#define EE_UART_ERR_TX_BUF_FULL   	(-22)
#define EE_UART_ERR_TX_BUF_EMPTY   	(-23)
#define EE_UART_ERR_BAD_VALUE		(-24)
#define EE_UART_ERR_TX_NOT_READY	(-25)
#define EE_UART_ERR_RX_NOT_READY	(-26)


/********************** Internal functions **************************/

/*
	int EE_hal_uart_config(EE_uart_st* usp, int baudrate, int setttings);
		This function is used to change UART parameters. 
		Arguments:
			- EE_uart_st* usp: UART structure pointer
			- int baudrate: rate value in Hz
			- int settings: settings value (please, use macros defined in ee_uart.h)   
		Actions: 
			- reset ier and iir register (polling mode)
			- set parity, stop bits, data length
			- calculate and set baudrate 
		Return values:
			- the function return the result: EE_UART_OK if no errors found
*/
int EE_hal_uart_config(EE_uart_st* usp, int baudrate, int setttings);					//1


/*
	int EE_hal_uart_set_mode(EE_uart_st* usp, int mode);	
		This function is used to change UART operating mode. 
		Arguments:
			- EE_uart_st* usp: UART structure pointer
			- int mode: UART desired operating mode (please, use macros defined in ee_uart.h)
		Actions: 
			- update the operating mode field in the structure
			- initialize rx and tx buffers if needed
			- set ier register
		Return values:
			- the function return the result:   EE_UART_OK if no errors found
												EE_UART_ERR_BAD_VALUE if bad argument passed 
*/
int EE_hal_uart_set_mode(EE_uart_st* usp, int mode);								//2


/*
	int EE_hal_uart_set_rx_callback(EE_uart_st* usp, EE_ISR_callback isr_rx_callback);	
		This function is used to set rx callback called after execution of the UART handler. 
		Arguments:
			- EE_uart_st* usp: UART structure pointer
			- EE_ISR_callback isr_rx_callback: Name of the function to be called (please, use macros defined in ee_uart.h)
		Actions: 
			- set rxcbk field in the structure
		Return values:
			- the function return the result:   EE_UART_OK if no errors found
*/
int EE_hal_uart_set_rx_callback(EE_uart_st* usp, EE_ISR_callback isr_rx_callback);		//3


/*
	int EE_hal_uart_set_tx_callback(EE_uart_st* usp, EE_ISR_callback isr_tx_callback);	
		This function is used to set tx callback called after execution of the UART handler. 
		Arguments:
			- EE_uart_st* usp: UART structure pointer
			- EE_ISR_callback isr_tx_callback: Name of the function to be called (please, use macros defined in ee_uart.h)
		Actions: 
			- set txcbk field in the structure
		Return values:
			- the function return the result:   EE_UART_OK if no errors found
*/
int EE_hal_uart_set_tx_callback(EE_uart_st* usp, EE_ISR_callback isr_rx_callback);		//4


/*
	int EE_hal_uart_write_byte(EE_uart_st* usp, EE_UINT8 data);	
		This function is used to transmit one byte.
		Arguments:
			- EE_uart_st* usp: UART structure pointer
			- EE_UINT8 data: Character to be transmitted 
		Actions: 
			- in tx polling mode: wait if tx register is not ready, then transmit 
			- in tx isr mode: load the character in the tx buffer and set ier register 
							  to enable tx interrupts
		Return values:
			- in tx polling mode: returns 1 if data is transmitted, else the return value is < 0 
			- in tx isr mode: returns 1 if data is loaded in the tx buffer, else the return value is < 0 
*/
int EE_hal_uart_write_byte(EE_uart_st* usp, EE_UINT8 data);									//5


/*
	int EE_hal_uart_read_byte(EE_uart_st* usp, EE_UINT8 *data);	
		This function is used to read one byte.
		Arguments:
			- EE_uart_st* usp: UART structure pointer
			- EE_UINT8 adddata: Address of the location (variable) to be written 
		Actions: 
			- in rx polling mode: wait if there isn't a value in rx register, then returns 
			- in rx isr mode: extracts a chracater from the rx buffer
		Return values:
			- in rx polling mode: returns 1 if data is received, else the return value is < 0 
			- in rx isr mode: returns 1 if data is extracted from the rx buffer, else the return value is < 0 
*/
int EE_hal_uart_read_byte(EE_uart_st* usp, EE_UINT8 *data);								//6


/*
	int EE_hal_uart_read_buffer(EE_uart_st* usp, EE_UINT8 *vet, int len);
		This function is used to read more than one characters.
		Arguments:
			- EE_uart_st* usp: UART structure pointer
			- EE_UINT8 *vet: address of the first location to be written with the received characters
			- int len: number of characters to read
		Actions: 
			- in rx polling mode: wait until all characters are read, then returns 
			- in rx isr mode: extracts chracaters from the rx buffer
		Return values:
			- in rx polling mode: returns the number of cahracters read, else the return value is < 0 
			- in rx isr mode: returns the number of cahracters extracted from the rx buffer, else the return value is < 0 
*/
int EE_hal_uart_read_buffer(EE_uart_st* usp, EE_UINT8 *vet, int len);						//7


/*
	int EE_hal_uart_write_buffer(EE_uart_st* usp, EE_UINT8 *vet, int len);
		This function is used to write more than one characters.
		Arguments:
			- EE_uart_st* usp: UART structure pointer
			- EE_UINT8 *vet: address of the first character to be transmitted
			- int len: number of characters to transmit
		Actions: 
			- in tx polling mode: wait until all characters are transmit, then returns 
			- in tx isr mode: load chracaters in the tx buffer
		Return values:
			- in tx polling mode: returns the number of characters transmitted, else the return value is < 0 
			- in tx isr mode: returns the number of cahracters loaded in the tx buffer, else the return value is < 0 
*/
int EE_hal_uart_write_buffer(EE_uart_st* usp, EE_UINT8 *vet, int len);						//8


/*
	int EE_hal_uart_enable_IRQ(EE_uart_st* usp, int ier);
		This function is used to enable UART IRQ.
		Arguments:
			- EE_uart_st* usp: UART structure pointer
			- int ier: value to be loaded in the ier register (please, see MicoUart.h from Lattice)
		Actions: 
			- enable irq loading in the ier register the argument passed 
		Return values:
			- the function return the result:   EE_UART_OK if no errors found
*/
int EE_hal_uart_enable_IRQ(EE_uart_st* usp, int ier);									//9


/*
	int EE_hal_uart_disable_IRQ(EE_uart_st* usp);
		This function is used to disable UART IRQ.
		Arguments:
			- EE_uart_st* usp: UART structure pointer
		Actions: 
			- disable irq clearing ier register 
		Return values:
			- the function return the result:   EE_UART_OK if no errors found
*/
int EE_hal_uart_disable_IRQ(EE_uart_st* usp);											//10

/*
	int EE_hal_uart_return_error(EE_uart_st* usp);
		This function return the error condition stored in the uart structure.
		Arguments:
			- EE_uart_st* usp: UART structure pointer
		Actions: 
			- return error condition
		Return values:
			- return error condition
*/
int EE_hal_uart_return_error(EE_uart_st* usp);											//11


/*
	void EE_uart_common_handler(int level);
		This function is UART IRQ handler.
		Arguments:
			- int level: IRQ level.
		Actions: 
			- The UART handler works for rx and tx interrupts. In case of rx interrupts
			  load the received character in the rx buffer, then calls the user callback. 
			  In case of tx interrupts extracts and transmits characters of the tx buffer,
			  then calls the user callaback..
		Return values:
			- none
*/
void EE_uart_common_handler(int level);													//handler

//int EE_hal_uart_disable(MicoUart_t* base);		ATT! not implemented! 
//int EE_hal_uart_enable(MicoUart_t* base);			ATT! not implemented!
//int EE_hal_uart_start(MicoUart_t* base);			ATT! not implemented!
//int EE_hal_uart_stop(MicoUart_t* base);			ATT! not implemented!

/* Macro for Structure declaration */
#define DECLARE_STRUCT_UART(uc, lc) \
  extern EE_uart_st EE_ST_NAME(lc);
/* Macro for structure definition */
#define DEFINE_STRUCT_UART(uc, lc) \
EE_uart_st cat3(ee_, lc, _st) = { \
	.err=EE_UART_OK, .mode= EE_UART_POLLING | EE_UART_RXTX_BLOCK, .base= (MicoUart_t* )cat2(uc, _BASE_ADDRESS),\
	.irqf= cat2(uc, _IRQ), .rxcbk= EE_NULL_CBK, .txcbk= EE_NULL_CBK,\
	.rxbuf.data= EE_VETRX_NAME(lc),.txbuf.data= EE_VETTX_NAME(lc)};
/* Macro for vectors (buffers) definition */  
#define DEFINE_VET_UART(uc, lc) \
EE_UINT8 EE_VETRX_NAME(lc)[EE_UART_BUFSIZE]; \
EE_UINT8 EE_VETTX_NAME(lc)[EE_UART_BUFSIZE];  

/* Macros for User functions (API) */  
#define DECLARE_FUNC_UART(uc, lc) \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _config)(int baudrate,int settings){ \
	return EE_hal_uart_config(& EE_ST_NAME(lc), baudrate, settings); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _set_mode)(int mode){ \
	return EE_hal_uart_set_mode(& EE_ST_NAME(lc), mode); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _set_rx_ISR_callback)(EE_ISR_callback rxcbk){ \
	return EE_hal_uart_set_rx_callback(& EE_ST_NAME(lc), rxcbk); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _set_tx_ISR_callback)(EE_ISR_callback txcbk){ \
	return EE_hal_uart_set_tx_callback(& EE_ST_NAME(lc), txcbk); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _send_byte)(EE_UINT8 data){ \
	return EE_hal_uart_write_byte(& EE_ST_NAME(lc), data); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _receive_byte)(EE_UINT8 *data){ \
	return EE_hal_uart_read_byte(& EE_ST_NAME(lc), data); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _send_buffer)(EE_UINT8 *vet, int len){ \
	return EE_hal_uart_write_buffer(& EE_ST_NAME(lc), vet, len); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _receive_buffer)(EE_UINT8 *vet, int len){ \
	return EE_hal_uart_read_buffer(& EE_ST_NAME(lc), vet, len); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _enable_IRQ)(int ier){ \
	return EE_hal_uart_enable_IRQ(& EE_ST_NAME(lc), ier); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _disable_IRQ)(void){ \
	return EE_hal_uart_disable_IRQ(& EE_ST_NAME(lc)); } \
__INLINE__ int __ALWAYS_INLINE__ cat3(EE_, lc, _return_error)(void){ \
	return EE_hal_uart_return_error(& EE_ST_NAME(lc)); }
	
/* User functions (API): */  
#ifdef EE_UART1_NAME_UC
DECLARE_STRUCT_UART(EE_UART1_NAME_UC, EE_UART1_NAME_LC)
DECLARE_FUNC_UART(EE_UART1_NAME_UC, EE_UART1_NAME_LC)
#endif	

#ifdef EE_UART2_NAME_UC
DECLARE_STRUCT_UART(EE_UART2_NAME_UC, EE_UART2_NAME_LC)
DECLARE_FUNC_UART(EE_UART2_NAME_UC, EE_UART2_NAME_LC)
#endif	

/* Return the Uart structure for the componente associated with the given IRQ
 * level */
__DECLARE_INLINE__ EE_uart_st *EE_get_uart_st_from_level(int level);


#ifndef EE_UART2_NAME_UC
/* If there is only one component of this kind, no test is done */
__INLINE__ EE_uart_st * __ALWAYS_INLINE__ EE_get_uart_st_from_level(int level)
{
    return & EE_ST_NAME(EE_UART1_NAME_LC);
}
#else /* #ifndef EE_UART2_NAME_UC */
__INLINE__ EE_uart_st * __ALWAYS_INLINE__ EE_get_uart_st_from_level(int level)
{
    if (level == EE_IRQ_NAME(EE_UART1_NAME_UC))
        return & EE_ST_NAME(EE_UART1_NAME_LC);
    else
        return & EE_ST_NAME(EE_UART2_NAME_LC);
}
#endif /* #ifndef EE_UART_NAME2_UC */


#ifdef __USE_MICO_PIC_API__
/* Macros for compatibility with pic32 uart driver*/ 
#define EE_mchp_uart_init(lc, baud, bf, mode) EE_hal_uart_config(& EE_ST_NAME(lc), baud, bf)
#define EE_mchp_uart_write_byte(lc, data) EE_hal_uart_write_byte(& EE_ST_NAME(lc), data)			
#define EE_mchp_uart_read_byte(lc, data) EE_hal_uart_read_byte(& EE_ST_NAME(lc), data)	
#define EE_mchp_uart_set_rx_callback(lc, rxcbk) EE_hal_uart_set_rx_callback(& EE_ST_NAME(lc), rxcbk)

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_uart_init(EE_UINT8 port, EE_UINT32 baud, EE_UINT16 byte_format, EE_UINT16 mode)
{
	EE_INT8 ret;
	
	#if defined(EE_UART1_NAME_LC) && defined(EE_UART2_NAME_LC)
	if(port==1)
		ret = EE_mchp_uart_init(EE_UART1_NAME_LC, baud, byte_format, mode);
	else 
		ret = EE_mchp_uart_init(EE_UART2_NAME_LC, baud, byte_format, mode);
	#else
		#if defined(EE_UART1_NAME_LC)
		ret = EE_mchp_uart_init(EE_UART1_NAME_LC, baud, byte_format, mode);
		#else	
		ret = EE_mchp_uart_init(EE_UART2_NAME_LC, baud, byte_format, mode);
		#endif
	#endif
	
	return ret;
}

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_uart_close(EE_UINT8 port)
{
	return EE_UART_OK;
}

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_uart_write_byte(EE_UINT8 port, EE_UINT8 data)
{
	EE_INT8 ret;
	
	#if defined(EE_UART1_NAME_LC) && defined(EE_UART2_NAME_LC)
	if(port==1)
		ret = EE_mchp_uart_write_byte(EE_UART1_NAME_LC, data);
	else 
		ret = EE_mchp_uart_write_byte(EE_UART2_NAME_LC, data);
	#else
		#if defined(EE_UART1_NAME_LC)
		ret = EE_mchp_uart_write_byte(EE_UART1_NAME_LC, data);
		#else	
		ret = EE_mchp_uart_write_byte(EE_UART2_NAME_LC, data);
		#endif
	#endif
	
	return ret;

}

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_uart_read_byte(EE_UINT8 port, EE_UINT8 *data)
{
	EE_INT8 ret;
	
	#if defined(EE_UART1_NAME_LC) && defined(EE_UART2_NAME_LC)
	if(port==1)
		ret = EE_mchp_uart_read_byte(EE_UART1_NAME_LC, data);
	else 
		ret = EE_mchp_uart_read_byte(EE_UART2_NAME_LC, data);
	#else
		#if defined(EE_UART1_NAME_LC)
		ret = EE_mchp_uart_read_byte(EE_UART1_NAME_LC, data);
		#else	
		ret = EE_mchp_uart_read_byte(EE_UART2_NAME_LC, data);
		#endif
	#endif
	
	return ret;

}

typedef void (*EE_mchp_ISR_callback)(EE_UINT8);
extern EE_mchp_ISR_callback ee_mchp_uart1_ISR_cbk;
extern EE_mchp_ISR_callback ee_mchp_uart2_ISR_cbk;
void ee_aux_uart1_ISR_cbk(void);
void ee_aux_uart2_ISR_cbk(void);

__INLINE__ EE_INT8 __ALWAYS_INLINE__ EE_uart_set_rx_callback(EE_UINT8 port, void (*RxFunc)(EE_UINT8 data), EE_UINT8 rxmode)
{
	EE_INT8 ret;
	
	#if defined(EE_UART1_NAME_LC) && defined(EE_UART2_NAME_LC)
	if(port==1)
	{
		ee_mchp_uart1_ISR_cbk = RxFunc;
		ret = EE_mchp_uart_set_rx_callback(EE_UART1_NAME_LC, ee_aux_uart1_ISR_cbk);
	}
	else 
	{
		ee_mchp_uart2_ISR_cbk = RxFunc;
		ret = EE_mchp_uart_set_rx_callback(EE_UART2_NAME_LC, ee_aux_uart2_ISR_cbk);
	}
	#else
		#if defined(EE_UART1_NAME_LC)
		ee_mchp_uart1_ISR_cbk = RxFunc;
		ret = EE_mchp_uart_set_rx_callback(EE_UART1_NAME_LC, ee_aux_uart1_ISR_cbk);
		#else	
		ee_mchp_uart2_ISR_cbk = RxFunc;
		ret = EE_mchp_uart_set_rx_callback(EE_UART2_NAME_LC, ee_aux_uart2_ISR_cbk);
		#endif
	#endif
	
	return ret;

}

#endif // #ifdef __USE_MICO_PIC_API__

#endif // #ifdef __USE_UART__

#endif //__INCLUDE_EEMCUMICO32_UART_INTERNAL_H__