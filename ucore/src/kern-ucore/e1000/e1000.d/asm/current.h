#ifndef __ASM_CURRENT_H__
#define __ASM_CURRENT_H__

#include <linux/compiler.h>
#include <asm/percpu.h>

struct task_struct;

DECLARE_PER_CPU(struct task_struct *, current_task);

static __always_inline struct task_struct *get_current(void)
{
	return this_cpu_read_stable(current_task);
}

#define current get_current()

#endif /* ! __ASM_CURRENT_H__ */
