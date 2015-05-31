#ifndef __ASM_TOPOLOGY_H__
#define __ASM_TOPOLOGY_H__

#  define ENABLE_TOPO_DEFINES

/*
 * to preserve the visibility of NUMA_NO_NODE definition,
 * moved to there from here.  May be used independent of
 * CONFIG_NUMA.
 */
#include <linux/numa.h>

#include <linux/cpumask.h>

#include <asm/mpspec.h>

DECLARE_EARLY_PER_CPU(int, x86_cpu_to_node_map);

extern cpumask_var_t node_to_cpumask_map[MAX_NUMNODES];

static inline const struct cpumask *cpumask_of_node(int node)
{
	return node_to_cpumask_map[node];
}

#define node_distance(a, b) __node_distance(a, b)

#include <asm-generic/topology.h>

#define topology_physical_package_id(cpu)	(cpu_data(cpu).phys_proc_id)
#define topology_core_id(cpu)			(cpu_data(cpu).cpu_core_id)
#define topology_core_cpumask(cpu)		(per_cpu(cpu_core_map, cpu))
#define topology_thread_cpumask(cpu)		(per_cpu(cpu_sibling_map, cpu))

struct pci_bus;

#endif /* ! __ASM_TOPOLOGY_H__ */
