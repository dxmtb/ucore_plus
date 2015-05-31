#ifndef __UAPI_LINUX_TIME_H__
#define __UAPI_LINUX_TIME_H__

#include <linux/types.h>

struct timespec {
	__kernel_time_t	tv_sec;			/* seconds */
	long		tv_nsec;		/* nanoseconds */
};

struct timeval {
	__kernel_time_t		tv_sec;		/* seconds */
	__kernel_suseconds_t	tv_usec;	/* microseconds */
};

#endif /* ! __UAPI_LINUX_TIME_H__ */
