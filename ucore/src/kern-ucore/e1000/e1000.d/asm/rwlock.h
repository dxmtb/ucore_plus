#ifndef __ASM_RWLOCK_H__
#define __ASM_RWLOCK_H__

#include <asm/asm.h>

typedef union {
	s32 lock;
	s32 write;
} arch_rwlock_t;

#define RW_LOCK_BIAS		0x00100000
#define READ_LOCK_SIZE(insn)	__ASM_FORM(insn##l)
#define READ_LOCK_ATOMIC(n)	atomic_##n
#define WRITE_LOCK_ADD(n)	__ASM_FORM_COMMA(addl n)
#define WRITE_LOCK_SUB(n)	__ASM_FORM_COMMA(subl n)
#define WRITE_LOCK_CMP		RW_LOCK_BIAS

#endif /* ! __ASM_RWLOCK_H__ */
