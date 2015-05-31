#ifndef __LINUX_HUGETLB_INLINE_H__
#define __LINUX_HUGETLB_INLINE_H__

#include <linux/mm.h>

static inline int is_vm_hugetlb_page(struct vm_area_struct *vma)
{
	return !!(vma->vm_flags & VM_HUGETLB);
}

#endif /* ! __LINUX_HUGETLB_INLINE_H__ */
