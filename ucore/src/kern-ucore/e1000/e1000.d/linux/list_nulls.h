#ifndef __LINUX_LIST_NULLS_H__
#define __LINUX_LIST_NULLS_H__

struct hlist_nulls_head {
	struct hlist_nulls_node *first;
};

struct hlist_nulls_node {
	struct hlist_nulls_node *next, **pprev;
};

#define hlist_nulls_entry(ptr, type, member) container_of(ptr,type,member)
/**
 * ptr_is_a_nulls - Test if a ptr is a nulls
 * @ptr: ptr to be tested
 *
 */
static inline int is_a_nulls(const struct hlist_nulls_node *ptr)
{
	return ((unsigned long)ptr & 1);
}

static inline int hlist_nulls_unhashed(const struct hlist_nulls_node *h)
{
	return !h->pprev;
}

static inline int hlist_nulls_empty(const struct hlist_nulls_head *h)
{
	return is_a_nulls(h->first);
}

static inline void __hlist_nulls_del(struct hlist_nulls_node *n)
{
	struct hlist_nulls_node *next = n->next;
	struct hlist_nulls_node **pprev = n->pprev;
	*pprev = next;
	if (!is_a_nulls(next))
		next->pprev = pprev;
}

#endif /* ! __LINUX_LIST_NULLS_H__ */
