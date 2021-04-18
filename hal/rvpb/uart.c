#include "stdint.h"
#include "uart.h"
#include "hal_uart.h"
#include "hal_interrupt.h"
#include "event.h"
#include "msg.h"

extern volatile PL011_t *uart;

static void uart_interrupt_handler (void);

void hal_uart_init (void)
{
	// enable uart
	uart->uartcr.bits.UARTEN = 0;
	uart->uartcr.bits.TXE = 1;
	uart->uartcr.bits.RXE = 1;
	uart->uartcr.bits.UARTEN = 1;

	// enable input interrupt
	uart->uartimsc.bits.RXIM = 1;

	// register uart interrupt handler
	hal_interrupt_enable (UART_INTERRUPT0);
	hal_interrupt_register_handler (uart_interrupt_handler, UART_INTERRUPT0);
}

void hal_uart_put_char (uint8_t ch)
{
	while (uart->uartfr.bits.TXFF);
	uart->uartdr.all = (ch & 0xff);
}

uint8_t hal_uart_get_char (void)
{
	uint8_t data;

	while (uart->uartfr.bits.RXFE);

#ifdef HAL_UART_THIRD_OPTIMIZE
	data = uart->uartdr.all;
#endif /* HAL_UART_THIRD_OPTIMIZE */

	/* check for an error flag */
#ifdef HAL_UART_FIRST_OPTIMIZE
	if (uart->uartdr.bits.BE || uart->uartdr.bits.FE ||
			uart->uartdr.bits.OE || uart->uartdr.bits.PE)
	{
		/* clear the error */
		uart->uartrsr.bits.BE = 1;
		uart->uartrsr.bits.FE = 1;
		uart->uartrsr.bits.OE = 1;
		uart->uartrsr.bits.PE = 1;
		return 0;
	}
#endif /* HAL_UART_FIRST_OPTIMIZE */

#ifdef HAL_UART_SECOND_OPTIMIZE
	if (uart->uartdr.all & 0xffffff00)
	{
		uart->uartrsr.all = 0xff;
		return 0;
	}
#endif /* HAL_UART_SECOND_OPTIMIZE */

#ifdef HAL_UART_THIRD_OPTIMIZE
	if (data & 0xffffff00)
	{
		uart->uartrsr.all = 0xff;
		return 0;
	}
#endif /* HAL_UART_THIRD_OPTIMIZE */

#if HAL_UART_FIRST_OPTIMIZE || HAL_UART_SECOND_OPTIMIZE
	data = uart->uartdr.bits.DATA;
	return data;
#endif

#if HAL_UART_THIRD_OPTIMIZE
	return (uint8_t)(data & 0xff);
#endif /* HAL_UART_THIRD_OPTIMIZE */
}

static void uart_interrupt_handler (void)
{
	uint8_t ch = hal_uart_get_char ();

	if (ch == 'U')
	{
		kernel_send_events (kernel_event_flag_unlock);
		return;
	}

	if (ch == 'X')
	{
		kernel_send_events (kernel_event_flag_cmd_out);
		return;
	}

	hal_uart_put_char (ch);

	kernel_send_msg (KERNEL_MSG_Q_TASK0, &ch, 1);
	kernel_send_events (kernel_event_flag_uart_in);
}
