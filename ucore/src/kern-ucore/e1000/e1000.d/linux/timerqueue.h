#ifndef __LINUX_TIMERQUEUE_H__
#define __LINUX_TIMERQUEUE_H__

#include <linux/rbtree.h>
#include <linux/ktime.h>

struct timerqueue_node {
	struct rb_node node;
	ktime_t expires;
};

struct timerqueue_head {
	struct rb_root head;
	struct timerqueue_node *next;
};

#endif /* ! __LINUX_TIMERQUEUE_H__ */
