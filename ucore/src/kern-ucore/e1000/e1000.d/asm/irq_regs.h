#ifndef __ASM_IRQ_REGS_H__
#define __ASM_IRQ_REGS_H__

#include <asm/percpu.h>

DECLARE_PER_CPU(struct pt_regs *, irq_regs);

static inline struct pt_regs *get_irq_regs(void)
{
	return this_cpu_read(irq_regs);
}

#endif /* ! __ASM_IRQ_REGS_H__ */
