#ifndef __LINUX_LLIST_H__
#define __LINUX_LLIST_H__

#include <linux/kernel.h>
#include <asm/cmpxchg.h>

struct llist_node {
	struct llist_node *next;
};

#endif /* ! __LINUX_LLIST_H__ */
