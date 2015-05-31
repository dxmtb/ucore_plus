#ifndef __ASM_PAGE_64_H__
#define __ASM_PAGE_64_H__

#include <asm/page_64_types.h>

extern unsigned long phys_base;

static inline unsigned long __phys_addr_nodebug(unsigned long x)
{
	unsigned long y = x - __START_KERNEL_map;

	/* use the carry flag to determine if x was < __START_KERNEL_map */
	x = y + ((x > y) ? phys_base : (__START_KERNEL_map - PAGE_OFFSET));

	return x;
}

#define __phys_addr(x)		__phys_addr_nodebug(x)

void clear_page(void *page);
void copy_page(void *to, void *from);

#endif /* ! __ASM_PAGE_64_H__ */
