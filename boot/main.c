#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "hal_uart.h"

static void hw_init (void);
extern void hal_interrupt_init (void);
extern void hal_uart_init (void);
extern void hal_uart_put_char (uint8_t ch);
static void printf_test (void);

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

	printf_test ();

	while (true);
}

static void hw_init (void)
{
	hal_interrupt_init ();
	hal_uart_init ();
}

static void printf_test (void)
{
	char *str = "printf pointer test";
	char *nullstr = 0;
	uint32_t i = 5;

	debug_printf ("%s\n", "Hellow printf");
	debug_printf ("output string pointer: %s\n", str);
	debug_printf ("%s is null pointer, %u number\n", nullstr, 10);
	debug_printf ("%u = 5\n", i);
	debug_printf ("dec=%u hex=%x\n", 0xff, 0xff);
	debug_printf ("print zero %u\n", 0);
}
