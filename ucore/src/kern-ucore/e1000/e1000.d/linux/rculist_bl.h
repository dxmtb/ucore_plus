#ifndef __LINUX_RCULIST_BL_H__
#define __LINUX_RCULIST_BL_H__

/*
 * RCU-protected bl list version. See include/linux/list_bl.h.
 */
#include <linux/list_bl.h>
#include <linux/rcupdate.h>

static inline void hlist_bl_set_first_rcu(struct hlist_bl_head *h,
					struct hlist_bl_node *n)
{
	LIST_BL_BUG_ON((unsigned long)n & LIST_BL_LOCKMASK);
	LIST_BL_BUG_ON(((unsigned long)h->first & LIST_BL_LOCKMASK) !=
							LIST_BL_LOCKMASK);
	rcu_assign_pointer(h->first,
		(struct hlist_bl_node *)((unsigned long)n | LIST_BL_LOCKMASK));
}

#endif /* ! __LINUX_RCULIST_BL_H__ */
