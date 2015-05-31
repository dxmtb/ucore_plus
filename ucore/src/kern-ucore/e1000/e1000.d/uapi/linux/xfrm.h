#ifndef __UAPI_LINUX_XFRM_H__
#define __UAPI_LINUX_XFRM_H__

#include <linux/types.h>

enum {
	XFRM_POLICY_IN	= 0,
	XFRM_POLICY_OUT	= 1,
	XFRM_POLICY_FWD	= 2,
	XFRM_POLICY_MASK = 3,
	XFRM_POLICY_MAX	= 3
};

#endif /* ! __UAPI_LINUX_XFRM_H__ */
