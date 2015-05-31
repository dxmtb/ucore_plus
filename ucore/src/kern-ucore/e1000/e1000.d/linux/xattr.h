#ifndef __LINUX_XATTR_H__
#define __LINUX_XATTR_H__

#include <linux/slab.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <uapi/linux/xattr.h>

struct inode;
struct dentry;

struct xattr_handler {
	const char *prefix;
	int flags;	/* fs private flags passed back to the handlers */
	size_t (*list)(struct dentry *dentry, char *list, size_t list_size,
		       const char *name, size_t name_len, int handler_flags);
	int (*get)(struct dentry *dentry, const char *name, void *buffer,
		   size_t size, int handler_flags);
	int (*set)(struct dentry *dentry, const char *name, const void *buffer,
		   size_t size, int flags, int handler_flags);
};

struct simple_xattrs {
	struct list_head head;
	spinlock_t lock;
};

#endif /* ! __LINUX_XATTR_H__ */
