#include "stdint.h"
#include "uart.h"
#include "hal_uart.h"

extern volatile PL011_t *uart;

void hal_uart_init (void)
{
	// enable uart
	uart->uartcr.bits.UARTEN = 0;
	uart->uartcr.bits.TXE = 1;
	uart->uartcr.bits.RXE = 1;
	uart->uartcr.bits.UARTEN = 1;
}

void hal_uart_put_char (uint8_t ch)
{
	while (uart->uartfr.bits.TXFF);
	uart->uartdr.all = (ch & 0xff);
}
