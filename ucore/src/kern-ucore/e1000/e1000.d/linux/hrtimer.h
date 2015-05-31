#ifndef __LINUX_HRTIMER_H__
#define __LINUX_HRTIMER_H__

#include <linux/rbtree.h>
#include <linux/ktime.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/percpu.h>
#include <linux/timer.h>
#include <linux/timerqueue.h>

struct hrtimer_clock_base;
struct hrtimer_cpu_base;

/*
 * Mode arguments of xxx_hrtimer functions:
 */
enum hrtimer_mode {
	HRTIMER_MODE_ABS = 0x0,		/* Time value is absolute */
	HRTIMER_MODE_REL = 0x1,		/* Time value is relative to now */
	HRTIMER_MODE_PINNED = 0x02,	/* Timer is bound to CPU */
	HRTIMER_MODE_ABS_PINNED = 0x02,
	HRTIMER_MODE_REL_PINNED = 0x03,
};

/*
 * Return values for the callback function
 */
enum hrtimer_restart {
	HRTIMER_NORESTART,	/* Timer is not restarted */
	HRTIMER_RESTART,	/* Timer must be restarted */
};

/*
 * Values to track state of the timer
 *
 * Possible states:
 *
 * 0x00		inactive
 * 0x01		enqueued into rbtree
 * 0x02		callback function running
 * 0x04		timer is migrated to another cpu
 *
 * Special cases:
 * 0x03		callback function running and enqueued
 *		(was requeued on another CPU)
 * 0x05		timer was migrated on CPU hotunplug
 *
 * The "callback function running and enqueued" status is only possible on
 * SMP. It happens for example when a posix timer expired and the callback
 * queued a signal. Between dropping the lock which protects the posix timer
 * and reacquiring the base lock of the hrtimer, another CPU can deliver the
 * signal and rearm the timer. We have to preserve the callback running state,
 * as otherwise the timer could be removed before the softirq code finishes the
 * the handling of the timer.
 *
 * The HRTIMER_STATE_ENQUEUED bit is always or'ed to the current state
 * to preserve the HRTIMER_STATE_CALLBACK in the above scenario. This
 * also affects HRTIMER_STATE_MIGRATE where the preservation is not
 * necessary. HRTIMER_STATE_MIGRATE is cleared after the timer is
 * enqueued on the new cpu.
 *
 * All state transitions are protected by cpu_base->lock.
 */
#define HRTIMER_STATE_INACTIVE	0x00
#define HRTIMER_STATE_ENQUEUED	0x01
#define HRTIMER_STATE_CALLBACK	0x02

/**
 * struct hrtimer - the basic hrtimer structure
 * @node:	timerqueue node, which also manages node.expires,
 *		the absolute expiry time in the hrtimers internal
 *		representation. The time is related to the clock on
 *		which the timer is based. Is setup by adding
 *		slack to the _softexpires value. For non range timers
 *		identical to _softexpires.
 * @_softexpires: the absolute earliest expiry time of the hrtimer.
 *		The time which was given as expiry time when the timer
 *		was armed.
 * @function:	timer expiry callback function
 * @base:	pointer to the timer base (per cpu and per clock)
 * @state:	state information (See bit values above)
 * @start_site:	timer statistics field to store the site where the timer
 *		was started
 * @start_comm: timer statistics field to store the name of the process which
 *		started the timer
 * @start_pid: timer statistics field to store the pid of the task which
 *		started the timer
 *
 * The hrtimer structure must be initialized by hrtimer_init()
 */
struct hrtimer {
	struct timerqueue_node		node;
	ktime_t				_softexpires;
	enum hrtimer_restart		(*function)(struct hrtimer *);
	struct hrtimer_clock_base	*base;
	unsigned long			state;
#ifdef CONFIG_TIMER_STATS
	int				start_pid;
	void				*start_site;
	char				start_comm[16];
#endif
};

/**
 * struct hrtimer_clock_base - the timer base for a specific clock
 * @cpu_base:		per cpu clock base
 * @index:		clock type index for per_cpu support when moving a
 *			timer to a base on another cpu.
 * @clockid:		clock id for per_cpu support
 * @active:		red black tree root node for the active timers
 * @resolution:		the resolution of the clock, in nanoseconds
 * @get_time:		function to retrieve the current time of the clock
 * @softirq_time:	the time when running the hrtimer queue in the softirq
 * @offset:		offset of this clock to the monotonic base
 */
struct hrtimer_clock_base {
	struct hrtimer_cpu_base	*cpu_base;
	int			index;
	clockid_t		clockid;
	struct timerqueue_head	active;
	ktime_t			resolution;
	ktime_t			(*get_time)(void);
	ktime_t			softirq_time;
	ktime_t			offset;
};

enum  hrtimer_base_type {
	HRTIMER_BASE_MONOTONIC,
	HRTIMER_BASE_REALTIME,
	HRTIMER_BASE_BOOTTIME,
	HRTIMER_BASE_TAI,
	HRTIMER_MAX_CLOCK_BASES,
};

/*
 * struct hrtimer_cpu_base - the per cpu clock bases
 * @lock:		lock protecting the base and associated clock bases
 *			and timers
 * @active_bases:	Bitfield to mark bases with active timers
 * @clock_was_set:	Indicates that clock was set from irq context.
 * @expires_next:	absolute time of the next event which was scheduled
 *			via clock_set_next_event()
 * @hres_active:	State of high resolution mode
 * @hang_detected:	The last hrtimer interrupt detected a hang
 * @nr_events:		Total number of hrtimer interrupt events
 * @nr_retries:		Total number of hrtimer interrupt retries
 * @nr_hangs:		Total number of hrtimer interrupt hangs
 * @max_hang_time:	Maximum time spent in hrtimer_interrupt
 * @clock_base:		array of clock bases for this cpu
 */
struct hrtimer_cpu_base {
	raw_spinlock_t			lock;
	unsigned int			active_bases;
	unsigned int			clock_was_set;
#ifdef CONFIG_HIGH_RES_TIMERS
	ktime_t				expires_next;
	int				hres_active;
	int				hang_detected;
	unsigned long			nr_events;
	unsigned long			nr_retries;
	unsigned long			nr_hangs;
	ktime_t				max_hang_time;
#endif
	struct hrtimer_clock_base	clock_base[HRTIMER_MAX_CLOCK_BASES];
};

static inline ktime_t hrtimer_get_expires(const struct hrtimer *timer)
{
	return timer->node.expires;
}

static inline ktime_t hrtimer_get_softexpires(const struct hrtimer *timer)
{
	return timer->_softexpires;
}

extern ktime_t ktime_get(void);
extern ktime_t ktime_get_real(void);

extern void hrtimer_init(struct hrtimer *timer, clockid_t which_clock,
			 enum hrtimer_mode mode);

extern int hrtimer_start(struct hrtimer *timer, ktime_t tim,
			 const enum hrtimer_mode mode);
extern int hrtimer_start_range_ns(struct hrtimer *timer, ktime_t tim,
			unsigned long range_ns, const enum hrtimer_mode mode);

extern int hrtimer_cancel(struct hrtimer *timer);

static inline int hrtimer_start_expires(struct hrtimer *timer,
						enum hrtimer_mode mode)
{
	unsigned long delta;
	ktime_t soft, hard;
	soft = hrtimer_get_softexpires(timer);
	hard = hrtimer_get_expires(timer);
	delta = ktime_to_ns(ktime_sub(hard, soft));
	return hrtimer_start_range_ns(timer, soft, delta, mode);
}

extern u64
hrtimer_forward(struct hrtimer *timer, ktime_t now, ktime_t interval);

#endif /* ! __LINUX_HRTIMER_H__ */
