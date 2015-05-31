#ifndef __UAPI_LINUX_AIO_ABI_H__
#define __UAPI_LINUX_AIO_ABI_H__

#include <linux/types.h>
#include <asm/byteorder.h>

struct io_event {
	__u64		data;		/* the data field from the iocb */
	__u64		obj;		/* what iocb this event came from */
	__s64		res;		/* result code for this event */
	__s64		res2;		/* secondary result */
};

#define PADDED(x,y)	x, y

#endif /* ! __UAPI_LINUX_AIO_ABI_H__ */
