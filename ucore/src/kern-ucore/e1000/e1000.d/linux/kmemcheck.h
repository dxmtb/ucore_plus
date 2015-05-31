#ifndef __LINUX_KMEMCHECK_H__
#define __LINUX_KMEMCHECK_H__

#include <linux/mm_types.h>
#include <linux/types.h>

static inline void kmemcheck_mark_initialized(void *address, unsigned int n)
{
}

#define kmemcheck_bitfield_begin(name)
#define kmemcheck_bitfield_end(name)
#define kmemcheck_annotate_bitfield(ptr, name)	\
	do {					\
	} while (0)

#endif /* ! __LINUX_KMEMCHECK_H__ */
