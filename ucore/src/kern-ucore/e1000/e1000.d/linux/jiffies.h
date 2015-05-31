#ifndef __LINUX_JIFFIES_H__
#define __LINUX_JIFFIES_H__

#include <linux/math64.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/time.h>
#include <linux/timex.h>
#include <asm/param.h>			/* for HZ */

# define SHIFT_HZ	10

#define TICK_NSEC ((NSEC_PER_SEC+HZ/2)/HZ)

/* some arch's have a small-data section that can be accessed register-relative
 * but that can only take up to, say, 4-byte variables. jiffies being part of
 * an 8-byte variable may not be correctly accessed unless we force the issue
 */
#define __jiffy_data  __attribute__((section(".data")))

extern unsigned long volatile __jiffy_data jiffies;

/*
 *	These inlines deal with timer wrapping correctly. You are
 *	strongly encouraged to use them
 *	1. Because people otherwise forget
 *	2. Because if the timer wrap changes in future you won't have to
 *	   alter your driver code.
 *
 * time_after(a,b) returns true if the time a is after time b.
 *
 * Do this with "<0" and ">=0" to only test the sign of the result. A
 * good compiler would generate better code (and a really good compiler
 * wouldn't care). Gcc is currently neither.
 */
#define time_after(a,b)		\
	(typecheck(unsigned long, a) && \
	 typecheck(unsigned long, b) && \
	 ((long)(b) - (long)(a) < 0))
#define time_before(a,b)	time_after(b,a)

#define SEC_JIFFIE_SC (31 - SHIFT_HZ)

#undef SEC_JIFFIE_SC

/*
 * Convert various time units to each other:
 */
extern unsigned int jiffies_to_msecs(const unsigned long j);

extern unsigned long msecs_to_jiffies(const unsigned int m);

extern clock_t jiffies_to_clock_t(unsigned long x);

#endif /* ! __LINUX_JIFFIES_H__ */
