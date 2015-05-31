#ifndef __LINUX_SEMAPHORE_H__
#define __LINUX_SEMAPHORE_H__

#include <linux/list.h>
#include <linux/spinlock.h>

#define __SEMAPHORE_INITIALIZER(name, n)				\
{									\
	.lock		= __RAW_SPIN_LOCK_UNLOCKED((name).lock),	\
	.count		= n,						\
	.wait_list	= LIST_HEAD_INIT((name).wait_list),		\
}

#endif /* ! __LINUX_SEMAPHORE_H__ */
