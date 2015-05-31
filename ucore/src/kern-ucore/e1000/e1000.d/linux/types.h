#ifndef __LINUX_TYPES_H__
#define __LINUX_TYPES_H__

#include <uapi/linux/types.h>

#define DECLARE_BITMAP(name,bits) \
	unsigned long name[BITS_TO_LONGS(bits)]

typedef __u32 __kernel_dev_t;

typedef __kernel_dev_t		dev_t;

typedef unsigned short		umode_t;

typedef __kernel_pid_t		pid_t;

typedef __kernel_clockid_t	clockid_t;

typedef _Bool			bool;

typedef __kernel_uid32_t	uid_t;
typedef __kernel_gid32_t	gid_t;

typedef __kernel_loff_t		loff_t;

typedef __kernel_size_t		size_t;

typedef __kernel_ssize_t	ssize_t;

typedef __kernel_time_t		time_t;

typedef __kernel_clock_t	clock_t;

typedef unsigned long		ulong;

typedef		__u8		u_int8_t;

typedef		__s32		int32_t;

typedef		__u8		uint8_t;

typedef		__u32		uint32_t;

typedef unsigned long sector_t;
typedef unsigned long blkcnt_t;

/*
 * The type of an index into the pagecache.  Use a #define so asm/types.h
 * can override it.
 */
#ifndef pgoff_t
#define pgoff_t unsigned long
#endif

typedef u64 dma_addr_t;

typedef unsigned __bitwise__ gfp_t;
typedef unsigned __bitwise__ fmode_t;
typedef unsigned __bitwise__ oom_flags_t;

typedef u64 phys_addr_t;

typedef phys_addr_t resource_size_t;

typedef struct {
	int counter;
} atomic_t;

typedef struct {
	long counter;
} atomic64_t;

struct list_head {
	struct list_head *next, *prev;
};

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
	struct hlist_node *next, **pprev;
};

/**
 * struct callback_head - callback structure for use with RCU and task_work
 * @next: next update requests in a list
 * @func: actual update function to call after the grace period.
 */
struct callback_head {
	struct callback_head *next;
	void (*func)(struct callback_head *head);
};
#define rcu_head callback_head

#endif /* ! __LINUX_TYPES_H__ */
