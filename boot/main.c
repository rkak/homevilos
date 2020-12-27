#include "stdint.h"
#include "hal_uart.h"

#include "stdio.h"

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
	hal_uart_put_char ('\n');

	putstr("Hello World!\n");

	i = 100;
	while (i--)
	{
		uint8_t ch = hal_uart_get_char ();
		hal_uart_put_char (ch);
	}
}

static void hw_init (void)
{
	hal_uart_init ();
}
