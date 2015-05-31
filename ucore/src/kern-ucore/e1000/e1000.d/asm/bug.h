#ifndef __ASM_BUG_H__
#define __ASM_BUG_H__

#define HAVE_ARCH_BUG

# define __BUG_C0	"2:\t.long 1b - 2b, %c0 - 2b\n"

#define BUG()							\
do {								\
	asm volatile("1:\tud2\n"				\
		     ".pushsection __bug_table,\"a\"\n"		\
		     __BUG_C0					\
		     "\t.word %c1, 0\n"				\
		     "\t.org 2b+%c2\n"				\
		     ".popsection"				\
		     : : "i" (__FILE__), "i" (__LINE__),	\
		     "i" (sizeof(struct bug_entry)));		\
	unreachable();						\
} while (0)

#include <asm-generic/bug.h>

#endif /* ! __ASM_BUG_H__ */
