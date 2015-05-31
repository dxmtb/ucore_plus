#ifndef __LINUX_KLIST_H__
#define __LINUX_KLIST_H__

#include <linux/spinlock.h>
#include <linux/kref.h>
#include <linux/list.h>

struct klist_node;

struct klist_node {
	void			*n_klist;	/* never access directly */
	struct list_head	n_node;
	struct kref		n_ref;
};

#endif /* ! __LINUX_KLIST_H__ */
