#ifndef __UAPI_LINUX_FIEMAP_H__
#define __UAPI_LINUX_FIEMAP_H__

#include <linux/types.h>

struct fiemap_extent {
	__u64 fe_logical;  /* logical offset in bytes for the start of
			    * the extent from the beginning of the file */
	__u64 fe_physical; /* physical offset in bytes for the start
			    * of the extent from the beginning of the disk */
	__u64 fe_length;   /* length in bytes for this extent */
	__u64 fe_reserved64[2];
	__u32 fe_flags;    /* FIEMAP_EXTENT_* flags for this extent */
	__u32 fe_reserved[3];
};

#endif /* ! __UAPI_LINUX_FIEMAP_H__ */
