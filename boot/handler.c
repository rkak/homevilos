#include "stdbool.h"
#include "stdint.h"
#include "hal_interrupt.h"
#include "hal_uart.h"

__attribute__ ((interrupt ("IRQ"))) void irq_handler (void)
{
	debug_printf ("hal interrupt run handler start");
	hal_interrupt_run_handler ();
}

__attribute__ ((interrupt ("FIQ"))) void fiq_handler (void)
{
	while (true);
}
