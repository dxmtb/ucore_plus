#ifndef __LINUX_VMSTAT_H__
#define __LINUX_VMSTAT_H__

#include <linux/types.h>
#include <linux/percpu.h>
#include <linux/mm.h>
#include <linux/mmzone.h>
#include <linux/vm_event_item.h>
#include <linux/atomic.h>

struct vm_event_state {
	unsigned long event[NR_VM_EVENT_ITEMS];
};

DECLARE_PER_CPU(struct vm_event_state, vm_event_states);

/*
 * Zone based page accounting with per cpu differentials.
 */
extern atomic_long_t vm_stat[NR_VM_ZONE_STAT_ITEMS];

static inline unsigned long zone_page_state(struct zone *zone,
					enum zone_stat_item item)
{
	long x = atomic_long_read(&zone->vm_stat[item]);
#ifdef CONFIG_SMP
	if (x < 0)
		x = 0;
#endif
	return x;
}

void __mod_zone_page_state(struct zone *, enum zone_stat_item item, int);

void inc_zone_page_state(struct page *, enum zone_stat_item);
void dec_zone_page_state(struct page *, enum zone_stat_item);

#endif /* ! __LINUX_VMSTAT_H__ */
