#ifndef __LINUX_UACCESS_H__
#define __LINUX_UACCESS_H__

#include <linux/preempt.h>
#include <asm/uaccess.h>

/*
 * These routines enable/disable the pagefault handler in that
 * it will not take any locks and go straight to the fixup table.
 *
 * They have great resemblance to the preempt_disable/enable calls
 * and in fact they are identical; this is because currently there is
 * no other way to make the pagefault handlers do this. So we do
 * disable preemption but we don't necessarily care about that.
 */
static inline void pagefault_disable(void)
{
	inc_preempt_count();
	/*
	 * make sure to have issued the store before a pagefault
	 * can hit.
	 */
	barrier();
}

static inline void pagefault_enable(void)
{
	/*
	 * make sure to issue those last loads/stores before enabling
	 * the pagefault handler again.
	 */
	barrier();
	dec_preempt_count();
	/*
	 * make sure we do..
	 */
	barrier();
	preempt_check_resched();
}

#endif /* ! __LINUX_UACCESS_H__ */
