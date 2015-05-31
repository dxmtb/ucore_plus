#ifndef __LINUX_PM_WAKEUP_H__
#define __LINUX_PM_WAKEUP_H__

#include <linux/types.h>

/**
 * struct wakeup_source - Representation of wakeup sources
 *
 * @total_time: Total time this wakeup source has been active.
 * @max_time: Maximum time this wakeup source has been continuously active.
 * @last_time: Monotonic clock when the wakeup source's was touched last time.
 * @prevent_sleep_time: Total time this source has been preventing autosleep.
 * @event_count: Number of signaled wakeup events.
 * @active_count: Number of times the wakeup sorce was activated.
 * @relax_count: Number of times the wakeup sorce was deactivated.
 * @expire_count: Number of times the wakeup source's timeout has expired.
 * @wakeup_count: Number of times the wakeup source might abort suspend.
 * @active: Status of the wakeup source.
 * @has_timeout: The wakeup source has been activated with a timeout.
 */
struct wakeup_source {
	const char 		*name;
	struct list_head	entry;
	spinlock_t		lock;
	struct timer_list	timer;
	unsigned long		timer_expires;
	ktime_t total_time;
	ktime_t max_time;
	ktime_t last_time;
	ktime_t start_prevent_time;
	ktime_t prevent_sleep_time;
	unsigned long		event_count;
	unsigned long		active_count;
	unsigned long		relax_count;
	unsigned long		expire_count;
	unsigned long		wakeup_count;
	bool			active:1;
	bool			autosleep_enabled:1;
};

static inline bool device_can_wakeup(struct device *dev)
{
	return dev->power.can_wakeup;
}

extern void wakeup_source_prepare(struct wakeup_source *ws, const char *name);

extern void wakeup_source_drop(struct wakeup_source *ws);

extern void wakeup_source_add(struct wakeup_source *ws);
extern void wakeup_source_remove(struct wakeup_source *ws);

extern int device_set_wakeup_enable(struct device *dev, bool enable);

#endif /* ! __LINUX_PM_WAKEUP_H__ */
