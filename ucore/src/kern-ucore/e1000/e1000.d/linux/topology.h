#ifndef __LINUX_TOPOLOGY_H__
#define __LINUX_TOPOLOGY_H__

#include <linux/cpumask.h>
#include <linux/bitops.h>
#include <linux/mmzone.h>
#include <linux/smp.h>
#include <linux/percpu.h>
#include <asm/topology.h>

DECLARE_PER_CPU(int, numa_node);

static inline int numa_node_id(void)
{
	return __this_cpu_read(numa_node);
}

static inline int cpu_to_node(int cpu)
{
	return per_cpu(numa_node, cpu);
}

#endif /* ! __LINUX_TOPOLOGY_H__ */
