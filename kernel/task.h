#ifndef KERNEL_TASK_H_
#define KERNEL_TASK_H_

#include "memory_map.h"

#define NOT_ENOUGH_TASK_NUM 0xFFFFFFFF
#define USR_TASK_STACK_SIZE 0x100000
#define MAX_TASK_NUM		(TASK_STACK_SIZE / USR_TASK_STACK_SIZE)

typedef struct kernel_task_context_t
{
	uint32_t spsr;
	uint32_t r0_r12[13];
	uint32_t pc;
} kernel_task_context_t;

typedef struct kernel_tcb_t
{
	uint32_t sp;
	uint8_t *stack_base;
	uint32_t priority;
} kernel_tcb_t;

typedef void (*kernel_task_func_t)(void);

void kernel_task_init (void);
uint32_t kernel_task_create (kernel_task_func_t start_func);
uint32_t kernel_task_prio_create (kernel_task_func_t start_func, uint32_t priority);

uint32_t kernel_task_get_current_task_id (void);

__attribute__ ((naked)) void kernel_task_context_switching (void);
static __attribute__ ((naked)) void save_context (void);
static __attribute__ ((naked)) void restore_context (void);
#endif /* KERNEL_TASK_H_ */
