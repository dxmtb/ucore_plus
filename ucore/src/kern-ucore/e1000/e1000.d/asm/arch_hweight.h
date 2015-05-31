#ifndef __ASM_ARCH_HWEIGHT_H__
#define __ASM_ARCH_HWEIGHT_H__

#define POPCNT32 ".byte 0xf3,0x40,0x0f,0xb8,0xc7"

#define POPCNT64 ".byte 0xf3,0x48,0x0f,0xb8,0xc7"
#define REG_IN "D"
#define REG_OUT "a"

/*
 * __sw_hweightXX are called from within the alternatives below
 * and callee-clobbered registers need to be taken care of. See
 * ARCH_HWEIGHT_CFLAGS in <arch/x86/Kconfig> for the respective
 * compiler switches.
 */
static inline unsigned int __arch_hweight32(unsigned int w)
{
	unsigned int res = 0;

	asm (ALTERNATIVE("call __sw_hweight32", POPCNT32, X86_FEATURE_POPCNT)
		     : "="REG_OUT (res)
		     : REG_IN (w));

	return res;
}

static inline unsigned long __arch_hweight64(__u64 w)
{
	unsigned long res = 0;

#ifdef CONFIG_X86_32
	return  __arch_hweight32((u32)w) +
		__arch_hweight32((u32)(w >> 32));
#else
	asm (ALTERNATIVE("call __sw_hweight64", POPCNT64, X86_FEATURE_POPCNT)
		     : "="REG_OUT (res)
		     : REG_IN (w));
#endif /* CONFIG_X86_32 */

	return res;
}

#endif /* ! __ASM_ARCH_HWEIGHT_H__ */
