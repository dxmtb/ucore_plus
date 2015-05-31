#ifndef __ASM_CACHE_H__
#define __ASM_CACHE_H__

#include <linux/linkage.h>

#define L1_CACHE_SHIFT	(CONFIG_X86_L1_CACHE_SHIFT)
#define L1_CACHE_BYTES	(1 << L1_CACHE_SHIFT)

#define __read_mostly __attribute__((__section__(".data..read_mostly")))

#define INTERNODE_CACHE_SHIFT CONFIG_X86_INTERNODE_CACHE_SHIFT

#endif /* ! __ASM_CACHE_H__ */
