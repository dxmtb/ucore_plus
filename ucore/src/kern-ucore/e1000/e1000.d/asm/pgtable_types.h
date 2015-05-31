#ifndef __ASM_PGTABLE_TYPES_H__
#define __ASM_PGTABLE_TYPES_H__

#include <linux/const.h>
#include <asm/page_types.h>

#define _PAGE_BIT_PRESENT	0	/* is present */
#define _PAGE_BIT_RW		1	/* writeable */
#define _PAGE_BIT_USER		2	/* userspace addressable */
#define _PAGE_BIT_PWT		3	/* page write through */
#define _PAGE_BIT_PCD		4	/* page cache disabled */
#define _PAGE_BIT_ACCESSED	5	/* was accessed (raised by CPU) */
#define _PAGE_BIT_DIRTY		6	/* was written to (raised by CPU) */
#define _PAGE_BIT_PSE		7	/* 4 MB (or 2MB) page */

#define _PAGE_BIT_GLOBAL	8	/* Global TLB entry PPro+ */
#define _PAGE_BIT_UNUSED1	9	/* available for programmer */

#define _PAGE_BIT_SPECIAL	_PAGE_BIT_UNUSED1

#define _PAGE_BIT_NX           63       /* No execute: only valid after cpuid check */

#define _PAGE_BIT_PROTNONE	_PAGE_BIT_GLOBAL

#define _PAGE_BIT_FILE		_PAGE_BIT_DIRTY

#define _PAGE_PRESENT	(_AT(pteval_t, 1) << _PAGE_BIT_PRESENT)
#define _PAGE_RW	(_AT(pteval_t, 1) << _PAGE_BIT_RW)
#define _PAGE_USER	(_AT(pteval_t, 1) << _PAGE_BIT_USER)
#define _PAGE_PWT	(_AT(pteval_t, 1) << _PAGE_BIT_PWT)
#define _PAGE_PCD	(_AT(pteval_t, 1) << _PAGE_BIT_PCD)
#define _PAGE_ACCESSED	(_AT(pteval_t, 1) << _PAGE_BIT_ACCESSED)
#define _PAGE_DIRTY	(_AT(pteval_t, 1) << _PAGE_BIT_DIRTY)
#define _PAGE_PSE	(_AT(pteval_t, 1) << _PAGE_BIT_PSE)
#define _PAGE_GLOBAL	(_AT(pteval_t, 1) << _PAGE_BIT_GLOBAL)

#define _PAGE_SPECIAL	(_AT(pteval_t, 1) << _PAGE_BIT_SPECIAL)

#define _PAGE_HIDDEN	(_AT(pteval_t, 0))

#define _PAGE_NX	(_AT(pteval_t, 1) << _PAGE_BIT_NX)

#define _PAGE_FILE	(_AT(pteval_t, 1) << _PAGE_BIT_FILE)
#define _PAGE_PROTNONE	(_AT(pteval_t, 1) << _PAGE_BIT_PROTNONE)

/*
 * _PAGE_NUMA indicates that this page will trigger a numa hinting
 * minor page fault to gather numa placement statistics (see
 * pte_numa()). The bit picked (8) is within the range between
 * _PAGE_FILE (6) and _PAGE_PROTNONE (8) bits. Therefore, it doesn't
 * require changes to the swp entry format because that bit is always
 * zero when the pte is not present.
 *
 * The bit picked must be always zero when the pmd is present and not
 * present, so that we don't lose information when we set it while
 * atomically clearing the present bit.
 *
 * Because we shared the same bit (8) with _PAGE_PROTNONE this can be
 * interpreted as _PAGE_NUMA only in places that _PAGE_PROTNONE
 * couldn't reach, like handle_mm_fault() (see access_error in
 * arch/x86/mm/fault.c, the vma protection must not be PROT_NONE for
 * handle_mm_fault() to be invoked).
 */
#define _PAGE_NUMA	_PAGE_PROTNONE

#define _KERNPG_TABLE	(_PAGE_PRESENT | _PAGE_RW | _PAGE_ACCESSED |	\
			 _PAGE_DIRTY)

#define _PAGE_CHG_MASK	(PTE_PFN_MASK | _PAGE_PCD | _PAGE_PWT |		\
			 _PAGE_SPECIAL | _PAGE_ACCESSED | _PAGE_DIRTY)
#define _HPAGE_CHG_MASK (_PAGE_CHG_MASK | _PAGE_PSE)

#define _PAGE_CACHE_WB		(0)
#define _PAGE_CACHE_WC		(_PAGE_PWT)
#define _PAGE_CACHE_UC_MINUS	(_PAGE_PCD)

# include <asm/pgtable_64_types.h>

#include <linux/types.h>

#define PTE_PFN_MASK		((pteval_t)PHYSICAL_PAGE_MASK)

#define PTE_FLAGS_MASK		(~PTE_PFN_MASK)

typedef struct pgprot { pgprotval_t pgprot; } pgprot_t;

typedef struct { pgdval_t pgd; } pgd_t;

static inline pgd_t native_make_pgd(pgdval_t val)
{
	return (pgd_t) { val };
}

static inline pgdval_t native_pgd_val(pgd_t pgd)
{
	return pgd.pgd;
}

static inline pgdval_t pgd_flags(pgd_t pgd)
{
	return native_pgd_val(pgd) & PTE_FLAGS_MASK;
}

typedef struct { pudval_t pud; } pud_t;

static inline pud_t native_make_pud(pmdval_t val)
{
	return (pud_t) { val };
}

static inline pudval_t native_pud_val(pud_t pud)
{
	return pud.pud;
}

typedef struct { pmdval_t pmd; } pmd_t;

static inline pmd_t native_make_pmd(pmdval_t val)
{
	return (pmd_t) { val };
}

static inline pmdval_t native_pmd_val(pmd_t pmd)
{
	return pmd.pmd;
}

static inline pudval_t pud_flags(pud_t pud)
{
	return native_pud_val(pud) & PTE_FLAGS_MASK;
}

static inline pmdval_t pmd_flags(pmd_t pmd)
{
	return native_pmd_val(pmd) & PTE_FLAGS_MASK;
}

static inline pte_t native_make_pte(pteval_t val)
{
	return (pte_t) { .pte = val };
}

static inline pteval_t native_pte_val(pte_t pte)
{
	return pte.pte;
}

static inline pteval_t pte_flags(pte_t pte)
{
	return native_pte_val(pte) & PTE_FLAGS_MASK;
}

#define pgprot_val(x)	((x).pgprot)
#define __pgprot(x)	((pgprot_t) { (x) } )

extern pteval_t __supported_pte_mask;

#define pgprot_writecombine	pgprot_writecombine

#define __HAVE_PFNMAP_TRACKING

struct file;

struct seq_file;

enum pg_level {
	PG_LEVEL_NONE,
	PG_LEVEL_4K,
	PG_LEVEL_2M,
	PG_LEVEL_1G,
	PG_LEVEL_NUM
};

#endif /* ! __ASM_PGTABLE_TYPES_H__ */
