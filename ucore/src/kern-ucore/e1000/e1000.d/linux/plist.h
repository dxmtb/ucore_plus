#ifndef __LINUX_PLIST_H__
#define __LINUX_PLIST_H__

#include <linux/kernel.h>
#include <linux/list.h>

struct plist_head {
	struct list_head node_list;
};

struct plist_node {
	int			prio;
	struct list_head	prio_list;
	struct list_head	node_list;
};

#endif /* ! __LINUX_PLIST_H__ */
