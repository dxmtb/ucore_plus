#ifndef __LINUX_RES_COUNTER_H__
#define __LINUX_RES_COUNTER_H__

#include <linux/spinlock.h>
#include <linux/errno.h>

struct res_counter {
	/*
	 * the current resource consumption level
	 */
	unsigned long long usage;
	/*
	 * the maximal value of the usage from the counter creation
	 */
	unsigned long long max_usage;
	/*
	 * the limit that usage cannot exceed
	 */
	unsigned long long limit;
	/*
	 * the limit that usage can be exceed
	 */
	unsigned long long soft_limit;
	/*
	 * the number of unsuccessful attempts to consume the resource
	 */
	unsigned long long failcnt;
	/*
	 * the lock to protect all of the above.
	 * the routines below consider this to be IRQ-safe
	 */
	spinlock_t lock;
	/*
	 * Parent counter, used for hierarchial resource accounting
	 */
	struct res_counter *parent;
};

u64 res_counter_read_u64(struct res_counter *counter, int member);

enum {
	RES_USAGE,
	RES_MAX_USAGE,
	RES_LIMIT,
	RES_FAILCNT,
	RES_SOFT_LIMIT,
};

int res_counter_charge_nofail(struct res_counter *counter,
		unsigned long val, struct res_counter **limit_fail_at);

u64 res_counter_uncharge(struct res_counter *counter, unsigned long val);

#endif /* ! __LINUX_RES_COUNTER_H__ */
