#include "stdint.h"
#include "stdbool.h"
#include "sync.h"

#define DEF_SEM_MAX		8

static int32_t sSem_max;
static int32_t sSem;

kernel_mutex_t sMutex;

void kernel_sem_init (int32_t max)
{
	sSem_max = (max <= 0) ? DEF_SEM_MAX : max;

	sSem_max = (max >= DEF_SEM_MAX) ? DEF_SEM_MAX : max;

	sSem = sSem_max;
}

bool kernel_sem_test (void)
{
	if (sSem <= 0)
		return false;

	sSem--;

	return true;
}

void kernel_sem_release (void)
{
	sSem++;

	if (sSem >= sSem_max)
	{
		sSem = sSem_max;
	}
}

void kernel_mutex_init (void)
{
	sMutex.owner = 0;
	sMutex.lock = false;
}

bool kernel_mutex_lock (uint32_t owner)
{
	if (sMutex.lock)
	{
		return false;
	}

	sMutex.owner = owner;
	sMutex.lock = true;
	return true;
}

bool kernel_mutex_unlock (uint32_t owner)
{
	if (sMutex.owner == owner)
	{
		sMutex.lock = false;
		return true;
	}

	return false;
}
