#include <ee.h>
#include <ee_irq.h>
#include "error.h"

void error(enum err_code code)
{
	/* Do nothing; you should add a breakpoint on this function to catch
	 * errors */
}

static void init_app1_timer(void)
{
	void App1Isr(void);
	EE_e200z7_register_ISR(10, App1Isr, 0);
	EE_e200z7_setup_decrementer(50000);
}

static void init_app2_timer(void)
{
	void App2Isr(void);
	EE_e200z7_register_ISR(11, App2Isr, 1);
	EE_e200zx_setup_fixed_intv(64-15);
}

void main(void)
{
	init_app1_timer();
	init_app2_timer();
	StartOS(OSDEFAULTAPPMODE);
	EE_hal_enableIRQ();
	while (1) {
		/* Background loop */
	}
}
