#ifndef __UAPI_LINUX_KERNEL_H__
#define __UAPI_LINUX_KERNEL_H__

#include <linux/sysinfo.h>

/*
 * 'kernel.h' contains some often-used function prototypes etc
 */
#define __ALIGN_KERNEL(x, a)		__ALIGN_KERNEL_MASK(x, (typeof(x))(a) - 1)
#define __ALIGN_KERNEL_MASK(x, mask)	(((x) + (mask)) & ~(mask))

#endif /* ! __UAPI_LINUX_KERNEL_H__ */
