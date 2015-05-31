#ifndef __LINUX_SRCU_H__
#define __LINUX_SRCU_H__

#include <linux/mutex.h>
#include <linux/rcupdate.h>
#include <linux/workqueue.h>

struct srcu_struct_array {
	unsigned long c[2];
	unsigned long seq[2];
};

struct rcu_batch {
	struct rcu_head *head, **tail;
};

struct srcu_struct {
	unsigned completed;
	struct srcu_struct_array __percpu *per_cpu_ref;
	spinlock_t queue_lock; /* protect ->batch_queue, ->running */
	bool running;
	/* callbacks just queued */
	struct rcu_batch batch_queue;
	/* callbacks try to do the first check_zero */
	struct rcu_batch batch_check0;
	/* callbacks done with the first check_zero and the flip */
	struct rcu_batch batch_check1;
	struct rcu_batch batch_done;
	struct delayed_work work;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map dep_map;
#endif /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */
};

int __srcu_read_lock(struct srcu_struct *sp) __acquires(sp);
void __srcu_read_unlock(struct srcu_struct *sp, int idx) __releases(sp);

#endif /* ! __LINUX_SRCU_H__ */
