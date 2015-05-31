#ifndef __ASM_HW_IRQ_H__
#define __ASM_HW_IRQ_H__

#include <asm/irq_vectors.h>

#include <linux/percpu.h>
#include <linux/profile.h>
#include <linux/smp.h>

#include <linux/atomic.h>
#include <asm/irq.h>
#include <asm/sections.h>

struct io_apic_irq_attr {
	int ioapic;
	int ioapic_pin;
	int trigger;
	int polarity;
};

struct irq_data;

#endif /* ! __ASM_HW_IRQ_H__ */
