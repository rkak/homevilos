#ifndef HAL_HALINTERRUPT_H_
#define HAL_HALINTERRUPT_H_

#define INTERRUPT_HANDLER_NUM 255

typedef void (*inter_hdlr_fptr)(void);

void hal_interrupt_init (void);
void hal_interrupt_enable (uint32_t interrupt_num);
void hal_interrupt_disable (uint32_t interrupt_num);
void hal_interrupt_register_handler (inter_hdlr_fptr handler, uint32_t interrupt_num);
void hal_interrupt_run_handler (void);

#endif /* HAL_HALINTERRUPT_H_ */
