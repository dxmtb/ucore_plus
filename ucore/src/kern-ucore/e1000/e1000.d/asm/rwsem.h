#ifndef __ASM_RWSEM_H__
#define __ASM_RWSEM_H__

#include <asm/asm.h>

# define RWSEM_ACTIVE_MASK		0xffffffffL

#define RWSEM_ACTIVE_BIAS		0x00000001L
#define RWSEM_WAITING_BIAS		(-RWSEM_ACTIVE_MASK-1)
#define RWSEM_ACTIVE_READ_BIAS		RWSEM_ACTIVE_BIAS
#define RWSEM_ACTIVE_WRITE_BIAS		(RWSEM_WAITING_BIAS + RWSEM_ACTIVE_BIAS)

/*
 * lock for writing
 */
static inline void __down_write_nested(struct rw_semaphore *sem, int subclass)
{
	long tmp;
	asm volatile("# beginning down_write\n\t"
		     LOCK_PREFIX "  xadd      %1,(%2)\n\t"
		     /* adds 0xffff0001, returns the old value */
		     "  test " __ASM_SEL(%w1,%k1) "," __ASM_SEL(%w1,%k1) "\n\t"
		     /* was the active mask 0 before? */
		     "  jz        1f\n"
		     "  call call_rwsem_down_write_failed\n"
		     "1:\n"
		     "# ending down_write"
		     : "+m" (sem->count), "=d" (tmp)
		     : "a" (sem), "1" (RWSEM_ACTIVE_WRITE_BIAS)
		     : "memory", "cc");
}

#endif /* ! __ASM_RWSEM_H__ */
