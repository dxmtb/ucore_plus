#ifndef __ASM_NUMA_H__
#define __ASM_NUMA_H__

#include <linux/nodemask.h>

#include <asm/topology.h>
#include <asm/apicdef.h>

/*
 * __apicid_to_node[] stores the raw mapping between physical apicid and
 * node and is used to initialize cpu_to_node mapping.
 *
 * The mapping may be overridden by apic->numa_cpu_node() on 32bit and thus
 * should be accessed by the accessors - set_apicid_to_node() and
 * numa_cpu_node().
 */
extern s16 __apicid_to_node[MAX_LOCAL_APIC];

#endif /* ! __ASM_NUMA_H__ */
