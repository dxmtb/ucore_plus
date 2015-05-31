#ifndef __LINUX_UNALIGNED_PACKED_STRUCT_H__
#define __LINUX_UNALIGNED_PACKED_STRUCT_H__

#include <linux/kernel.h>

struct __una_u32 { u32 x; } __packed;

static inline u32 __get_unaligned_cpu32(const void *p)
{
	const struct __una_u32 *ptr = (const struct __una_u32 *)p;
	return ptr->x;
}

#endif /* ! __LINUX_UNALIGNED_PACKED_STRUCT_H__ */
