#ifndef __UAPI_LINUX_FS_H__
#define __UAPI_LINUX_FS_H__

#include <linux/limits.h>
#include <linux/ioctl.h>
#include <linux/types.h>

#undef NR_OPEN

#define MS_MANDLOCK	64	/* Allow mandatory locks on an FS */

#define MS_NOSEC	(1<<28)

#endif /* ! __UAPI_LINUX_FS_H__ */
