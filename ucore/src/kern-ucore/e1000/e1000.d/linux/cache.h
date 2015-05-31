#ifndef __LINUX_CACHE_H__
#define __LINUX_CACHE_H__

#include <linux/kernel.h>
#include <asm/cache.h>

#ifndef SMP_CACHE_BYTES
#define SMP_CACHE_BYTES L1_CACHE_BYTES
#endif

#ifndef ____cacheline_aligned
#define ____cacheline_aligned __attribute__((__aligned__(SMP_CACHE_BYTES)))
#endif

#define ____cacheline_aligned_in_smp ____cacheline_aligned

#define ____cacheline_internodealigned_in_smp \
	__attribute__((__aligned__(1 << (INTERNODE_CACHE_SHIFT))))

#endif /* ! __LINUX_CACHE_H__ */
