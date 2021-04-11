#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

#include "msg.h"


kernel_cir_q_t sMsg_q [KERNEL_MSG_Q_NUM];

void kernel_msg_q_init (void)
{
	for (uint32_t i = 0; i < KERNEL_MSG_Q_NUM; i++)
	{
		sMsg_q[i].front = 0;
		sMsg_q[i].rear = 0;
		memclr (sMsg_q[i].queue, MSG_Q_SIZE_BYTE);
	}
}

bool kernel_msg_q_is_empty (kernel_msg_q_t q_name)
{
	if (q_name >= KERNEL_MSG_Q_NUM)
		return false;

	if (sMsg_q[q_name].front == sMsg_q[q_name].rear)
		return true;

	return false;
}

bool kernel_msg_q_is_full (kernel_msg_q_t q_name)
{
	if (q_name >= KERNEL_MSG_Q_NUM)
		return false;

	if (((sMsg_q[q_name].rear + 1) % MSG_Q_SIZE_BYTE) == sMsg_q[q_name].front)
		return true;

	return false;
}

bool kernel_msg_q_enqueue (kernel_msg_q_t q_name, uint8_t data)
{
	if (q_name >= KERNEL_MSG_Q_NUM)
		return false;

	if (kernel_msg_q_is_full (q_name))
		return false;

	sMsg_q[q_name].rear++;
	sMsg_q[q_name].rear %= MSG_Q_SIZE_BYTE;

	uint32_t idx = sMsg_q[q_name].rear;
	sMsg_q[q_name].queue[idx] = data;

	return true;
}

bool kernel_msg_q_dequeue (kernel_msg_q_t q_name, uint8_t *out_data)
{
	if (q_name >= KERNEL_MSG_Q_NUM)
		return false;

	if (kernel_msg_q_is_empty (q_name))
		return false;

	sMsg_q[q_name].front++;
	sMsg_q[q_name].front %= MSG_Q_SIZE_BYTE;
	
	uint32_t idx = sMsg_q[q_name].front;
	*out_data = sMsg_q[q_name].queue[idx];

	return true;
}
