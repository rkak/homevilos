#include "stdbool.h"
#include "stdint.h"
#include "hal_interrupt.h"
#include "hal_uart.h"

void __attribute__ ((interrupt ("IRQ"))) irq_handler (void)
{
	hal_interrupt_run_handler ();
}

void __attribute__ ((interrupt ("FIQ"))) fiq_handler (void)
{
	while (true);
}
