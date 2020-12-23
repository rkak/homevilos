#include "stdint.h"

#include "hal_uart.h"

static void hw_init (void);
extern void hal_uart_init (void);
extern void hal_uart_put_char (uint8_t ch);

void main (void)
{
	uint32_t i = 100;

	hw_init ();

	while (i--)
	{
		hal_uart_put_char ('N');
	}
}

static void hw_init (void)
{
	hal_uart_init ();
}
