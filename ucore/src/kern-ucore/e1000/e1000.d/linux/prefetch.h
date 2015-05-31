#ifndef __LINUX_PREFETCH_H__
#define __LINUX_PREFETCH_H__

#include <linux/types.h>
#include <asm/processor.h>
#include <asm/cache.h>

#ifndef ARCH_HAS_PREFETCH
#define prefetch(x) __builtin_prefetch(x)
#endif

#endif /* ! __LINUX_PREFETCH_H__ */
