#ifndef __ASM_PAGE_64_TYPES_H__
#define __ASM_PAGE_64_TYPES_H__

#define THREAD_SIZE_ORDER	1
#define THREAD_SIZE  (PAGE_SIZE << THREAD_SIZE_ORDER)

#define IRQ_STACK_ORDER 2
#define IRQ_STACK_SIZE (PAGE_SIZE << IRQ_STACK_ORDER)

/*
 * Set __PAGE_OFFSET to the most negative possible address +
 * PGDIR_SIZE*16 (pgd slot 272).  The gap is to allow a space for a
 * hypervisor to fit.  Choosing 16 slots here is arbitrary, but it's
 * what Xen requires.
 */
#define __PAGE_OFFSET           _AC(0xffff880000000000, UL)

#define __START_KERNEL_map	_AC(0xffffffff80000000, UL)

#define __PHYSICAL_MASK_SHIFT	46

#endif /* ! __ASM_PAGE_64_TYPES_H__ */
