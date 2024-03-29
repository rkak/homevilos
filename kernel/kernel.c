#include "stdint.h"
#include "stdbool.h"
#include "memio.h"
#include "kernel.h"
#include "msg.h"
#include "sync.h"

void kernel_start (void)
{
	kernel_task_start ();
}

void kernel_yield (void)
{
	kernel_task_scheduler ();
}

void kernel_send_events (uint32_t event_list)
{
	for (uint32_t i = 0; i < 32; i++)
	{
		if ((event_list >> i) & 1)
		{
			kernel_event_flag_t sending_event = kernel_event_flag_empty;
			sending_event = (kernel_event_flag_t) SET_BIT (sending_event, i);
			kernel_event_flag_set (sending_event);
		}
	}
}

kernel_event_flag_t kernel_wait_events (uint32_t waiting_list)
{
	for (uint32_t i = 0; i < 32; i++)
	{
		if ((waiting_list >> i) & 1)
		{
			kernel_event_flag_t waiting_event = kernel_event_flag_empty;
			waiting_event = (kernel_event_flag_t) SET_BIT (waiting_event, i);

			if (kernel_event_flag_check (waiting_event))
			{
				return waiting_event;
			}
		}
	}

	return kernel_event_flag_empty;
}

bool kernel_send_msg (kernel_msg_q_t q_name, void *data, uint32_t count)
{
	uint8_t *d = (uint8_t *) data;

	for (uint32_t i = 0; i < count; i++)
	{
		if (kernel_msg_q_enqueue(q_name, *d) == false)
		{
			for (uint32_t j = 0; j < i; j++)
			{
				uint8_t rollback;
				kernel_msg_q_dequeue (q_name, &rollback);
			}
			return false;
		}
		d++;
	}

	return true;
}

uint32_t kernel_recv_msg (kernel_msg_q_t q_name, void *out_data, uint32_t count)
{
	uint8_t *d = (uint8_t *) out_data;

	for (uint32_t i = 0; i < count; i++)
	{
		if (kernel_msg_q_dequeue (q_name, d) == false)
		{
			return i;
		}
		d++;
	}

	return count;
}

void kernel_lock_sem (void)
{
	while (kernel_sem_test () == false)
	{
		kernel_yield ();
	}
}

void kernel_unlock_sem (void)
{
	kernel_sem_release ();
}

void kernel_lock_mutex (void)
{
	while (true)
	{
		uint32_t current_task_id = kernel_task_get_current_task_id ();
		if (kernel_mutex_lock (current_task_id) == false)
			kernel_yield ();
		else
			break;
	}
}

void kernel_unlock_mutex (void)
{
	uint32_t current_task_id = kernel_task_get_current_task_id ();

	if (kernel_mutex_unlock (current_task_id) == false)
		kernel_yield ();
}
