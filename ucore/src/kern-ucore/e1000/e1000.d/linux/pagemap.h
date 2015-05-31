#ifndef __LINUX_PAGEMAP_H__
#define __LINUX_PAGEMAP_H__

/*
 * Copyright 1995 Linus Torvalds
 */
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/highmem.h>
#include <linux/compiler.h>
#include <asm/uaccess.h>
#include <linux/gfp.h>
#include <linux/bitops.h>
#include <linux/hardirq.h> /* for in_interrupt() */
#include <linux/hugetlb_inline.h>

static inline gfp_t mapping_gfp_mask(struct address_space * mapping)
{
	return (__force gfp_t)mapping->flags & __GFP_BITS_MASK;
}

/*
 * The page cache can done in larger chunks than
 * one page, because it allows for more efficient
 * throughput (it can then be mapped into user
 * space in smaller chunks for same flexibility).
 *
 * Or rather, it _will_ be done in larger chunks.
 */
#define PAGE_CACHE_SHIFT	PAGE_SHIFT

extern struct page *__page_cache_alloc(gfp_t gfp);

typedef int filler_t(void *, struct page *);

extern struct page * find_or_create_page(struct address_space *mapping,
				pgoff_t index, gfp_t gfp_mask);

extern struct page * read_cache_page_async(struct address_space *mapping,
				pgoff_t index, filler_t *filler, void *data);
extern struct page * read_cache_page(struct address_space *mapping,
				pgoff_t index, filler_t *filler, void *data);

extern pgoff_t linear_hugepage_index(struct vm_area_struct *vma,
				     unsigned long address);

extern void __lock_page(struct page *page);
extern int __lock_page_killable(struct page *page);
extern int __lock_page_or_retry(struct page *page, struct mm_struct *mm,
				unsigned int flags);

static inline void __set_page_locked(struct page *page)
{
	__set_bit(PG_locked, &page->flags);
}

static inline void __clear_page_locked(struct page *page)
{
	__clear_bit(PG_locked, &page->flags);
}

static inline int trylock_page(struct page *page)
{
	return (likely(!test_and_set_bit_lock(PG_locked, &page->flags)));
}

/*
 * This is exported only for wait_on_page_locked/wait_on_page_writeback.
 * Never use this directly!
 */
extern void wait_on_page_bit(struct page *page, int bit_nr);

extern int wait_on_page_bit_killable(struct page *page, int bit_nr);

int add_to_page_cache_locked(struct page *page, struct address_space *mapping,
				pgoff_t index, gfp_t gfp_mask);

#endif /* ! __LINUX_PAGEMAP_H__ */
