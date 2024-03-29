#include "stdint.h"
#include "memio.h"
#include "interrupt.h"
#include "hal_interrupt.h"
#include "arm_cpu.h"
#include "hal_uart.h"

extern volatile gic_cput_t *gic_cpu;
extern volatile gic_dist_t *gic_dist;

static inter_hdlr_fptr sHandlers[INTERRUPT_HANDLER_NUM];

void hal_interrupt_init (void)
{
	gic_cpu->cpu_control.bits.enable = 1;
	gic_cpu->priority_mask.bits.priority_mask = GIC_PRIORITY_MASK_NONE;
	gic_dist->distributor_ctrl.bits.enable = 1;

	for (uint32_t i = 0; i < INTERRUPT_HANDLER_NUM ; i++)
	{
		sHandlers[i] = NULL;
	}

	enable_irq ();
}

void hal_interrupt_enable (uint32_t interrupt_num)
{
	if ((interrupt_num < GIC_IRQ_START) || (GIC_IRQ_END < interrupt_num))
		return;

	uint32_t bit_num = interrupt_num - GIC_IRQ_START;

	if (bit_num < GIC_IRQ_START)
	{
		SET_BIT (gic_dist->set_enable1, bit_num);
	}
	else
	{
		bit_num -= GIC_IRQ_START;
		SET_BIT (gic_dist->set_enable2, bit_num);
	}
}

void hal_interrupt_disable (uint32_t interrupt_num)
{
	if ((interrupt_num < GIC_IRQ_START) || (GIC_IRQ_END < interrupt_num))
		return;

	uint32_t bit_num = interrupt_num - GIC_IRQ_START;

	if (bit_num < GIC_IRQ_START)
	{
		CLR_BIT (gic_dist->set_enable1, bit_num);
	}
	else
	{
		bit_num -= GIC_IRQ_START;
		CLR_BIT (gic_dist->set_enable2, bit_num);
	}
}

void hal_interrupt_register_handler (inter_hdlr_fptr handler, uint32_t interrupt_num)
{
	sHandlers[interrupt_num] = handler;
}

void hal_interrupt_run_handler (void)
{
	uint32_t interrupt_num = gic_cpu->interrupt_ack.bits.interrupt_id;

	if (sHandlers[interrupt_num] != NULL)
		sHandlers[interrupt_num] ();

	gic_cpu->end_of_interrupt.bits.interrupt_id = interrupt_num;
}

