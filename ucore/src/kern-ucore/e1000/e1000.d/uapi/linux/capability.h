#ifndef __UAPI_LINUX_CAPABILITY_H__
#define __UAPI_LINUX_CAPABILITY_H__

#include <linux/types.h>

struct task_struct;

#define _LINUX_CAPABILITY_U32S_3     2

#define VFS_CAP_U32_2           2

#define VFS_CAP_U32             VFS_CAP_U32_2

#define CAP_CHOWN            0

#define CAP_DAC_OVERRIDE     1

#define CAP_DAC_READ_SEARCH  2

#define CAP_FOWNER           3

#define CAP_FSETID           4

#define CAP_LINUX_IMMUTABLE  9

#define CAP_SYS_RESOURCE     24

#define CAP_MKNOD            27

#define CAP_MAC_OVERRIDE     32

#define CAP_TO_INDEX(x)     ((x) >> 5)        /* 1 << 5 == bits in __u32 */
#define CAP_TO_MASK(x)      (1 << ((x) & 31)) /* mask for indexed __u32 */

#endif /* ! __UAPI_LINUX_CAPABILITY_H__ */
