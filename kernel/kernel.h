#ifndef KERNEL_KERNEL_H_
#define KERNEL_KERNEL_H_

#include "task.h"
#include "event.h"
#include "msg.h"


void kernel_start (void);
void kernel_yield (void);

void kernel_send_events (uint32_t event_list);
kernel_event_flag_t kernel_wait_events (uint32_t waiting_list);
bool kernel_send_msg (kernel_msg_q_t q_name, void *data, uint32_t count);
uint32_t kernel_recv_msg (kernel_msg_q_t q_name, void *out_data, uint32_t count);

void kernel_lock_sem (void);
void kernel_unlock_sem (void);

#endif /* KERNEL_KERNEL_H_ */
