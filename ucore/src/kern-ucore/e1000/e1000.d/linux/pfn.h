#ifndef __LINUX_PFN_H__
#define __LINUX_PFN_H__

#include <linux/types.h>

#define PFN_PHYS(x)	((phys_addr_t)(x) << PAGE_SHIFT)

#endif /* ! __LINUX_PFN_H__ */
