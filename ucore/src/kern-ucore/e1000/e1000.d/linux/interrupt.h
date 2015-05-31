#ifndef __LINUX_INTERRUPT_H__
#define __LINUX_INTERRUPT_H__

#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/bitops.h>
#include <linux/preempt.h>
#include <linux/cpumask.h>
#include <linux/irqreturn.h>
#include <linux/irqnr.h>
#include <linux/hardirq.h>
#include <linux/irqflags.h>
#include <linux/smp.h>
#include <linux/percpu.h>
#include <linux/hrtimer.h>
#include <linux/kref.h>
#include <linux/workqueue.h>

#include <linux/atomic.h>
#include <asm/ptrace.h>

#define IRQF_SHARED		0x00000080
#define IRQF_PROBE_SHARED	0x00000100

typedef irqreturn_t (*irq_handler_t)(int, void *);

/**
 * struct irqaction - per interrupt action descriptor
 * @handler:	interrupt handler function
 * @name:	name of the device
 * @dev_id:	cookie to identify the device
 * @percpu_dev_id:	cookie to identify the device
 * @next:	pointer to the next irqaction for shared interrupts
 * @irq:	interrupt number
 * @flags:	flags (see IRQF_* above)
 * @thread_fn:	interrupt handler function for threaded interrupts
 * @thread:	thread pointer for threaded interrupts
 * @thread_flags:	flags related to @thread
 * @thread_mask:	bitmask for keeping track of @thread activity
 * @dir:	pointer to the proc/irq/NN/name entry
 */
struct irqaction {
	irq_handler_t		handler;
	void			*dev_id;
	void __percpu		*percpu_dev_id;
	struct irqaction	*next;
	irq_handler_t		thread_fn;
	struct task_struct	*thread;
	unsigned int		irq;
	unsigned int		flags;
	unsigned long		thread_flags;
	unsigned long		thread_mask;
	const char		*name;
	struct proc_dir_entry	*dir;
} ____cacheline_internodealigned_in_smp;

extern int __must_check
request_threaded_irq(unsigned int irq, irq_handler_t handler,
		     irq_handler_t thread_fn,
		     unsigned long flags, const char *name, void *dev);

static inline int __must_check
request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
	    const char *name, void *dev)
{
	return request_threaded_irq(irq, handler, NULL, flags, name, dev);
}

extern void free_irq(unsigned int, void *);

struct device;

extern int __must_check
devm_request_threaded_irq(struct device *dev, unsigned int irq,
			  irq_handler_t handler, irq_handler_t thread_fn,
			  unsigned long irqflags, const char *devname,
			  void *dev_id);

extern void disable_irq_nosync(unsigned int irq);
extern void disable_irq(unsigned int irq);

extern void enable_irq(unsigned int irq);

/**
 * struct irq_affinity_notify - context for notification of IRQ affinity changes
 * @irq:		Interrupt to which notification applies
 * @kref:		Reference count, for internal use
 * @work:		Work item, for internal use
 * @notify:		Function to be called on change.  This will be
 *			called in process context.
 * @release:		Function to be called on release.  This will be
 *			called in process context.  Once registered, the
 *			structure must only be freed when this function is
 *			called or later.
 */
struct irq_affinity_notify {
	unsigned int irq;
	struct kref kref;
	struct work_struct work;
	void (*notify)(struct irq_affinity_notify *, const cpumask_t *mask);
	void (*release)(struct kref *ref);
};

extern int irq_set_irq_wake(unsigned int irq, unsigned int on);

DECLARE_PER_CPU(struct task_struct *, ksoftirqd);

struct tasklet_struct
{
	struct tasklet_struct *next;
	unsigned long state;
	atomic_t count;
	void (*func)(unsigned long);
	unsigned long data;
};

enum
{
	TASKLET_STATE_SCHED,	/* Tasklet is scheduled for execution */
	TASKLET_STATE_RUN	/* Tasklet is running (SMP only) */
};

static inline void tasklet_unlock_wait(struct tasklet_struct *t)
{
	while (test_bit(TASKLET_STATE_RUN, &(t)->state)) { barrier(); }
}

extern void __tasklet_schedule(struct tasklet_struct *t);

extern void __tasklet_hi_schedule(struct tasklet_struct *t);

extern void __tasklet_hi_schedule_first(struct tasklet_struct *t);

static inline void tasklet_disable_nosync(struct tasklet_struct *t)
{
	atomic_inc(&t->count);
	smp_mb__after_atomic_inc();
}

extern void tasklet_kill(struct tasklet_struct *t);

struct seq_file;

#endif /* ! __LINUX_INTERRUPT_H__ */
