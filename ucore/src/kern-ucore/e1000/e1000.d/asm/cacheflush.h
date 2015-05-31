#ifndef __ASM_CACHEFLUSH_H__
#define __ASM_CACHEFLUSH_H__

#include <asm-generic/cacheflush.h>
#include <asm/special_insns.h>

#define _PGMT_DEFAULT		0
#define _PGMT_WC		(1UL << PG_arch_1)
#define _PGMT_UC_MINUS		(1UL << PG_uncached)
#define _PGMT_WB		(1UL << PG_uncached | 1UL << PG_arch_1)
#define _PGMT_MASK		(1UL << PG_uncached | 1UL << PG_arch_1)
#define _PGMT_CLEAR_MASK	(~_PGMT_MASK)

#endif /* ! __ASM_CACHEFLUSH_H__ */
