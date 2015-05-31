#ifndef __ASM_MSR_H__
#define __ASM_MSR_H__

#include <uapi/asm/msr.h>

#include <asm/asm.h>
#include <asm/errno.h>
#include <asm/cpumask.h>

static inline unsigned long long native_read_tscp(unsigned int *aux)
{
	unsigned long low, high;
	asm volatile(".byte 0x0f,0x01,0xf9"
		     : "=a" (low), "=d" (high), "=c" (*aux));
	return low | ((u64)high << 32);
}

#define DECLARE_ARGS(val, low, high)	unsigned low, high
#define EAX_EDX_VAL(val, low, high)	((low) | ((u64)(high) << 32))

#define EAX_EDX_RET(val, low, high)	"=a" (low), "=d" (high)

static inline unsigned long long native_read_msr(unsigned int msr)
{
	DECLARE_ARGS(val, low, high);

	asm volatile("rdmsr" : EAX_EDX_RET(val, low, high) : "c" (msr));
	return EAX_EDX_VAL(val, low, high);
}

static inline unsigned long long native_read_msr_safe(unsigned int msr,
						      int *err)
{
	DECLARE_ARGS(val, low, high);

	asm volatile("2: rdmsr ; xor %[err],%[err]\n"
		     "1:\n\t"
		     ".section .fixup,\"ax\"\n\t"
		     "3:  mov %[fault],%[err] ; jmp 1b\n\t"
		     ".previous\n\t"
		     _ASM_EXTABLE(2b, 3b)
		     : [err] "=r" (*err), EAX_EDX_RET(val, low, high)
		     : "c" (msr), [fault] "i" (-EIO));
	return EAX_EDX_VAL(val, low, high);
}

static inline void native_write_msr(unsigned int msr,
				    unsigned low, unsigned high)
{
	asm volatile("wrmsr" : : "c" (msr), "a"(low), "d" (high) : "memory");
}

notrace static inline int native_write_msr_safe(unsigned int msr,
					unsigned low, unsigned high)
{
	int err;
	asm volatile("2: wrmsr ; xor %[err],%[err]\n"
		     "1:\n\t"
		     ".section .fixup,\"ax\"\n\t"
		     "3:  mov %[fault],%[err] ; jmp 1b\n\t"
		     ".previous\n\t"
		     _ASM_EXTABLE(2b, 3b)
		     : [err] "=a" (err)
		     : "c" (msr), "0" (low), "d" (high),
		       [fault] "i" (-EIO)
		     : "memory");
	return err;
}

static __always_inline unsigned long long __native_read_tsc(void)
{
	DECLARE_ARGS(val, low, high);

	asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));

	return EAX_EDX_VAL(val, low, high);
}

#include <linux/errno.h>

#define rdmsrl(msr, val)			\
	((val) = native_read_msr((msr)))

#define wrmsrl(msr, val)						\
	native_write_msr((msr), (u32)((u64)(val)), (u32)((u64)(val) >> 32))

#define rdtscll(val)						\
	((val) = __native_read_tsc())

#endif /* ! __ASM_MSR_H__ */
