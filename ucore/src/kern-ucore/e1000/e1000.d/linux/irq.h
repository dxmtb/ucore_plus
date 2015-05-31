#ifndef __LINUX_IRQ_H__
#define __LINUX_IRQ_H__

#include <linux/smp.h>
#include <linux/linkage.h>
#include <linux/cache.h>
#include <linux/spinlock.h>
#include <linux/cpumask.h>
#include <linux/gfp.h>
#include <linux/irqreturn.h>
#include <linux/irqnr.h>
#include <linux/errno.h>
#include <linux/topology.h>
#include <linux/wait.h>

#include <asm/irq.h>
#include <asm/ptrace.h>
#include <asm/irq_regs.h>

struct seq_file;
struct module;
struct irq_desc;
struct irq_data;
typedef	void (*irq_flow_handler_t)(unsigned int irq,
					    struct irq_desc *desc);

#define IRQ_NO_BALANCING_MASK	(IRQ_PER_CPU | IRQ_NO_BALANCING)

struct msi_desc;
struct irq_domain;

/**
 * struct irq_data - per irq and irq chip data passed down to chip functions
 * @irq:		interrupt number
 * @hwirq:		hardware interrupt number, local to the interrupt domain
 * @node:		node index useful for balancing
 * @state_use_accessors: status information for irq chip functions.
 *			Use accessor functions to deal with it
 * @chip:		low level interrupt hardware access
 * @domain:		Interrupt translation domain; responsible for mapping
 *			between hwirq number and linux irq number.
 * @handler_data:	per-IRQ data for the irq_chip methods
 * @chip_data:		platform-specific per-chip private data for the chip
 *			methods, to allow shared chip implementations
 * @msi_desc:		MSI descriptor
 * @affinity:		IRQ affinity on SMP
 *
 * The fields here need to overlay the ones in irq_desc until we
 * cleaned up the direct references and switched everything over to
 * irq_data.
 */
struct irq_data {
	unsigned int		irq;
	unsigned long		hwirq;
	unsigned int		node;
	unsigned int		state_use_accessors;
	struct irq_chip		*chip;
	struct irq_domain	*domain;
	void			*handler_data;
	void			*chip_data;
	struct msi_desc		*msi_desc;
	cpumask_var_t		affinity;
};

/**
 * struct irq_chip - hardware interrupt chip descriptor
 *
 * @name:		name for /proc/interrupts
 * @irq_startup:	start up the interrupt (defaults to ->enable if NULL)
 * @irq_shutdown:	shut down the interrupt (defaults to ->disable if NULL)
 * @irq_enable:		enable the interrupt (defaults to chip->unmask if NULL)
 * @irq_disable:	disable the interrupt
 * @irq_ack:		start of a new interrupt
 * @irq_mask:		mask an interrupt source
 * @irq_mask_ack:	ack and mask an interrupt source
 * @irq_unmask:		unmask an interrupt source
 * @irq_eoi:		end of interrupt
 * @irq_set_affinity:	set the CPU affinity on SMP machines
 * @irq_retrigger:	resend an IRQ to the CPU
 * @irq_set_type:	set the flow type (IRQ_TYPE_LEVEL/etc.) of an IRQ
 * @irq_set_wake:	enable/disable power-management wake-on of an IRQ
 * @irq_bus_lock:	function to lock access to slow bus (i2c) chips
 * @irq_bus_sync_unlock:function to sync and unlock slow bus (i2c) chips
 * @irq_cpu_online:	configure an interrupt source for a secondary CPU
 * @irq_cpu_offline:	un-configure an interrupt source for a secondary CPU
 * @irq_suspend:	function called from core code on suspend once per chip
 * @irq_resume:		function called from core code on resume once per chip
 * @irq_pm_shutdown:	function called from core code on shutdown once per chip
 * @irq_print_chip:	optional to print special chip info in show_interrupts
 * @flags:		chip specific flags
 */
struct irq_chip {
	const char	*name;
	unsigned int	(*irq_startup)(struct irq_data *data);
	void		(*irq_shutdown)(struct irq_data *data);
	void		(*irq_enable)(struct irq_data *data);
	void		(*irq_disable)(struct irq_data *data);

	void		(*irq_ack)(struct irq_data *data);
	void		(*irq_mask)(struct irq_data *data);
	void		(*irq_mask_ack)(struct irq_data *data);
	void		(*irq_unmask)(struct irq_data *data);
	void		(*irq_eoi)(struct irq_data *data);

	int		(*irq_set_affinity)(struct irq_data *data, const struct cpumask *dest, bool force);
	int		(*irq_retrigger)(struct irq_data *data);
	int		(*irq_set_type)(struct irq_data *data, unsigned int flow_type);
	int		(*irq_set_wake)(struct irq_data *data, unsigned int on);

	void		(*irq_bus_lock)(struct irq_data *data);
	void		(*irq_bus_sync_unlock)(struct irq_data *data);

	void		(*irq_cpu_online)(struct irq_data *data);
	void		(*irq_cpu_offline)(struct irq_data *data);

	void		(*irq_suspend)(struct irq_data *data);
	void		(*irq_resume)(struct irq_data *data);
	void		(*irq_pm_shutdown)(struct irq_data *data);

	void		(*irq_print_chip)(struct irq_data *data, struct seq_file *p);

	unsigned long	flags;
};

#include <linux/irqdesc.h>

/*
 * Pick up the arch-dependent methods:
 */
#include <asm/hw_irq.h>

struct irqaction;

extern void
irq_set_chip_and_handler_name(unsigned int irq, struct irq_chip *chip,
			      irq_flow_handler_t handle, const char *name);

extern void
__irq_set_handler(unsigned int irq, irq_flow_handler_t handle, int is_chained,
		  const char *name);

void irq_modify_status(unsigned int irq, unsigned long clr, unsigned long set);

static inline void irq_set_status_flags(unsigned int irq, unsigned long set)
{
	irq_modify_status(irq, 0, set);
}

static inline void irq_clear_status_flags(unsigned int irq, unsigned long clr)
{
	irq_modify_status(irq, clr, 0);
}

/*
 * Dynamic irq helper functions. Obsolete. Use irq_alloc_desc* and
 * irq_free_desc instead.
 */
extern void dynamic_irq_cleanup(unsigned int irq);

extern struct irq_data *irq_get_irq_data(unsigned int irq);

void irq_free_descs(unsigned int irq, unsigned int cnt);
int irq_reserve_irqs(unsigned int from, unsigned int cnt);

#endif /* ! __LINUX_IRQ_H__ */
