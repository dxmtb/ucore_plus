#ifndef __ASM_PGTABLE_H__
#define __ASM_PGTABLE_H__

#include <asm/page.h>
#include <asm/e820.h>

#include <asm/pgtable_types.h>

/*
 * Macro to mark a page protection value as UC-
 */
#define pgprot_noncached(prot)					\
	((boot_cpu_data.x86 > 3)				\
	 ? (__pgprot(pgprot_val(prot) | _PAGE_CACHE_UC_MINUS))	\
	 : (prot))

#include <asm/x86_init.h>

#define set_pte_at(mm, addr, ptep, pte)	native_set_pte_at(mm, addr, ptep, pte)

#define pte_clear(mm, addr, ptep)	native_pte_clear(mm, addr, ptep)

#define pmd_update(mm, addr, ptep)              do { } while (0)

#define pgd_val(x)	native_pgd_val(x)

#define pud_val(x)	native_pud_val(x)

#define pmd_val(x)	native_pmd_val(x)
#define __pmd(x)	native_make_pmd(x)

#define pte_val(x)	native_pte_val(x)
#define __pte(x)	native_make_pte(x)

static inline pte_t pte_set_flags(pte_t pte, pteval_t set)
{
	pteval_t v = native_pte_val(pte);

	return native_make_pte(v | set);
}

static inline pte_t pte_clear_flags(pte_t pte, pteval_t clear)
{
	pteval_t v = native_pte_val(pte);

	return native_make_pte(v & ~clear);
}

static inline pte_t pte_mkwrite(pte_t pte)
{
	return pte_set_flags(pte, _PAGE_RW);
}

static inline pmd_t pmd_set_flags(pmd_t pmd, pmdval_t set)
{
	pmdval_t v = native_pmd_val(pmd);

	return __pmd(v | set);
}

static inline pmd_t pmd_clear_flags(pmd_t pmd, pmdval_t clear)
{
	pmdval_t v = native_pmd_val(pmd);

	return __pmd(v & ~clear);
}

/*
 * Mask out unsupported bits in a present pgprot.  Non-present pgprots
 * can use those bits for other purposes, so leave them be.
 */
static inline pgprotval_t massage_pgprot(pgprot_t pgprot)
{
	pgprotval_t protval = pgprot_val(pgprot);

	if (protval & _PAGE_PRESENT)
		protval &= __supported_pte_mask;

	return protval;
}

#define pgprot_modify pgprot_modify

# include <asm/pgtable_64.h>

#include <linux/mm_types.h>
#include <linux/log2.h>

#define __HAVE_ARCH_PTE_SAME

#define pte_accessible pte_accessible

static inline int pmd_none(pmd_t pmd)
{
	/* Only check low word on 32-bit platforms, since it might be
	   out of sync with upper half. */
	return (unsigned long)native_pmd_val(pmd) == 0;
}

static inline unsigned long pmd_page_vaddr(pmd_t pmd)
{
	return (unsigned long)__va(pmd_val(pmd) & PTE_PFN_MASK);
}

/*
 * the pmd page can be thought of an array like this: pmd_t[PTRS_PER_PMD]
 *
 * this macro returns the index of the entry in the pmd page which would
 * control the given virtual address
 */
static inline unsigned long pmd_index(unsigned long address)
{
	return (address >> PMD_SHIFT) & (PTRS_PER_PMD - 1);
}

/*
 * the pte page can be thought of an array like this: pte_t[PTRS_PER_PTE]
 *
 * this function returns the index of the entry in the pte page which would
 * control the given virtual address
 */
static inline unsigned long pte_index(unsigned long address)
{
	return (address >> PAGE_SHIFT) & (PTRS_PER_PTE - 1);
}

static inline int pmd_bad(pmd_t pmd)
{
#ifdef CONFIG_NUMA_BALANCING
	/* pmd_numa check */
	if ((pmd_flags(pmd) & (_PAGE_NUMA|_PAGE_PRESENT)) == _PAGE_NUMA)
		return 0;
#endif
	return (pmd_flags(pmd) & ~_PAGE_USER) != _KERNPG_TABLE;
}

static inline int pud_none(pud_t pud)
{
	return native_pud_val(pud) == 0;
}

static inline unsigned long pud_page_vaddr(pud_t pud)
{
	return (unsigned long)__va((unsigned long)pud_val(pud) & PTE_PFN_MASK);
}

static inline pmd_t *pmd_offset(pud_t *pud, unsigned long address)
{
	return (pmd_t *)pud_page_vaddr(*pud) + pmd_index(address);
}

static inline int pud_bad(pud_t pud)
{
	return (pud_flags(pud) & ~(_KERNPG_TABLE | _PAGE_USER)) != 0;
}

static inline unsigned long pgd_page_vaddr(pgd_t pgd)
{
	return (unsigned long)__va((unsigned long)pgd_val(pgd) & PTE_PFN_MASK);
}

static inline unsigned long pud_index(unsigned long address)
{
	return (address >> PUD_SHIFT) & (PTRS_PER_PUD - 1);
}

static inline pud_t *pud_offset(pgd_t *pgd, unsigned long address)
{
	return (pud_t *)pgd_page_vaddr(*pgd) + pud_index(address);
}

static inline int pgd_bad(pgd_t pgd)
{
	return (pgd_flags(pgd) & ~_PAGE_USER) != _KERNPG_TABLE;
}

static inline int pgd_none(pgd_t pgd)
{
	return !native_pgd_val(pgd);
}

static inline pte_t native_local_ptep_get_and_clear(pte_t *ptep)
{
	pte_t res = *ptep;

	/* Pure native function needs no input for mm, addr */
	native_pte_clear(NULL, 0, ptep);
	return res;
}

static inline void native_set_pte_at(struct mm_struct *mm, unsigned long addr,
				     pte_t *ptep , pte_t pte)
{
	native_set_pte(ptep, pte);
}

/*
 * Rules for using pte_update - it must be called after any PTE update which
 * has not been done using the set_pte / clear_pte interfaces.  It is used by
 * shadow mode hypervisors to resynchronize the shadow page tables.  Kernel PTE
 * updates should either be sets, clears, or set_pte_atomic for P->P
 * transitions, which means this hook should only be called for user PTEs.
 * This hook implies a P->P protection or access change has taken place, which
 * requires a subsequent TLB flush.  The notification can optionally be delayed
 * until the TLB flush event by using the pte_update_defer form of the
 * interface, but care must be taken to assure that the flush happens while
 * still holding the same page table lock so that the shadow and primary pages
 * do not become out of sync on SMP.
 */
#define pte_update(mm, addr, ptep)		do { } while (0)

/*
 * We only update the dirty/accessed state if we set
 * the dirty bit by hand in the kernel, since the hardware
 * will do the accessed bit for us, and we don't want to
 * race with other CPU's that might be updating the dirty
 * bit at the same time.
 */
struct vm_area_struct;

#define  __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS

#define __HAVE_ARCH_PTEP_TEST_AND_CLEAR_YOUNG

#define __HAVE_ARCH_PTEP_CLEAR_YOUNG_FLUSH

#define __HAVE_ARCH_PTEP_GET_AND_CLEAR
static inline pte_t ptep_get_and_clear(struct mm_struct *mm, unsigned long addr,
				       pte_t *ptep)
{
	pte_t pte = native_ptep_get_and_clear(ptep);
	pte_update(mm, addr, ptep);
	return pte;
}

#define __HAVE_ARCH_PTEP_GET_AND_CLEAR_FULL

#define __HAVE_ARCH_PTEP_SET_WRPROTECT

#define flush_tlb_fix_spurious_fault(vma, address) do { } while (0)

#define  __HAVE_ARCH_PMDP_SET_ACCESS_FLAGS

#define __HAVE_ARCH_PMDP_TEST_AND_CLEAR_YOUNG

#define __HAVE_ARCH_PMDP_CLEAR_YOUNG_FLUSH

#define __HAVE_ARCH_PMDP_SPLITTING_FLUSH

#define __HAVE_ARCH_PMD_WRITE

#define __HAVE_ARCH_PMDP_GET_AND_CLEAR

#define __HAVE_ARCH_PMDP_SET_WRPROTECT

#define PTE_SHIFT ilog2(PTRS_PER_PTE)
static inline int page_level_shift(enum pg_level level)
{
	return (PAGE_SHIFT - PTE_SHIFT) + level * PTE_SHIFT;
}
static inline unsigned long page_level_size(enum pg_level level)
{
	return 1UL << page_level_shift(level);
}

#include <asm-generic/pgtable.h>

#endif /* ! __ASM_PGTABLE_H__ */
