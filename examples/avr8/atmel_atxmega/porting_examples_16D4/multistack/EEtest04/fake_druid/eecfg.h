#ifndef __EECFG_H__
#define __EECFG_H__


#define RTDRUID_CONFIGURATOR_NUMBER 1277



/***************************************************************************
 *
 * Common defines ( CPU 0 )
 *
 **************************************************************************/

    /* TASK definition */
    #define EE_MAX_TASK 2
    #define Task1 0
    #define Task2 1

    /* MUTEX definition */
    #define EE_MAX_RESOURCE 0U

    /* ALARM definition */
    #define EE_MAX_ALARM 0

    /* COUNTER definition */
    #define EE_MAX_COUNTER 0

    /* APPMODE definition */
    #define EE_MAX_APPMODE 0U

    /* CPUs */
    #define EE_MAX_CPU 1
    #define EE_CURRENTCPU 0

/***************************************************************************
 *
 * ISR
 *
 **************************************************************************/
#define EE_AVR8_TCC0_OVF_ISR		timer1_handler
#define EE_AVR8_TCC0_OVF_ISR_PRI	EE_ISR_PRI_LOW
#define EE_AVR8_TCC0_OVF_ISR_CAT	2

#define EE_AVR8_TCC1_OVF_ISR		timer2_handler
#define EE_AVR8_TCC1_OVF_ISR_PRI	EE_ISR_PRI_HI
#define EE_AVR8_TCC1_OVF_ISR_CAT	2

#ifndef __DISABLE_EEOPT_DEFINES__

/***************************************************************************
 *
 * User options
 *
 **************************************************************************/
#define DEBUG
#define __ASSERT__


/***************************************************************************
 *
 * Automatic options
 *
 **************************************************************************/
//#define __RTD_CYGWIN__
//#define __RTD_LINUX__
#define __AVR8__
#define __ATXMEGA__
#define __FP__
#define __IRQ_STACK_NEEDED__
#define __MULTI__
#define __FP_NO_RESOURCE__
#define __ALLOW_NESTED_IRQ__

#endif



#endif

