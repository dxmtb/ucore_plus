#ifndef __ASM_CPUFEATURE_H__
#define __ASM_CPUFEATURE_H__

#include <asm/required-features.h>

#define NCAPINTS	10	/* N 32-bit words worth of info */
#define NBUGINTS	1	/* N 32-bit bug flags */

#define X86_FEATURE_FPU		(0*32+ 0) /* Onboard FPU */

#define X86_FEATURE_PSE		(0*32+ 3) /* Page Size Extensions */

#define X86_FEATURE_MSR		(0*32+ 5) /* Model-Specific Registers */
#define X86_FEATURE_PAE		(0*32+ 6) /* Physical Address Extensions */

#define X86_FEATURE_CX8		(0*32+ 8) /* CMPXCHG8 instruction */

#define X86_FEATURE_PGE		(0*32+13) /* Page Global Enable */

#define X86_FEATURE_CMOV	(0*32+15) /* CMOV instructions */

#define X86_FEATURE_ACPI	(0*32+22) /* ACPI via MSR */

#define X86_FEATURE_FXSR	(0*32+24) /* FXSAVE/FXRSTOR, CR4.OSFXSR */
#define X86_FEATURE_XMM		(0*32+25) /* "sse" */
#define X86_FEATURE_XMM2	(0*32+26) /* "sse2" */

#define X86_FEATURE_HT		(0*32+28) /* Hyper-Threading */

#define X86_FEATURE_LM		(1*32+29) /* Long Mode (x86-64) */

#define X86_FEATURE_3DNOW	(1*32+31) /* 3DNow! */

#define X86_FEATURE_REP_GOOD	(3*32+16) /* rep microcode works well */
#define X86_FEATURE_MFENCE_RDTSC (3*32+17) /* "" Mfence synchronizes RDTSC */
#define X86_FEATURE_LFENCE_RDTSC (3*32+18) /* "" Lfence synchronizes RDTSC */
#define X86_FEATURE_11AP	(3*32+19) /* "" Bad local APIC aka 11AP */
#define X86_FEATURE_NOPL	(3*32+20) /* The NOPL (0F 1F) instructions */

#define X86_FEATURE_EXTD_APICID	(3*32+26) /* has extended APICID (8 bits) */

#define X86_FEATURE_APERFMPERF	(3*32+28) /* APERFMPERF */

#define X86_FEATURE_MWAIT	(4*32+ 3) /* "monitor" Monitor/Mwait support */

#define X86_FEATURE_EST		(4*32+ 7) /* Enhanced SpeedStep */

#define X86_FEATURE_POPCNT      (4*32+23) /* POPCNT instruction */

#define X86_FEATURE_RDRAND	(4*32+30) /* The RDRAND instruction */

#define X86_FEATURE_ERMS	(9*32+ 9) /* Enhanced REP MOVSB/STOSB */

#define X86_FEATURE_SMAP	(9*32+20) /* Supervisor Mode Access Prevention */

#include <asm/asm.h>
#include <linux/bitops.h>

#define test_cpu_cap(c, bit)						\
	 test_bit(bit, (unsigned long *)((c)->x86_capability))

#define REQUIRED_MASK_BIT_SET(bit)					\
	 ( (((bit)>>5)==0 && (1UL<<((bit)&31) & REQUIRED_MASK0)) ||	\
	   (((bit)>>5)==1 && (1UL<<((bit)&31) & REQUIRED_MASK1)) ||	\
	   (((bit)>>5)==2 && (1UL<<((bit)&31) & REQUIRED_MASK2)) ||	\
	   (((bit)>>5)==3 && (1UL<<((bit)&31) & REQUIRED_MASK3)) ||	\
	   (((bit)>>5)==4 && (1UL<<((bit)&31) & REQUIRED_MASK4)) ||	\
	   (((bit)>>5)==5 && (1UL<<((bit)&31) & REQUIRED_MASK5)) ||	\
	   (((bit)>>5)==6 && (1UL<<((bit)&31) & REQUIRED_MASK6)) ||	\
	   (((bit)>>5)==7 && (1UL<<((bit)&31) & REQUIRED_MASK7)) ||	\
	   (((bit)>>5)==8 && (1UL<<((bit)&31) & REQUIRED_MASK8)) ||	\
	   (((bit)>>5)==9 && (1UL<<((bit)&31) & REQUIRED_MASK9)) )

#define cpu_has(c, bit)							\
	(__builtin_constant_p(bit) && REQUIRED_MASK_BIT_SET(bit) ? 1 :	\
	 test_cpu_cap(c, bit))

#define boot_cpu_has(bit)	cpu_has(&boot_cpu_data, bit)

#define cpu_has_ht		boot_cpu_has(X86_FEATURE_HT)

#undef  cpu_has_vme

#undef  cpu_has_pae

#undef  cpu_has_mp

#undef  cpu_has_k6_mtrr

#undef  cpu_has_cyrix_arr

#undef  cpu_has_centaur_mcr

#endif /* ! __ASM_CPUFEATURE_H__ */
