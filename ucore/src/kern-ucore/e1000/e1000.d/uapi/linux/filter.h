#ifndef __UAPI_LINUX_FILTER_H__
#define __UAPI_LINUX_FILTER_H__

#include <linux/compiler.h>
#include <linux/types.h>

struct sock_filter {	/* Filter block */
	__u16	code;   /* Actual filter code */
	__u8	jt;	/* Jump true */
	__u8	jf;	/* Jump false */
	__u32	k;      /* Generic multiuse field */
};

#endif /* ! __UAPI_LINUX_FILTER_H__ */
