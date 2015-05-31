#ifndef __ASM_THREAD_INFO_H__
#define __ASM_THREAD_INFO_H__

#include <linux/compiler.h>
#include <asm/page.h>
#include <asm/types.h>

struct task_struct;
struct exec_domain;
#include <asm/processor.h>
#include <linux/atomic.h>

struct thread_info {
	struct task_struct	*task;		/* main task structure */
	struct exec_domain	*exec_domain;	/* execution domain */
	__u32			flags;		/* low level flags */
	__u32			status;		/* thread synchronous flags */
	__u32			cpu;		/* current CPU */
	int			preempt_count;	/* 0 => preemptable,
						   <0 => BUG */
	mm_segment_t		addr_limit;
	struct restart_block    restart_block;
	void __user		*sysenter_return;
#ifdef CONFIG_X86_32
	unsigned long           previous_esp;   /* ESP of the previous stack in
						   case of nested (IRQ) stacks
						*/
	__u8			supervisor_stack[0];
#endif
	unsigned int		sig_on_uaccess_error:1;
	unsigned int		uaccess_err:1;	/* uaccess failed */
};

#define TIF_SIGPENDING		2	/* signal pending */
#define TIF_NEED_RESCHED	3	/* rescheduling necessary */

#define TIF_SECCOMP		8	/* secure computing */

#define TIF_IA32		17	/* IA32 compatibility process */

#define TIF_ADDR32		29	/* 32-bit address space on 64 bits */

#define PREEMPT_ACTIVE		0x10000000

#include <asm/percpu.h>
#define KERNEL_STACK_OFFSET (5*8)

DECLARE_PER_CPU(unsigned long, kernel_stack);

static inline struct thread_info *current_thread_info(void)
{
	struct thread_info *ti;
	ti = (void *)(this_cpu_read_stable(kernel_stack) +
		      KERNEL_STACK_OFFSET - THREAD_SIZE);
	return ti;
}

/*
 * Thread-synchronous status.
 *
 * This is different from the flags in that nobody else
 * ever touches our thread-synchronous status, so we don't
 * have to worry about atomic accesses.
 */
#define TS_COMPAT		0x0002	/* 32bit syscall active (64BIT)*/
#define TS_POLLING		0x0004	/* idle task polling need_resched,
					   skip sending interrupt */
#define TS_RESTORE_SIGMASK	0x0008	/* restore signal mask in do_signal() */

#define HAVE_SET_RESTORE_SIGMASK	1

static inline bool test_restore_sigmask(void)
{
	return current_thread_info()->status & TS_RESTORE_SIGMASK;
}
static inline bool test_and_clear_restore_sigmask(void)
{
	struct thread_info *ti = current_thread_info();
	if (!(ti->status & TS_RESTORE_SIGMASK))
		return false;
	ti->status &= ~TS_RESTORE_SIGMASK;
	return true;
}

static inline bool is_ia32_task(void)
{
#ifdef CONFIG_X86_32
	return true;
#endif
#ifdef CONFIG_IA32_EMULATION
	if (current_thread_info()->status & TS_COMPAT)
		return true;
#endif
	return false;
}

#endif /* ! __ASM_THREAD_INFO_H__ */
