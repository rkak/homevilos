#ifndef KERNEL_MSG_H_
#define KERNEL_MSG_H_

#define MSG_Q_SIZE_BYTE			512

typedef enum kernel_msg_q_t
{
	KERNEL_MSG_Q_TASK0,
	KERNEL_MSG_Q_TASK1,
	KERNEL_MSG_Q_TASK2,

	KERNEL_MSG_Q_NUM
} kernel_msg_q_t;

typedef struct kernel_cir_q_t
{
	uint32_t front;
	uint32_t rear;
	uint8_t  queue[MSG_Q_SIZE_BYTE];
} kernel_cir_q_t;

void kernel_msg_q_init (void);
bool kernel_msg_q_is_empty (kernel_msg_q_t q_name);
bool kernel_msg_q_is_full (kernel_msg_q_t q_name);
bool kernel_msg_q_enqueue (kernel_msg_q_t q_name, uint8_t data);
bool kernel_msg_q_dequeue (kernel_msg_q_t q_name, uint8_t *out_data);

#endif /* KERNEL_MSG_H_ */
