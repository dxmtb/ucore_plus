#ifndef __LINUX_PROPORTIONS_H__
#define __LINUX_PROPORTIONS_H__

#include <linux/percpu_counter.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>

struct prop_global {
	/*
	 * The period over which we differentiate
	 *
	 *   period = 2^shift
	 */
	int shift;
	/*
	 * The total event counter aka 'time'.
	 *
	 * Treated as an unsigned long; the lower 'shift - 1' bits are the
	 * counter bits, the remaining upper bits the period counter.
	 */
	struct percpu_counter events;
};

/*
 * global proportion descriptor
 *
 * this is needed to consitently flip prop_global structures.
 */
struct prop_descriptor {
	int index;
	struct prop_global pg[2];
	struct mutex mutex;		/* serialize the prop_global switch */
};

struct prop_local_percpu {
	/*
	 * the local events counter
	 */
	struct percpu_counter events;

	/*
	 * snapshot of the last seen global state
	 */
	int shift;
	unsigned long period;
	raw_spinlock_t lock;		/* protect the snapshot state */
};

void __prop_inc_percpu(struct prop_descriptor *pd, struct prop_local_percpu *pl);

struct prop_local_single {
	/*
	 * the local events counter
	 */
	unsigned long events;

	/*
	 * snapshot of the last seen global state
	 * and a lock protecting this state
	 */
	unsigned long period;
	int shift;
	raw_spinlock_t lock;		/* protect the snapshot state */
};

void __prop_inc_single(struct prop_descriptor *pd, struct prop_local_single *pl);

#endif /* ! __LINUX_PROPORTIONS_H__ */
