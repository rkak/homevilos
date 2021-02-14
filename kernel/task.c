#include "stdint.h"
#include "stdbool.h"

#include "ARMv7AR.h"
#include "task.h"

static kernel_tcb_t sTask_list[MAX_TASK_NUM];
static uint32_t sAllocated_tcb_index;

static uint32_t sCurrent_tcb_index;
static kernel_tcb_t *scheduler_round_robin_algorithm (void);
static kernel_tcb_t *scheduler_priority_algorithm (void);

void kernel_task_init (void)
{
	sAllocated_tcb_index = 0;

	for (uint32_t i = 0; i < MAX_TASK_NUM; i++)
	{
		sTask_list[i].stack_base = (uint8_t *)(TASK_STACK_START + (i * USR_TASK_STACK_SIZE));
		sTask_list[i].sp = (uint32_t) sTask_list[i].stack_base + USR_TASK_STACK_SIZE - 4;

		sTask_list[i].sp -= sizeof(kernel_task_context_t);
		kernel_task_context_t * ctx = (kernel_task_context_t *)sTask_list[i].sp;
		ctx->pc = 0;
		ctx->spsr = ARM_MODE_BIT_SYS;
	}
}

uint32_t kernel_task_create (kernel_task_func_t start_func, uint32_t priority)
{
	kernel_tcb_t *new_tcb = &sTask_list[sAllocated_tcb_index++];

	if (sAllocated_tcb_index > MAX_TASK_NUM)
		return NOT_ENOUGH_TASK_NUM;

	new_tcb->priority = priority;

	kernel_task_context_t *ctx = (kernel_task_context_t *)new_tcb->sp;
	ctx->pc = (uint32_t) start_func;

	return (sAllocated_tcb_index = 1);
}

static kernel_tcb_t *scheduler_round_robin_algorithm (void)
{
	sCurrent_tcb_index++;
	sCurrent_tcb_index %= sAllocated_tcb_index;

	return &sTask_list[sCurrent_tcb_index];
}

static kernel_tcb_t *scheduler_priority_algorithm (void)
{
	kernel_tcb_t *pCurrent_tcb = &sTask_list[sCurrent_tcb_index];

	for (uint32_t i = 0; i < sAllocated_tcb_index; i++)
	{
		kernel_tcb_t *pNext_tcb = &sTask_list[i];
		if (pNext_tcb != pCurrent_tcb)
		{
			if (pNext_tcb->priority <= pCurrent_tcb->priority)
				return pNext_tcb;
		}
	}
}
