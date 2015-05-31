#ifndef __LINUX_VTIME_H__
#define __LINUX_VTIME_H__

struct task_struct;

static inline void vtime_account_system(struct task_struct *tsk) { }

static inline void vtime_account_irq_enter(struct task_struct *tsk) { }

static inline void vtime_account_irq_exit(struct task_struct *tsk)
{
	/* On hard|softirq exit we always account to hard|softirq cputime */
	vtime_account_system(tsk);
}

static inline void irqtime_account_irq(struct task_struct *tsk) { }

#endif /* ! __LINUX_VTIME_H__ */
