#ifndef __LINUX_ERRNO_H__
#define __LINUX_ERRNO_H__

#include <uapi/linux/errno.h>

/*
 * These should never be seen by user programs.  To return one of ERESTART*
 * codes, signal_pending() MUST be set.  Note that ptrace can observe these
 * at syscall exit tracing, but they will never be left for the debugged user
 * process to see.
 */
#define ERESTARTSYS	512
#define ERESTARTNOINTR	513

#endif /* ! __LINUX_ERRNO_H__ */
