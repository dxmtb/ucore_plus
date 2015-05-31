#ifndef __ASM_GENERIC_PERCPU_H__
#define __ASM_GENERIC_PERCPU_H__

#include <linux/compiler.h>
#include <linux/threads.h>
#include <linux/percpu-defs.h>

extern unsigned long __per_cpu_offset[NR_CPUS];

#define per_cpu_offset(x) (__per_cpu_offset[x])

#define SHIFT_PERCPU_PTR(__p, __offset)	({				\
	__verify_pcpu_ptr((__p));					\
	RELOC_HIDE((typeof(*(__p)) __kernel __force *)(__p), (__offset)); \
})

/*
 * A percpu variable may point to a discarded regions. The following are
 * established ways to produce a usable pointer from the percpu variable
 * offset.
 */
#define per_cpu(var, cpu) \
	(*SHIFT_PERCPU_PTR(&(var), per_cpu_offset(cpu)))

#define PER_CPU_BASE_SECTION ".data..percpu"

#define PER_CPU_SHARED_ALIGNED_SECTION "..shared_aligned"
#define PER_CPU_ALIGNED_SECTION "..shared_aligned"

#define PER_CPU_FIRST_SECTION "..first"

#ifndef PER_CPU_ATTRIBUTES
#define PER_CPU_ATTRIBUTES
#endif

#endif /* ! __ASM_GENERIC_PERCPU_H__ */
