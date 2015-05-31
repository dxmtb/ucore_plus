#ifndef __ASM_PTRACE_H__
#define __ASM_PTRACE_H__

#include <asm/segment.h>
#include <asm/page_types.h>
#include <uapi/asm/ptrace.h>

struct pt_regs {
	unsigned long r15;
	unsigned long r14;
	unsigned long r13;
	unsigned long r12;
	unsigned long bp;
	unsigned long bx;
/* arguments: non interrupts/non tracing syscalls only save up to here*/
	unsigned long r11;
	unsigned long r10;
	unsigned long r9;
	unsigned long r8;
	unsigned long ax;
	unsigned long cx;
	unsigned long dx;
	unsigned long si;
	unsigned long di;
	unsigned long orig_ax;
/* end of arguments */
/* cpu exception frame or undefined */
	unsigned long ip;
	unsigned long cs;
	unsigned long flags;
	unsigned long sp;
	unsigned long ss;
/* top of stack page */
};

#include <linux/init.h>

struct cpuinfo_x86;
struct task_struct;

#define profile_pc profile_pc

/*
 * user_mode_vm(regs) determines whether a register set came from user mode.
 * This is true if V8086 mode was enabled OR if the register set was from
 * protected mode with RPL-3 CS value.  This tricky test checks that with
 * one comparison.  Many places in the kernel can bypass this full check
 * if they have already ruled out V8086 mode, so user_mode(regs) can be used.
 */
static inline int user_mode(struct pt_regs *regs)
{
#ifdef CONFIG_X86_32
	return (regs->cs & SEGMENT_RPL_MASK) == USER_RPL;
#else
	return !!(regs->cs & 3);
#endif
}

#define compat_user_stack_pointer()	\
	(test_thread_flag(TIF_IA32) 	\
	 ? current_pt_regs()->sp 	\
	 : this_cpu_read(old_rsp))

static inline unsigned long kernel_stack_pointer(struct pt_regs *regs)
{
	return regs->sp;
}

#define GET_IP(regs) ((regs)->ip)
#define GET_FP(regs) ((regs)->bp)
#define GET_USP(regs) ((regs)->sp)

#include <asm-generic/ptrace.h>

#define MAX_REG_OFFSET (offsetof(struct pt_regs, ss))

/**
 * regs_within_kernel_stack() - check the address in the stack
 * @regs:	pt_regs which contains kernel stack pointer.
 * @addr:	address which is checked.
 *
 * regs_within_kernel_stack() checks @addr is within the kernel stack page(s).
 * If @addr is within the kernel stack, it returns true. If not, returns false.
 */
static inline int regs_within_kernel_stack(struct pt_regs *regs,
					   unsigned long addr)
{
	return ((addr & ~(THREAD_SIZE - 1))  ==
		(kernel_stack_pointer(regs) & ~(THREAD_SIZE - 1)));
}

#endif /* ! __ASM_PTRACE_H__ */
