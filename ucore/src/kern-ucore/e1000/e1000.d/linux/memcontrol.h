#ifndef __LINUX_MEMCONTROL_H__
#define __LINUX_MEMCONTROL_H__

#include <linux/cgroup.h>
#include <linux/vm_event_item.h>
#include <linux/hardirq.h>
#include <linux/jump_label.h>

struct mem_cgroup;

struct page;
struct mm_struct;
struct kmem_cache;

struct mem_cgroup;

enum {
	UNDER_LIMIT,
	SOFT_LIMIT,
	OVER_LIMIT,
};

struct sock;

#endif /* ! __LINUX_MEMCONTROL_H__ */
