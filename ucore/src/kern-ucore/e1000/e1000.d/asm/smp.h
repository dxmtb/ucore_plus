#ifndef __ASM_SMP_H__
#define __ASM_SMP_H__

#define _ASM_X86_SMP_H

#include <linux/cpumask.h>
#include <linux/init.h>
#include <asm/percpu.h>

# include <asm/mpspec.h>
# include <asm/apic.h>

#  include <asm/io_apic.h>

#include <asm/thread_info.h>
#include <asm/cpumask.h>
#include <asm/cpufeature.h>

extern int smp_num_siblings;

DECLARE_PER_CPU_READ_MOSTLY(cpumask_var_t, cpu_sibling_map);
DECLARE_PER_CPU_READ_MOSTLY(cpumask_var_t, cpu_core_map);

DECLARE_PER_CPU_READ_MOSTLY(cpumask_var_t, cpu_llc_shared_map);

DECLARE_PER_CPU_READ_MOSTLY(int, cpu_number);

DECLARE_EARLY_PER_CPU_READ_MOSTLY(u16, x86_bios_cpu_apicid);

struct task_struct;

struct smp_ops {
	void (*smp_prepare_boot_cpu)(void);
	void (*smp_prepare_cpus)(unsigned max_cpus);
	void (*smp_cpus_done)(unsigned max_cpus);

	void (*stop_other_cpus)(int wait);
	void (*smp_send_reschedule)(int cpu);

	int (*cpu_up)(unsigned cpu, struct task_struct *tidle);
	int (*cpu_disable)(void);
	void (*cpu_die)(unsigned int cpu);
	void (*play_dead)(void);

	void (*send_call_func_ipi)(const struct cpumask *mask);
	void (*send_call_func_single_ipi)(int cpu);
};

extern struct smp_ops smp_ops;

#define raw_smp_processor_id() (this_cpu_read(cpu_number))

#endif /* ! __ASM_SMP_H__ */
