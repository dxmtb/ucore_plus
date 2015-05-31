#ifndef __LINUX_COMPAT_H__
#define __LINUX_COMPAT_H__

#include <linux/stat.h>
#include <linux/param.h>	/* for HZ */
#include <linux/sem.h>
#include <linux/socket.h>
#include <linux/if.h>
#include <linux/fs.h>
#include <linux/aio_abi.h>	/* for aio_context_t */

#include <asm/compat.h>
#include <asm/siginfo.h>
#include <asm/signal.h>

#define _COMPAT_NSIG_WORDS	(_COMPAT_NSIG / _COMPAT_NSIG_BPW)

#define COMPAT_SIGEV_PAD_SIZE	((SIGEV_MAX_SIZE/sizeof(int)) - 3)

struct compat_robust_list {
	compat_uptr_t			next;
};

struct compat_robust_list_head {
	struct compat_robust_list	list;
	compat_long_t			futex_offset;
	compat_uptr_t			list_op_pending;
};

#endif /* ! __LINUX_COMPAT_H__ */
