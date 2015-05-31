#ifndef __LINUX_LIST_BL_H__
#define __LINUX_LIST_BL_H__

#include <linux/list.h>
#include <linux/bit_spinlock.h>

#define LIST_BL_LOCKMASK	1UL

#define LIST_BL_BUG_ON(x)

struct hlist_bl_head {
	struct hlist_bl_node *first;
};

struct hlist_bl_node {
	struct hlist_bl_node *next, **pprev;
};

static inline void INIT_HLIST_BL_NODE(struct hlist_bl_node *h)
{
	h->next = NULL;
	h->pprev = NULL;
}

static inline int hlist_bl_unhashed(const struct hlist_bl_node *h)
{
	return !h->pprev;
}

static inline struct hlist_bl_node *hlist_bl_first(struct hlist_bl_head *h)
{
	return (struct hlist_bl_node *)
		((unsigned long)h->first & ~LIST_BL_LOCKMASK);
}

static inline void hlist_bl_set_first(struct hlist_bl_head *h,
					struct hlist_bl_node *n)
{
	LIST_BL_BUG_ON((unsigned long)n & LIST_BL_LOCKMASK);
	LIST_BL_BUG_ON(((unsigned long)h->first & LIST_BL_LOCKMASK) !=
							LIST_BL_LOCKMASK);
	h->first = (struct hlist_bl_node *)((unsigned long)n | LIST_BL_LOCKMASK);
}

static inline void __hlist_bl_del(struct hlist_bl_node *n)
{
	struct hlist_bl_node *next = n->next;
	struct hlist_bl_node **pprev = n->pprev;

	LIST_BL_BUG_ON((unsigned long)n & LIST_BL_LOCKMASK);

	/* pprev may be `first`, so be careful not to lose the lock bit */
	*pprev = (struct hlist_bl_node *)
			((unsigned long)next |
			 ((unsigned long)*pprev & LIST_BL_LOCKMASK));
	if (next)
		next->pprev = pprev;
}

#endif /* ! __LINUX_LIST_BL_H__ */
