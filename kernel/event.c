#include "stdint.h"
#include "stdbool.h"

#include "stdio.h"
#include "event.h"

static uint32_t sEvent_flag;

void kernel_event_flag_init (void)
{
	sEvent_flag = 0;
}

void kernel_event_flag_set (kernel_event_flag_t event)
{
	sEvent_flag |= (uint32_t) event;
}

void kernel_event_flag_clear (kernel_event_flag_t event)
{
	sEvent_flag &= ~((uint32_t) event);
}

bool kernel_event_flag_check (kernel_event_flag_t event)
{
	if (sEvent_flag & (uint32_t) event)
	{
		kernel_event_flag_clear (event);
		return true;
	}

	return false;
}
