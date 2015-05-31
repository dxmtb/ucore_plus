#ifndef __ASM_GENERIC_PGTABLE_H__
#define __ASM_GENERIC_PGTABLE_H__

#include <linux/mm_types.h>
#include <linux/bug.h>

/*
 * When walking page tables, we usually want to skip any p?d_none entries;
 * and any p?d_bad entries - reporting the error before resetting to none.
 * Do the tests inline, but report and clear the bad entry in mm/memory.c.
 */
void pgd_clear_bad(pgd_t *);
void pud_clear_bad(pud_t *);
void pmd_clear_bad(pmd_t *);

static inline pte_t __ptep_modify_prot_start(struct mm_struct *mm,
					     unsigned long addr,
					     pte_t *ptep)
{
	/*
	 * Get the current pte state, but zero it out to make it
	 * non-present, preventing the hardware from asynchronously
	 * updating it.
	 */
	return ptep_get_and_clear(mm, addr, ptep);
}

static inline void __ptep_modify_prot_commit(struct mm_struct *mm,
					     unsigned long addr,
					     pte_t *ptep, pte_t pte)
{
	/*
	 * The pte is non-present, so there's no hardware state to
	 * preserve.
	 */
	set_pte_at(mm, addr, ptep, pte);
}

static inline int pmd_trans_huge(pmd_t pmd)
{
	return 0;
}

static inline pmd_t pmd_read_atomic(pmd_t *pmdp)
{
	/*
	 * Depend on compiler for an atomic pmd read. NOTE: this is
	 * only going to work, if the pmdval_t isn't larger than
	 * an unsigned long.
	 */
	return *pmdp;
}

#endif /* ! __ASM_GENERIC_PGTABLE_H__ */
