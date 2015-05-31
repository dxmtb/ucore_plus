#ifndef __ASM_COMPAT_H__
#define __ASM_COMPAT_H__

/*
 * Architecture specific compatibility types
 */
#include <linux/types.h>
#include <linux/sched.h>
#include <asm/processor.h>
#include <asm/user32.h>
#include <asm/unistd.h>

typedef s32		compat_time_t;

typedef s32		compat_long_t;

typedef u32		compat_uptr_t;

struct compat_timespec {
	compat_time_t	tv_sec;
	s32		tv_nsec;
};

#define _COMPAT_NSIG		64
#define _COMPAT_NSIG_BPW	32

static inline bool is_x32_task(void)
{
#ifdef CONFIG_X86_X32_ABI
	if (task_pt_regs(current)->orig_ax & __X32_SYSCALL_BIT)
		return true;
#endif
	return false;
}

#endif /* ! __ASM_COMPAT_H__ */
