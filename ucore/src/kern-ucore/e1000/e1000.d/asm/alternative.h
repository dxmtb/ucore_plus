#ifndef __ASM_ALTERNATIVE_H__
#define __ASM_ALTERNATIVE_H__

#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/stringify.h>
#include <asm/asm.h>

#define LOCK_PREFIX_HERE \
		".pushsection .smp_locks,\"a\"\n"	\
		".balign 4\n"				\
		".long 671f - .\n" /* offset */		\
		".popsection\n"				\
		"671:"

#define LOCK_PREFIX LOCK_PREFIX_HERE "\n\tlock; "

struct module;

#define OLDINSTR(oldinstr)	"661:\n\t" oldinstr "\n662:\n"

#define b_replacement(number)	"663"#number
#define e_replacement(number)	"664"#number

#define alt_slen "662b-661b"
#define alt_rlen(number) e_replacement(number)"f-"b_replacement(number)"f"

#define ALTINSTR_ENTRY(feature, number)					      \
	" .long 661b - .\n"				/* label           */ \
	" .long " b_replacement(number)"f - .\n"	/* new instruction */ \
	" .word " __stringify(feature) "\n"		/* feature bit     */ \
	" .byte " alt_slen "\n"				/* source len      */ \
	" .byte " alt_rlen(number) "\n"			/* replacement len */

#define DISCARD_ENTRY(number)				/* rlen <= slen */    \
	" .byte 0xff + (" alt_rlen(number) ") - (" alt_slen ")\n"

#define ALTINSTR_REPLACEMENT(newinstr, feature, number)	/* replacement */     \
	b_replacement(number)":\n\t" newinstr "\n" e_replacement(number) ":\n\t"

#define ALTERNATIVE(oldinstr, newinstr, feature)			\
	OLDINSTR(oldinstr)						\
	".pushsection .altinstructions,\"a\"\n"				\
	ALTINSTR_ENTRY(feature, 1)					\
	".popsection\n"							\
	".pushsection .discard,\"aw\",@progbits\n"			\
	DISCARD_ENTRY(1)						\
	".popsection\n"							\
	".pushsection .altinstr_replacement, \"ax\"\n"			\
	ALTINSTR_REPLACEMENT(newinstr, feature, 1)			\
	".popsection"

#define ALTERNATIVE_2(oldinstr, newinstr1, feature1, newinstr2, feature2)\
	OLDINSTR(oldinstr)						\
	".pushsection .altinstructions,\"a\"\n"				\
	ALTINSTR_ENTRY(feature1, 1)					\
	ALTINSTR_ENTRY(feature2, 2)					\
	".popsection\n"							\
	".pushsection .discard,\"aw\",@progbits\n"			\
	DISCARD_ENTRY(1)						\
	DISCARD_ENTRY(2)						\
	".popsection\n"							\
	".pushsection .altinstr_replacement, \"ax\"\n"			\
	ALTINSTR_REPLACEMENT(newinstr1, feature1, 1)			\
	ALTINSTR_REPLACEMENT(newinstr2, feature2, 2)			\
	".popsection"

/*
 * This must be included *after* the definition of ALTERNATIVE due to
 * <asm/arch_hweight.h>
 */
#include <asm/cpufeature.h>

/*
 * Alternative instructions for different CPU types or capabilities.
 *
 * This allows to use optimized instructions even on generic binary
 * kernels.
 *
 * length of oldinstr must be longer or equal the length of newinstr
 * It can be padded with nops as needed.
 *
 * For non barrier like inlines please define new variants
 * without volatile and memory clobber.
 */
#define alternative(oldinstr, newinstr, feature)			\
	asm volatile (ALTERNATIVE(oldinstr, newinstr, feature) : : : "memory")

/*
 * Alternative inline assembly with input.
 *
 * Pecularities:
 * No memory clobber here.
 * Argument numbers start with 1.
 * Best is to use constraints that are fixed size (like (%1) ... "r")
 * If you use variable sized constraints like "m" or "g" in the
 * replacement make sure to pad to the worst case length.
 * Leaving an unused argument 0 to keep API compatibility.
 */
#define alternative_input(oldinstr, newinstr, feature, input...)	\
	asm volatile (ALTERNATIVE(oldinstr, newinstr, feature)		\
		: : "i" (0), ## input)

#define alternative_io(oldinstr, newinstr, feature, output, input...)	\
	asm volatile (ALTERNATIVE(oldinstr, newinstr, feature)		\
		: output : "i" (0), ## input)

/*
 * Like alternative_call, but there are two features and respective functions.
 * If CPU has feature2, function2 is used.
 * Otherwise, if CPU has feature1, function1 is used.
 * Otherwise, old function is used.
 */
#define alternative_call_2(oldfunc, newfunc1, feature1, newfunc2, feature2,   \
			   output, input...)				      \
	asm volatile (ALTERNATIVE_2("call %P[old]", "call %P[new1]", feature1,\
		"call %P[new2]", feature2)				      \
		: output : [old] "i" (oldfunc), [new1] "i" (newfunc1),	      \
		[new2] "i" (newfunc2), ## input)

/*
 * use this macro(s) if you need more than one output parameter
 * in alternative_io
 */
#define ASM_OUTPUT2(a...) a

#endif /* ! __ASM_ALTERNATIVE_H__ */
