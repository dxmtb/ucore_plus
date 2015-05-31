#ifndef __ASM_APIC_H__
#define __ASM_APIC_H__

#include <linux/cpumask.h>
#include <linux/pm.h>

#include <asm/alternative.h>
#include <asm/cpufeature.h>
#include <asm/processor.h>
#include <asm/apicdef.h>
#include <linux/atomic.h>
#include <asm/fixmap.h>
#include <asm/mpspec.h>
#include <asm/msr.h>

#define APIC_DEBUG   2

extern unsigned int apic_verbosity;

extern int disable_apic;

extern void __inquire_remote_apic(int apicid);

/*
 * Copyright 2004 James Cleverdon, IBM.
 * Subject to the GNU Public License, v.2
 *
 * Generic APIC sub-arch data struct.
 *
 * Hacked for x86-64 by James Cleverdon from i386 architecture code by
 * Martin Bligh, Andi Kleen, James Bottomley, John Stultz, and
 * James Cleverdon.
 */
struct apic {
	char *name;

	int (*probe)(void);
	int (*acpi_madt_oem_check)(char *oem_id, char *oem_table_id);
	int (*apic_id_valid)(int apicid);
	int (*apic_id_registered)(void);

	u32 irq_delivery_mode;
	u32 irq_dest_mode;

	const struct cpumask *(*target_cpus)(void);

	int disable_esr;

	int dest_logical;
	unsigned long (*check_apicid_used)(physid_mask_t *map, int apicid);
	unsigned long (*check_apicid_present)(int apicid);

	void (*vector_allocation_domain)(int cpu, struct cpumask *retmask,
					 const struct cpumask *mask);
	void (*init_apic_ldr)(void);

	void (*ioapic_phys_id_map)(physid_mask_t *phys_map, physid_mask_t *retmap);

	void (*setup_apic_routing)(void);
	int (*multi_timer_check)(int apic, int irq);
	int (*cpu_present_to_apicid)(int mps_cpu);
	void (*apicid_to_cpu_present)(int phys_apicid, physid_mask_t *retmap);
	void (*setup_portio_remap)(void);
	int (*check_phys_apicid_present)(int phys_apicid);
	void (*enable_apic_mode)(void);
	int (*phys_pkg_id)(int cpuid_apic, int index_msb);

	/*
	 * When one of the next two hooks returns 1 the apic
	 * is switched to this. Essentially they are additional
	 * probe functions:
	 */
	int (*mps_oem_check)(struct mpc_table *mpc, char *oem, char *productid);

	unsigned int (*get_apic_id)(unsigned long x);
	unsigned long (*set_apic_id)(unsigned int id);
	unsigned long apic_id_mask;

	int (*cpu_mask_to_apicid_and)(const struct cpumask *cpumask,
				      const struct cpumask *andmask,
				      unsigned int *apicid);

	/* ipi */
	void (*send_IPI_mask)(const struct cpumask *mask, int vector);
	void (*send_IPI_mask_allbutself)(const struct cpumask *mask,
					 int vector);
	void (*send_IPI_allbutself)(int vector);
	void (*send_IPI_all)(int vector);
	void (*send_IPI_self)(int vector);

	/* wakeup_secondary_cpu */
	int (*wakeup_secondary_cpu)(int apicid, unsigned long start_eip);

	int trampoline_phys_low;
	int trampoline_phys_high;

	void (*wait_for_init_deassert)(atomic_t *deassert);
	void (*smp_callin_clear_local_apic)(void);
	void (*inquire_remote_apic)(int apicid);

	/* apic ops */
	u32 (*read)(u32 reg);
	void (*write)(u32 reg, u32 v);
	/*
	 * ->eoi_write() has the same signature as ->write().
	 *
	 * Drivers can support both ->eoi_write() and ->write() by passing the same
	 * callback value. Kernel can override ->eoi_write() and fall back
	 * on write for EOI.
	 */
	void (*eoi_write)(u32 reg, u32 v);
	u64 (*icr_read)(void);
	void (*icr_write)(u32 low, u32 high);
	void (*wait_icr_idle)(void);
	u32 (*safe_wait_icr_idle)(void);

#ifdef CONFIG_X86_32
	/*
	 * Called very early during boot from get_smp_config().  It should
	 * return the logical apicid.  x86_[bios]_cpu_to_apicid is
	 * initialized before this function is called.
	 *
	 * If logical apicid can't be determined that early, the function
	 * may return BAD_APICID.  Logical apicid will be configured after
	 * init_apic_ldr() while bringing up CPUs.  Note that NUMA affinity
	 * won't be applied properly during early boot in this case.
	 */
	int (*x86_32_early_logical_apicid)(int cpu);

	/*
	 * Optional method called from setup_local_APIC() after logical
	 * apicid is guaranteed to be known to initialize apicid -> node
	 * mapping if NUMA initialization hasn't done so already.  Don't
	 * add new users.
	 */
	int (*x86_32_numa_cpu_node)(int cpu);
#endif
};

/*
 * Pointer to the local APIC driver in use on this system (there's
 * always just one such driver in use - the kernel decides via an
 * early probing process which one it picks - and then sticks to it):
 */
extern struct apic *apic;

static inline u32 apic_read(u32 reg)
{
	return apic->read(reg);
}

static inline void apic_eoi(void)
{
	apic->eoi_write(APIC_EOI, APIC_EOI_ACK);
}

#include <asm/smp.h>

DECLARE_EARLY_PER_CPU_READ_MOSTLY(u16, x86_bios_cpu_apicid);

#endif /* ! __ASM_APIC_H__ */
