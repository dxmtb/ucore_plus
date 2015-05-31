#ifndef __LINUX_SECCOMP_H__
#define __LINUX_SECCOMP_H__

#include <uapi/linux/seccomp.h>

#include <linux/thread_info.h>
#include <asm/seccomp.h>

struct seccomp_filter;
/**
 * struct seccomp - the state of a seccomp'ed process
 *
 * @mode:  indicates one of the valid values above for controlled
 *         system calls available to a process.
 * @filter: The metadata and ruleset for determining what system calls
 *          are allowed for a task.
 *
 *          @filter must only be accessed from the context of current as there
 *          is no locking.
 */
struct seccomp {
	int mode;
	struct seccomp_filter *filter;
};

extern int __secure_computing(int);
static inline int secure_computing(int this_syscall)
{
	if (unlikely(test_thread_flag(TIF_SECCOMP)))
		return  __secure_computing(this_syscall);
	return 0;
}

#endif /* ! __LINUX_SECCOMP_H__ */
