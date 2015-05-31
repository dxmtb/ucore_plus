#ifndef __LINUX_RWSEM_H__
#define __LINUX_RWSEM_H__

#define _LINUX_RWSEM_H

#include <linux/linkage.h>

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/spinlock.h>

#include <linux/atomic.h>

struct rw_semaphore;

struct rw_semaphore {
	long			count;
	raw_spinlock_t		wait_lock;
	struct list_head	wait_list;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
};

#include <asm/rwsem.h>

/*
 * lock for reading
 */
extern void down_read(struct rw_semaphore *sem);

/*
 * lock for writing
 */
extern void down_write(struct rw_semaphore *sem);

/*
 * release a read lock
 */
extern void up_read(struct rw_semaphore *sem);

/*
 * release a write lock
 */
extern void up_write(struct rw_semaphore *sem);

#endif /* ! __LINUX_RWSEM_H__ */
