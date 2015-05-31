#ifndef __ASM_GENERIC_DIV64_H__
#define __ASM_GENERIC_DIV64_H__

#include <linux/types.h>
#include <linux/compiler.h>

# define do_div(n,base) ({					\
	uint32_t __base = (base);				\
	uint32_t __rem;						\
	__rem = ((uint64_t)(n)) % __base;			\
	(n) = ((uint64_t)(n)) / __base;				\
	__rem;							\
 })

#endif /* ! __ASM_GENERIC_DIV64_H__ */
