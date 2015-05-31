#ifndef __LINUX_TIME_H__
#define __LINUX_TIME_H__

# include <linux/cache.h>
# include <linux/seqlock.h>
# include <linux/math64.h>
#include <uapi/linux/time.h>

#define MSEC_PER_SEC	1000L
#define USEC_PER_MSEC	1000L
#define NSEC_PER_USEC	1000L

#define USEC_PER_SEC	1000000L
#define NSEC_PER_SEC	1000000000L

extern void set_normalized_timespec(struct timespec *ts, time_t sec, s64 nsec);

#define KTIME_MAX			((s64)~((u64)1 << 63))

# define KTIME_SEC_MAX			(KTIME_MAX / NSEC_PER_SEC)

/*
 * Returns true if the timespec is norm, false if denorm:
 */
static inline bool timespec_valid(const struct timespec *ts)
{
	/* Dates before 1970 are bogus */
	if (ts->tv_sec < 0)
		return false;
	/* Can't have more nanoseconds then a second */
	if ((unsigned long)ts->tv_nsec >= NSEC_PER_SEC)
		return false;
	return true;
}

extern bool persistent_clock_exist;

/**
 * ns_to_timespec - Convert nanoseconds to timespec
 * @nsec:	the nanoseconds value to be converted
 *
 * Returns the timespec representation of the nsec parameter.
 */
extern struct timespec ns_to_timespec(const s64 nsec);

/**
 * ns_to_timeval - Convert nanoseconds to timeval
 * @nsec:	the nanoseconds value to be converted
 *
 * Returns the timeval representation of the nsec parameter.
 */
extern struct timeval ns_to_timeval(const s64 nsec);

#endif /* ! __LINUX_TIME_H__ */
