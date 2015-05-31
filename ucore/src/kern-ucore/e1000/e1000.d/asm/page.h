#ifndef __ASM_PAGE_H__
#define __ASM_PAGE_H__

#include <linux/types.h>

#include <asm/page_types.h>

#include <asm/page_64.h>

struct page;

#include <linux/range.h>

static inline void clear_user_page(void *page, unsigned long vaddr,
				   struct page *pg)
{
	clear_page(page);
}

static inline void copy_user_page(void *to, void *from, unsigned long vaddr,
				  struct page *topage)
{
	copy_page(to, from);
}

#define __alloc_zeroed_user_highpage(movableflags, vma, vaddr) \
	alloc_page_vma(GFP_HIGHUSER | __GFP_ZERO | movableflags, vma, vaddr)
#define __HAVE_ARCH_ALLOC_ZEROED_USER_HIGHPAGE

#define __pa(x)		__phys_addr((unsigned long)(x))

#define __va(x)			((void *)((unsigned long)(x)+PAGE_OFFSET))

/*
 * virt_to_page(kaddr) returns a valid pointer if and only if
 * virt_addr_valid(kaddr) returns true.
 */
#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)

#include <asm-generic/memory_model.h>
#include <asm-generic/getorder.h>

#define __HAVE_ARCH_GATE_AREA 1

#endif /* ! __ASM_PAGE_H__ */
