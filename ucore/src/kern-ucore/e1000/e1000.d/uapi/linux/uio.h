#ifndef __UAPI_LINUX_UIO_H__
#define __UAPI_LINUX_UIO_H__

#include <linux/compiler.h>
#include <linux/types.h>

struct iovec
{
	void __user *iov_base;	/* BSD uses caddr_t (1003.1g requires void *) */
	__kernel_size_t iov_len; /* Must be size_t (1003.1g) */
};

#endif /* ! __UAPI_LINUX_UIO_H__ */
