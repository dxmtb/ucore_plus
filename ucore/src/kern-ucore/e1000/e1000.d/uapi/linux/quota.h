#ifndef __UAPI_LINUX_QUOTA_H__
#define __UAPI_LINUX_QUOTA_H__

#include <linux/errno.h>
#include <linux/types.h>

#define MAXQUOTAS 2

struct if_dqinfo {
	__u64 dqi_bgrace;
	__u64 dqi_igrace;
	__u32 dqi_flags;
	__u32 dqi_valid;
};

#endif /* ! __UAPI_LINUX_QUOTA_H__ */
