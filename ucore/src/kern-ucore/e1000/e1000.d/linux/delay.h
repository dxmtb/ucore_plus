#ifndef __LINUX_DELAY_H__
#define __LINUX_DELAY_H__

#include <linux/kernel.h>

#include <asm/delay.h>

#ifndef MAX_UDELAY_MS
#define MAX_UDELAY_MS	5
#endif

#ifndef mdelay
#define mdelay(n) (\
	(__builtin_constant_p(n) && (n)<=MAX_UDELAY_MS) ? udelay((n)*1000) : \
	({unsigned long __ms=(n); while (__ms--) udelay(1000);}))
#endif

void msleep(unsigned int msecs);
unsigned long msleep_interruptible(unsigned int msecs);

#endif /* ! __LINUX_DELAY_H__ */
