#ifndef __LINUX_HARDIRQ_H__
#define __LINUX_HARDIRQ_H__

#include <linux/preempt.h>
#include <linux/lockdep.h>
#include <linux/ftrace_irq.h>
#include <linux/vtime.h>
#include <asm/hardirq.h>

/*
 * We put the hardirq and softirq counter into the preemption
 * counter. The bitmask has the following meaning:
 *
 * - bits 0-7 are the preemption count (max preemption depth: 256)
 * - bits 8-15 are the softirq count (max # of softirqs: 256)
 *
 * The hardirq count can in theory reach the same as NR_IRQS.
 * In reality, the number of nested IRQS is limited to the stack
 * size as well. For archs with over 1000 IRQS it is not practical
 * to expect that they will all nest. We give a max of 10 bits for
 * hardirq nesting. An arch may choose to give less than 10 bits.
 * m68k expects it to be 8.
 *
 * - bits 16-25 are the hardirq count (max # of nested hardirqs: 1024)
 * - bit 26 is the NMI_MASK
 * - bit 27 is the PREEMPT_ACTIVE flag
 *
 * PREEMPT_MASK: 0x000000ff
 * SOFTIRQ_MASK: 0x0000ff00
 * HARDIRQ_MASK: 0x03ff0000
 *     NMI_MASK: 0x04000000
 */
#define PREEMPT_BITS	8
#define SOFTIRQ_BITS	8
#define NMI_BITS	1

#define MAX_HARDIRQ_BITS 10

#ifndef HARDIRQ_BITS
# define HARDIRQ_BITS	MAX_HARDIRQ_BITS
#endif

#define PREEMPT_SHIFT	0
#define SOFTIRQ_SHIFT	(PREEMPT_SHIFT + PREEMPT_BITS)
#define HARDIRQ_SHIFT	(SOFTIRQ_SHIFT + SOFTIRQ_BITS)
#define NMI_SHIFT	(HARDIRQ_SHIFT + HARDIRQ_BITS)

#define __IRQ_MASK(x)	((1UL << (x))-1)

#define SOFTIRQ_MASK	(__IRQ_MASK(SOFTIRQ_BITS) << SOFTIRQ_SHIFT)
#define HARDIRQ_MASK	(__IRQ_MASK(HARDIRQ_BITS) << HARDIRQ_SHIFT)
#define NMI_MASK	(__IRQ_MASK(NMI_BITS)     << NMI_SHIFT)

#define softirq_count()	(preempt_count() & SOFTIRQ_MASK)
#define irq_count()	(preempt_count() & (HARDIRQ_MASK | SOFTIRQ_MASK \
				 | NMI_MASK))

#define in_softirq()		(softirq_count())
#define in_interrupt()		(irq_count())

extern void synchronize_irq(unsigned int irq);

#endif /* ! __LINUX_HARDIRQ_H__ */
