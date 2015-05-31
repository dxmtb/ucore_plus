#ifndef __ASM_VM86_H__
#define __ASM_VM86_H__

#include <asm/ptrace.h>
#include <uapi/asm/vm86.h>

struct kernel_vm86_regs {
/*
 * normal regs, with special meaning for the segment descriptors..
 */
	struct pt_regs pt;
/*
 * these are specific to v86 mode:
 */
	unsigned short es, __esh;
	unsigned short ds, __dsh;
	unsigned short fs, __fsh;
	unsigned short gs, __gsh;
};

#endif /* ! __ASM_VM86_H__ */
