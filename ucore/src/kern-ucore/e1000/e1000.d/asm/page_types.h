#ifndef __ASM_PAGE_TYPES_H__
#define __ASM_PAGE_TYPES_H__

#include <linux/const.h>
#include <linux/types.h>

#define PAGE_SHIFT	12
#define PAGE_SIZE	(_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))

#define __PHYSICAL_MASK		((phys_addr_t)((1ULL << __PHYSICAL_MASK_SHIFT) - 1))

/* Cast PAGE_MASK to a signed type so that it is sign-extended if
   virtual addresses are 32-bits but physical addresses are larger
   (ie, 32-bit PAE). */
#define PHYSICAL_PAGE_MASK	(((signed long)PAGE_MASK) & __PHYSICAL_MASK)

#define PAGE_OFFSET		((unsigned long)__PAGE_OFFSET)

#include <asm/page_64_types.h>

extern unsigned long max_pfn_mapped;

#endif /* ! __ASM_PAGE_TYPES_H__ */
