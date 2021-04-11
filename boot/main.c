#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "hal_uart.h"
#include "hal_timer.h"
#include "task.h"
#include "kernel.h"
#include "event.h"
#include "msg.h"

static void hw_init (void);
extern void hal_interrupt_init (void);
extern void hal_uart_init (void);
extern void hal_uart_put_char (uint8_t ch);
static void printf_test (void);
static void timer_test (void);
static void kernel_init (void);
void user_task0 (void);
void user_task1 (void);
void user_task2 (void);

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

//	printf_test ();

//	timer_test ();
//	while (true);

	kernel_init ();
}

static void hw_init (void)
{
	hal_interrupt_init ();
	hal_uart_init ();
	hal_timer_init ();
}

static void kernel_init (void)
{
	uint32_t task_id;

	kernel_task_init ();
	kernel_event_flag_init ();

	task_id = kernel_task_create (user_task0);
	if (task_id == NOT_ENOUGH_TASK_NUM)
	{
		putstr ("Task0 creation failed\n");
	}

	task_id = kernel_task_create (user_task1);
	if (task_id == NOT_ENOUGH_TASK_NUM)
	{
		putstr ("Task1 creation failed\n");
	}

	task_id = kernel_task_create (user_task2);
	if (task_id == NOT_ENOUGH_TASK_NUM)
	{
		putstr ("Task2 creation failed\n");
	}

	kernel_start ();
}

static void printf_test (void)
{
	char *str = "printf pointer test";
	char *nullstr = 0;
	uint32_t i = 5;
	uint32_t *sysctrl0 = (uint32_t*)0x10001000;

	debug_printf ("%s\n", "Hellow printf");
	debug_printf ("output string pointer: %s\n", str);
	debug_printf ("%s is null pointer, %u number\n", nullstr, 10);
	debug_printf ("%u = 5\n", i);
	debug_printf ("dec=%u hex=%x\n", 0xff, 0xff);
	debug_printf ("print zero %u\n", 0);
	debug_printf ("SYSCTL0 %x\n", *sysctrl0);
}

static void timer_test (void)
{
	while (true)
	{
		debug_printf ("current count : %u\n", hal_timer_get_1ms_counter());
		delay (1000);
	}
}

void user_task0 (void)
{
	uint32_t local = 0;

	debug_printf ("User task #0 SP=0x%x\n", &local);

	uint8_t cmd_buf[16];
	uint32_t cmd_buf_idx = 0;
	uint8_t uartch = 0;

	while (true)
	{
		bool pending_event = true;

		while (pending_event)
		{
			kernel_event_flag_t handle_event = kernel_wait_events (kernel_event_flag_uart_in | kernel_event_flag_cmd_out);
			switch (handle_event)
			{
				case kernel_event_flag_uart_in:
					kernel_recv_msg (KERNEL_MSG_Q_TASK0, &uartch, 1);
					if (uartch == '\r')
					{
						cmd_buf[cmd_buf_idx] = '\0';

						while (true)
						{
							kernel_send_events (kernel_event_flag_cmd_in);

							if (kernel_send_msg (KERNEL_MSG_Q_TASK1, &cmd_buf_idx, 1) == false)
							{
								kernel_yield ();
							}
							else if (kernel_send_msg (KERNEL_MSG_Q_TASK1, cmd_buf, cmd_buf_idx) == false)
							{
								/* rollback for cmd_buf_idx */
								uint8_t rollback;
								kernel_recv_msg (KERNEL_MSG_Q_TASK1, &rollback, 1);
								kernel_yield ();
							}
							else
							{
								break;
							}
						}

						cmd_buf_idx = 0;
					}
					else
					{
						cmd_buf[cmd_buf_idx] = uartch;
						cmd_buf_idx ++;
						cmd_buf_idx %= 16;
					}
					break;
				case kernel_event_flag_cmd_out:
					debug_printf ("\nEvent handled of cmd out by task0\n");
					break;
				default:
					pending_event = false;
					break;
			}
		}
		kernel_yield ();
	}
}

void user_task1 (void)
{
	uint32_t local = 0;

	debug_printf ("User task #1 SP=0x%x\n", &local);

	uint8_t cmd_len = 0;
	uint8_t cmd[16] = {0};

	while (true)
	{
		kernel_event_flag_t handle_event = kernel_wait_events (kernel_event_flag_cmd_in);
		switch (handle_event)
		{
			case kernel_event_flag_cmd_in:
				memclr (cmd, 16);
				kernel_recv_msg (KERNEL_MSG_Q_TASK1, &cmd_len, 1);
				kernel_recv_msg (KERNEL_MSG_Q_TASK1, cmd, cmd_len);
				debug_printf ("\nRecv Cmd : %s\n", cmd);
				break;
		}
		kernel_yield ();
	}
}

void user_task2 (void)
{
	uint32_t local = 0;

	debug_printf ("User task #2 SP=0x%x\n", &local);
	while (true)
	{
		kernel_yield ();
	}
}
