#ifndef __UAPI_LINUX_RESOURCE_H__
#define __UAPI_LINUX_RESOURCE_H__

#include <linux/time.h>
#include <linux/types.h>

struct rlimit {
	unsigned long	rlim_cur;
	unsigned long	rlim_max;
};

/*
 * Due to binary compatibility, the actual resource numbers
 * may be different for different linux versions..
 */
#include <asm/resource.h>

#endif /* ! __UAPI_LINUX_RESOURCE_H__ */
