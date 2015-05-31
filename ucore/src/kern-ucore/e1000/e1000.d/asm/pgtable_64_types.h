#ifndef __ASM_PGTABLE_64_TYPES_H__
#define __ASM_PGTABLE_64_TYPES_H__

#include <asm/sparsemem.h>

#include <linux/types.h>

/*
 * These are used to make use of C type-checking..
 */
typedef unsigned long	pteval_t;
typedef unsigned long	pmdval_t;
typedef unsigned long	pudval_t;
typedef unsigned long	pgdval_t;
typedef unsigned long	pgprotval_t;

typedef struct { pteval_t pte; } pte_t;

#define PAGETABLE_LEVELS	4

/*
 * 3rd level page
 */
#define PUD_SHIFT	30
#define PTRS_PER_PUD	512

/*
 * PMD_SHIFT determines the size of the area a middle-level
 * page table can map
 */
#define PMD_SHIFT	21
#define PTRS_PER_PMD	512

/*
 * entries per page directory level
 */
#define PTRS_PER_PTE	512

#define VMALLOC_START    _AC(0xffffc90000000000, UL)
#define VMALLOC_END      _AC(0xffffe8ffffffffff, UL)
#define VMEMMAP_START	 _AC(0xffffea0000000000, UL)

#endif /* ! __ASM_PGTABLE_64_TYPES_H__ */
