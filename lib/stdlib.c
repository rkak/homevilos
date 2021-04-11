#include "stdint.h"
#include "stdbool.h"
#include "hal_timer.h"

void delay (uint32_t ms)
{
	uint32_t goal = hal_timer_get_1ms_counter() + ms;

	while (goal != hal_timer_get_1ms_counter ());
}

void memclr (void *data, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		*((char *)data + i) = 0;
	}
}
