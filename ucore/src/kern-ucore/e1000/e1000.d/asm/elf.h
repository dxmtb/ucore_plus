#ifndef __ASM_ELF_H__
#define __ASM_ELF_H__

/*
 * ELF register definitions..
 */
#include <linux/thread_info.h>

#include <asm/ptrace.h>
#include <asm/user.h>
#include <asm/auxvec.h>

#define ELF_NGREG (sizeof(struct user_regs_struct) / sizeof(elf_greg_t))

/*
 * These are used to set parameters in the core dumps.
 */
#define ELF_CLASS	ELFCLASS64

#include <asm/vdso.h>

#include <asm/processor.h>

#define SET_PERSONALITY(ex) set_personality_64bit()

/*
 * An executable for which elf_read_implies_exec() returns TRUE will
 * have the READ_IMPLIES_EXEC personality flag set automatically.
 */
#define elf_read_implies_exec(ex, executable_stack)	\
	(executable_stack != EXSTACK_DISABLE_X)

struct task_struct;

#endif /* ! __ASM_ELF_H__ */
