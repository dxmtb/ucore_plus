#ifndef __NET_PKT_SCHED_H__
#define __NET_PKT_SCHED_H__

#include <linux/jiffies.h>
#include <linux/ktime.h>
#include <net/sch_generic.h>

struct qdisc_walker {
	int	stop;
	int	skip;
	int	count;
	int	(*fn)(struct Qdisc *, unsigned long cl, struct qdisc_walker *);
};

#define QDISC_ALIGNTO		64
#define QDISC_ALIGN(len)	(((len) + QDISC_ALIGNTO-1) & ~(QDISC_ALIGNTO-1))

#define PSCHED_SHIFT			6
#define PSCHED_TICKS2NS(x)		((s64)(x) << PSCHED_SHIFT)
#define PSCHED_NS2TICKS(x)		((x) >> PSCHED_SHIFT)

struct qdisc_watchdog {
	struct hrtimer	timer;
	struct Qdisc	*qdisc;
};

extern void qdisc_watchdog_schedule_ns(struct qdisc_watchdog *wd, u64 expires);

extern void __qdisc_run(struct Qdisc *q);

#endif /* ! __NET_PKT_SCHED_H__ */
